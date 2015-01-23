#include "LuaEmbedder.h"
#include "LuaNumberArray.h"

#include <SDL/SDL.h>
#include <sstream>

namespace LuaEmbedder
{
  std::map<lua_State*, std::vector<lua_State*>> LuaStates = std::map<lua_State*, std::vector<lua_State*>>();
  std::map<lua_State*, lua_State*> LuaThreads = std::map<lua_State*, lua_State*>();
  std::vector<int (*)(lua_State*)> Functions = std::vector<int (*)(lua_State*)>();
  
  lua_State* CreateState()
  {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    
    LuaNumberArray<float>::Embed(L, "FloatArray");
    LuaNumberArray<int>::Embed(L, "IntArray");
    LuaNumberArray<unsigned int>::Embed(L, "UnsignedIntArray");
    
    assert(LuaStates.find(L) == LuaStates.end());
    LuaStates[L] = std::vector<lua_State*>();
    return L;
  }
  lua_State* CopyState(lua_State* L)
  {
    assert(LuaStates.find(L) != LuaStates.end());
    lua_State* copy = luaL_newstate();
    luaL_openlibs(copy);
    LuaStates[L].push_back(copy);
    LuaThreads[copy] = L;
    return copy;
  }
  void Quit()
  {
    for (std::map<lua_State*, std::vector<lua_State*>>::iterator it0 = LuaStates.begin(); it0 != LuaStates.end(); it0++)
    {
      for (std::vector<lua_State*>::iterator it1 = it0->second.begin(); it1 != it0->second.end(); it1++)
	lua_gc((*it1), LUA_GCCOLLECT, 0);
      lua_gc((it0->first), LUA_GCCOLLECT, 0);
      lua_close((it0->first));
    }
    LuaStates.clear();
    LuaThreads.clear();
  }
  
  std::string LoadFile(const std::string& filepath)
  {
    SDL_RWops* file = SDL_RWFromFile(filepath.c_str(), "r");
    if (file == NULL)
    {
      SDL_Log("File %s not found", filepath.c_str());
      return std::string();
    }
    Sint64 length = SDL_RWseek(file, 0, RW_SEEK_END);
    if (length <= 0)
    {
      SDL_Log("Length of file %s lower than or equal to zero", filepath.c_str());
      return std::string();
    }
    SDL_RWseek(file, 0, RW_SEEK_SET);
    char* source = new char[length + 1];
    SDL_RWread(file, source, length, 1);
    source[length] = '\0';
    SDL_RWclose(file);
    std::string sourceString = std::string(source);
    delete source;
    
    std::string directory = filepath.substr(0, filepath.rfind('\\/') + 1);

    size_t prevPackagePathIndex = 0;
    size_t prevRequireIndex = 0;
    while (true)
    {
      size_t currPackagePathIndex = sourceString.find("package.path = package.path .. \";", prevPackagePathIndex);
      size_t currRequireIndex = sourceString.find("require \"", prevRequireIndex);

      if (currRequireIndex == std::string::npos)
	break;

      if (currPackagePathIndex == std::string::npos || currRequireIndex < currPackagePathIndex)
      {
	size_t commentIndex = sourceString.find_last_of("--", currRequireIndex);
	size_t newlineIndex = sourceString.find_last_of('\n', currRequireIndex);
	if (commentIndex > newlineIndex)
	{
	  prevRequireIndex = sourceString.find('\n', currRequireIndex);
	  continue;
	}
	
	currRequireIndex += 9;
	size_t foundQuotationMark = sourceString.find("\"", currRequireIndex);
	if (foundQuotationMark != std::string::npos)
	{
	  size_t length = foundQuotationMark - currRequireIndex;
	  std::string file = sourceString.substr(currRequireIndex, length);
	  std::string subSourceString = LoadFile(directory + file + std::string(".lua"));
	  subSourceString.push_back('\n');
	  currRequireIndex -= 9;
	  sourceString.erase(currRequireIndex, length + 10);
	  sourceString.insert(currRequireIndex, subSourceString);
	}
	prevRequireIndex = currRequireIndex;
      }
      else
      {
	size_t commentIndex = sourceString.find_last_of("--", currPackagePathIndex);
	size_t newlineIndex = sourceString.find_last_of('\n', currPackagePathIndex);
	if (commentIndex > newlineIndex)
	{
	  prevPackagePathIndex = sourceString.find('\n', currPackagePathIndex);
	  continue;
	}
	
	currPackagePathIndex += 33;
	size_t foundQuestionMark = sourceString.find("?.lua", currPackagePathIndex);
	if (foundQuestionMark != std::string::npos)
	{
	  size_t length = foundQuestionMark - currPackagePathIndex;
	  #ifdef _DEBUG
	    directory = sourceString.substr(currPackagePathIndex, length);
	  #else
	    size_t foundContentFolder = sourceString.find("content/", currPackagePathIndex);
	    if (foundContentFolder != std::string::npos)
	      directory = sourceString.substr(foundContentFolder, foundQuestionMark - foundContentFolder);
	    else
	      directory = sourceString.substr(currPackagePathIndex, length);
	  #endif
	  currPackagePathIndex -= 33;
	  sourceString.erase(currPackagePathIndex, length + 39);
	}
	prevPackagePathIndex = currPackagePathIndex;
      }
    }
    return sourceString;
  }

  bool Load(lua_State* L, const std::string& filepath)
  {
    std::string source = LoadFile(filepath);
    bool error = luaL_dostring(L, source.c_str());
    if (error)
    {
      SDL_Log("LuaEmbedder::Load : %s", (lua_isstring(L, -1) ? lua_tostring(L, -1) : "Unknown error"));
      return false;
    }
    lua_gc(L, LUA_GCCOLLECT, 0);
    return true;
  }
  bool Preload(lua_State* L, const std::string& filepath)
  {
    std::string source = LoadFile(filepath);
    bool error = luaL_loadstring(L, source.c_str());
    if (error)
    {
      SDL_Log("LuaEmbedder::Preload : %s", (lua_isstring(L, -1) ? lua_tostring(L, -1) : "Unknown error"));
      return false;
    }
    lua_gc(L, LUA_GCCOLLECT, 0);
    return true;
  }
  bool CallFunction(lua_State* L, const std::string& name, int argumentCount, const std::string& library)
  {
    if (library.empty())
    {
      lua_getglobal(L, name.c_str());
    }
    else
    {
      lua_getglobal(L, library.c_str());
      assert(!lua_isnil(L, -1));
      lua_pushstring(L, name.c_str());
      lua_gettable(L, -2);
    }
    bool error = lua_pcall(L, argumentCount, LUA_MULTRET, 0);
    lua_gc(L, LUA_GCCOLLECT, 0);
    if (error)
    {
	  SDL_Log("LuaEmbedder::CallFunction : %s", (lua_isstring(L, -1) ? lua_tostring(L, -1) : "Unknown error"));
	  return false;
    }
    return true;
  }
  
  void CollectGarbage(lua_State* L)
  {
    lua_gc(L, LUA_GCCOLLECT, 0);
  }
  void CollectGarbage(lua_State* L, int durationInMilliseconds)
  {
    lua_gc(L, LUA_GCSETSTEPMUL, durationInMilliseconds);
    lua_gc(L, LUA_GCSTEP, 0);
  }
  int GetMemoryUsage()
  {
    int memoryUsage = 0;
    for (std::map<lua_State*, std::vector<lua_State*>>::iterator it0 = LuaStates.begin(); it0 != LuaStates.end(); it0++)
    {
      for (std::vector<lua_State*>::iterator it1 = it0->second.begin(); it1 != it0->second.end(); it1++)
	memoryUsage += lua_gc((*it1), LUA_GCCOUNT, 0);
      memoryUsage += lua_gc((it0->first), LUA_GCCOUNT, 0);
    }
    return memoryUsage;
  }
  void CollectGarbage()
  {
    for (std::map<lua_State*, std::vector<lua_State*>>::iterator it0 = LuaStates.begin(); it0 != LuaStates.end(); it0++)
    {
      for (std::vector<lua_State*>::iterator it1 = it0->second.begin(); it1 != it0->second.end(); it1++)
	lua_gc((*it1), LUA_GCCOLLECT, 0);
      lua_gc((it0->first), LUA_GCCOLLECT, 0);
    }
  }
  
  #define ADD_VARIABLE(luaState, type) \
    if (library.empty()) \
    { \
      lua_push##type(luaState, value); \
      lua_setglobal(luaState, name.c_str()); \
    } \
    else \
    { \
      lua_getglobal(luaState, library.c_str()); \
      if (lua_isnil(luaState, -1)) \
      { \
	lua_pop(luaState, 1); \
	luaL_newmetatable(luaState, library.c_str()); \
      } \
      \
      lua_pushstring(luaState, name.c_str()); \
      lua_push##type(luaState, value); \
      lua_settable(luaState, -3); \
      lua_setglobal(luaState, library.c_str()); \
    }
  void AddFloat(lua_State* L, const std::string& name, float value, const std::string& library)
  {
    std::map<lua_State*, lua_State*>::iterator it0 = LuaThreads.find(L);
    std::map<lua_State*, std::vector<lua_State*>>::iterator it1;
    if (it0 != LuaThreads.end())
      it1 = LuaStates.find(it0->second);
    else
    {
      it1 = LuaStates.find(L);
      if (it1 == LuaStates.end())
      {
	ADD_VARIABLE(L, number);
	return;
      }
    }
    assert(it1 != LuaStates.end());
    for (std::vector<lua_State*>::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
      ADD_VARIABLE((*it2), number);
    ADD_VARIABLE(it1->first, number);
  }
  void AddInt(lua_State* L, const std::string& name, int value, const std::string& library)
  {
    std::map<lua_State*, lua_State*>::iterator it0 = LuaThreads.find(L);
    std::map<lua_State*, std::vector<lua_State*>>::iterator it1;
    if (it0 != LuaThreads.end())
      it1 = LuaStates.find(it0->second);
    else
    {
      it1 = LuaStates.find(L);
      if (it1 == LuaStates.end())
      {
	ADD_VARIABLE(L, integer);
	return;
      }
    }
    assert(it1 != LuaStates.end());
    for (std::vector<lua_State*>::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
      ADD_VARIABLE((*it2), integer);
    ADD_VARIABLE(it1->first, integer);
  }
  void AddBool(lua_State* L, const std::string& name, bool value, const std::string& library)
  {
    std::map<lua_State*, lua_State*>::iterator it0 = LuaThreads.find(L);
    std::map<lua_State*, std::vector<lua_State*>>::iterator it1;
    if (it0 != LuaThreads.end())
      it1 = LuaStates.find(it0->second);
    else
    {
      it1 = LuaStates.find(L);
      if (it1 == LuaStates.end())
      {
	ADD_VARIABLE(L, boolean);
	return;
      }
    }
    assert(it1 != LuaStates.end());
    for (std::vector<lua_State*>::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
      ADD_VARIABLE((*it2), boolean);
    ADD_VARIABLE(it1->first, boolean);
  }
  void AddString(lua_State* L, const std::string& name, const char* value, const std::string& library)
  {
    std::map<lua_State*, lua_State*>::iterator it0 = LuaThreads.find(L);
    std::map<lua_State*, std::vector<lua_State*>>::iterator it1;
    if (it0 != LuaThreads.end())
      it1 = LuaStates.find(it0->second);
    else
    {
      it1 = LuaStates.find(L);
      if (it1 == LuaStates.end())
      {
	ADD_VARIABLE(L, string);
	return;
      }
    }
    assert(it1 != LuaStates.end());
    for (std::vector<lua_State*>::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
      ADD_VARIABLE((*it2), string);
    ADD_VARIABLE(it1->first, string);
  }
  static int FunctionDispatch(lua_State* L)
  {
    assert(lua_isnumber(L, lua_upvalueindex(1)));
    int functionIndex = (int)lua_tonumber(L, lua_upvalueindex(1));
    return (*(Functions[functionIndex]))(L);
  }
  #define ADD_FUNCTION(luaState) \
    if (library.empty()) \
    { \
      int functionIndex = (int)Functions.size(); \
      Functions.push_back(functionPointer); \
      lua_pushinteger(L, functionIndex); \
      lua_pushcclosure(L, FunctionDispatch, 1); \
      lua_setglobal(L, name.c_str()); \
    } \
    else \
    { \
      lua_getglobal(L, library.c_str()); \
      if (lua_isnil(L, -1)) \
      { \
	lua_pop(L, 1); \
	luaL_newmetatable(L, library.c_str()); \
      } \
      int functionIndex = (int)Functions.size(); \
      Functions.push_back(functionPointer); \
      lua_pushstring(L, name.c_str());  \
      lua_pushinteger(L, functionIndex); \
      lua_pushcclosure(L, FunctionDispatch, 1); \
      lua_settable(L, -3); \
      lua_setglobal(L, library.c_str()); \
    }
  void AddFunction(lua_State* L, const std::string& name, int (*functionPointer)(lua_State*), const std::string& library)
  {
    std::map<lua_State*, lua_State*>::iterator it0 = LuaThreads.find(L);
    std::map<lua_State*, std::vector<lua_State*>>::iterator it1;
    if (it0 != LuaThreads.end())
      it1 = LuaStates.find(it0->second);
    else
    {
      it1 = LuaStates.find(L);
      if (it1 == LuaStates.end())
      {
	ADD_FUNCTION(L);
	return;
      }
    }
    assert(it1 != LuaStates.end());
    for (std::vector<lua_State*>::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
      ADD_FUNCTION((*it2));
    ADD_FUNCTION(it1->first);
  }
  void AddFloat(const std::string& name, float value, const std::string& library)
  {
    for (std::map<lua_State*, std::vector<lua_State*>>::iterator it0 = LuaStates.begin(); it0 != LuaStates.end(); it0++)
    {
      for (std::vector<lua_State*>::iterator it1 = it0->second.begin(); it1 != it0->second.end(); it1++)
	AddFloat((*it1), name, value, library);
      AddFloat((it0->first), name, value, library);
    }
  }
  void AddInt(const std::string& name, int value, const std::string& library)
  {
    for (std::map<lua_State*, std::vector<lua_State*>>::iterator it0 = LuaStates.begin(); it0 != LuaStates.end(); it0++)
    {
      for (std::vector<lua_State*>::iterator it1 = it0->second.begin(); it1 != it0->second.end(); it1++)
	AddInt((*it1), name, value, library);
      AddInt((it0->first), name, value, library);
    }
  }
  void AddBool(const std::string& name, bool value, const std::string& library)
  {
    for (std::map<lua_State*, std::vector<lua_State*>>::iterator it0 = LuaStates.begin(); it0 != LuaStates.end(); it0++)
    {
      for (std::vector<lua_State*>::iterator it1 = it0->second.begin(); it1 != it0->second.end(); it1++)
	AddBool((*it1), name, value, library);
      AddBool((it0->first), name, value, library);
    }
  }
  void AddString(const std::string& name, const char* value, const std::string& library)
  {
    for (std::map<lua_State*, std::vector<lua_State*>>::iterator it0 = LuaStates.begin(); it0 != LuaStates.end(); it0++)
    {
      for (std::vector<lua_State*>::iterator it1 = it0->second.begin(); it1 != it0->second.end(); it1++)
	AddString((*it1), name, value, library);
      AddString((it0->first), name, value, library);
    }
  }
  void AddFunction(const std::string& name, int (*functionPointer)(lua_State*), const std::string& library)
  {
    for (std::map<lua_State*, std::vector<lua_State*>>::iterator it0 = LuaStates.begin(); it0 != LuaStates.end(); it0++)
    {
      for (std::vector<lua_State*>::iterator it1 = it0->second.begin(); it1 != it0->second.end(); it1++)
	AddFunction((*it1), name, functionPointer, library);
      AddFunction((it0->first), name, functionPointer, library);
    }
  }
  
  float PullFloat(lua_State* L, int index)
  {
    if (!lua_isnumber(L, index))
	  SDL_Log("LuaEmbedder::PullFloat : Element at index %d is not a number", index);
    return (float)lua_tonumber(L, index);
  }
  int PullInt(lua_State* L, int index)
  {
    if (!lua_isnumber(L, index))
    {
      SDL_Log("LuaEmbedder::PullInt : Element at index %d is not a number", index);
      /*lua_Debug debugInfo;
      lua_getstack(L, 1, &debugInfo);
      lua_getinfo(L, "nSlu", &debugInfo);
      std::string source = std::string(debugInfo.source);
      int currentLine = 0, targetLine = debugInfo.currentline;

      std::istringstream iss(source);
      std::string line;
      std::string info;
      while (std::getline(iss, line))
      {
	      if (currentLine == targetLine - 1)
	      {
		      info.insert(info.end(), line.begin(), line.end());
		      info.push_back('\n');
	      }
	      else if (currentLine == targetLine)
	      {
		      info.insert(info.end(), line.begin(), line.end());
		      info.push_back('\n');
	      }
	      else if (currentLine == targetLine + 1)
	      {
		      info.insert(info.end(), line.begin(), line.end());
		      SDL_Log("%s", info.c_str());
		      break;
	      }
	      currentLine++;
      }*/
    }
    return (int)lua_tointeger(L, index);
  }
  bool PullBool(lua_State* L, int index)
  {
    if (!lua_isboolean(L, index))
	  SDL_Log("LuaEmbedder::PullBool : Element at index %d is not a boolean", index);
    return (bool)lua_toboolean(L, index);
  }
  std::string PullString(lua_State* L, int index)
  {
    if (!lua_isstring(L, index))
      SDL_Log("LuaEmbedder::PullString : Element at index %d is not a string", index);
    return std::string(lua_tostring(L, index));
  }
  #define PULL_GLOBAL_VARIABLE() \
    if (library.empty()) \
    { \
      lua_getglobal(L, name.c_str()); \
    } \
    else \
    { \
      lua_getglobal(L, library.c_str()); \
      assert(!lua_isnil(L, -1)); \
      lua_pushstring(L, name.c_str()); \
      lua_gettable(L, -2); \
    }
  float PullFloat(lua_State* L, const std::string& name, const std::string& library)
  {
    PULL_GLOBAL_VARIABLE();
    return PullFloat(L, -1);
  }
  int PullInt(lua_State* L, const std::string& name, const std::string& library)
  {
    PULL_GLOBAL_VARIABLE();
    return PullInt(L, -1);
  }
  bool PullBool(lua_State* L, const std::string& name, const std::string& library)
  {
    PULL_GLOBAL_VARIABLE();
    return PullBool(L, -1);
  }
  std::string PullString(lua_State* L, const std::string& name, const std::string& library)
  {
    PULL_GLOBAL_VARIABLE();
    return PullString(L, -1);
  }
  
  void PushFloat(lua_State* L, float value)
  {
    lua_pushnumber(L, value);
  }
  void PushInt(lua_State* L, int value)
  {
    lua_pushinteger(L, (lua_Integer)value);
  }
  void PushBool(lua_State* L, bool value)
  {
    lua_pushboolean(L, (int)value);
  }
  void PushString(lua_State* L, const std::string& value)
  {
    lua_pushstring(L, value.c_str());
  }
  void PushNull(lua_State* L)
  {
    lua_pushnil(L);
  }
  void PushFloatArray(lua_State* L, const float* values, unsigned int size, bool remove)
  {
    LuaNumberArray<float>::Push(L, "FloatArray", values, size, remove);
  }
  void PushIntArray(lua_State* L, const int* values, unsigned int size, bool remove)
  {
    LuaNumberArray<int>::Push(L, "IntArray", values, size, remove);
  }
  void PushUnsignedIntArray(lua_State* L, const unsigned int* values, unsigned int size, bool remove)
  {
    LuaNumberArray<unsigned int>::Push(L, "UnsignedIntArray", values, size, remove);
  }
  void PushBoolArray(lua_State* L, const bool* values, unsigned int size)
  {
    lua_newtable(L);
    for (unsigned int i = 0; i < size; ++i)
    {
      lua_pushboolean(L, (int)values[i]);
      lua_rawseti(L, -2, i + 1);
    }
    lua_pushliteral(L, "n");
    lua_pushinteger(L, (int)size);
    lua_rawset(L, -3);
  }
  void PushStringArray(lua_State* L, const std::string* values, unsigned int size)
  {
    lua_newtable(L);
    for (unsigned int i = 0; i < size; ++i)
    {
      lua_pushstring(L, values[i].c_str());
      lua_rawseti(L, -2, i + 1);
    }
  }
  
  bool IsFloat(lua_State* L, int index)
  {
    return lua_isnumber(L, index);
  }
  bool IsInt(lua_State* L, int index)
  {
    return lua_isnumber(L, index);
  }
  bool IsBool(lua_State* L, int index)
  {
    return lua_isboolean(L, index);
  }
  bool IsString(lua_State* L, int index)
  {
    return lua_isstring(L, index);
  }
  bool IsFunction(lua_State* L, int index)
  {
    return lua_isfunction(L, index);
  }
  
  void SaveFunction(lua_State* L, int index, const std::string& key)
  {
    if (!lua_isfunction(L, index))
    {
      SDL_Log("LuaEmbedder::SaveFunction : Element at index %d is not a function", index);
      return;
    }
    
    lua_getglobal(L, "saved_functions");
    if (lua_isnil(L, -1))
    {
      lua_pop(L, 1);
      lua_newtable(L);
      lua_pushglobaltable(L);
      lua_pushstring(L, "saved_functions");
      lua_pushvalue(L, -3);
      lua_settable(L, -3);
      lua_pop(L, 1);
    }
    lua_pushstring(L, key.c_str());
    lua_pushvalue(L, index);
    lua_settable(L, -3);
    lua_pop(L, 1);
  }
  
  bool CallSavedFunction(lua_State* L, const std::string& key, int argumentCount)
  {
    lua_getglobal(L, "saved_functions");
    if (lua_isnil(L, -1))
    {
      SDL_Log("LuaEmbedder::CallSavedFunction : Invalid key %s", key.c_str());
      return false;
    }
    
    lua_pushstring(L, key.c_str());
    lua_gettable(L, -2);
    if (lua_isnil(L, -1))
    {
      SDL_Log("LuaEmbedder::CallSavedFunction : Invalid key %s", key.c_str());
      return false;
    }
    
    lua_remove(L, -2);
    
    lua_insert(L, -(1 + argumentCount));
    bool error = lua_pcall(L, argumentCount, LUA_MULTRET, 0);
    lua_gc(L, LUA_GCCOLLECT, 0);
    if (error)
    {
      SDL_Log("LuaEmbedder::CallFunction : %s", (lua_isstring(L, -1) ? lua_tostring(L, -1) : "Unknown error"));
      return false;
    }
    
    return true;
  }
}