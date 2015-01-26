#include "LuaEmbedder.h"
#include "LuaNumberArray.h"

#include <SDL/SDL.h>
#include <File/File.h>
#include <sstream>

namespace LuaEmbedder
{
  lua_State* L = nullptr;
  std::vector<int (*)()> Functions = std::vector<int (*)()>();
  
  void Init()
  {
    L = luaL_newstate();
    luaL_openlibs(L);
    
    LuaNumberArray<float>::Embed(L, "FloatArray");
    LuaNumberArray<int>::Embed(L, "IntArray");
    LuaNumberArray<unsigned int>::Embed(L, "UnsignedIntArray");  
  }
  void Quit()
  {
    lua_gc(L, LUA_GCCOLLECT, 0);
    lua_close(L);
  }
  
  std::string LoadFile(const std::string& filepath)
  {
    // Read source
    char* source = File::Read(filepath);
    std::string sourceString = std::string(source);
    delete source;
    // Set directory string
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

  bool Load(const std::string& filepath)
  {
    std::string source = LoadFile(filepath);
    bool error = luaL_dostring(L, source.c_str());
	//bool error = luaL_dofile(L, filepath.c_str());
    if (error)
    {
      SDL_Log("LuaEmbedder::Load : %s", (lua_isstring(L, -1) ? lua_tostring(L, -1) : "Unknown error"));
      return false;
    }
    lua_gc(L, LUA_GCCOLLECT, 0);
    return true;
  }
  bool CallFunction(const std::string& name, int argumentCount, const std::string& library)
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
  
  void CollectGarbage()
  {
    lua_gc(L, LUA_GCCOLLECT, 0);
  }
  
  void CollectGarbage(int durationInMilliseconds)
  {
    lua_gc(L, LUA_GCSETSTEPMUL, durationInMilliseconds);
    lua_gc(L, LUA_GCSTEP, 0);
  }
  
  int GetMemoryUsage()
  {
    return lua_gc(L, LUA_GCCOUNT, 0);
  }
  
  #define ADD_VARIABLE(type) \
    if (library.empty()) \
    { \
      lua_push##type(L, value); \
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
      \
      lua_pushstring(L, name.c_str()); \
      lua_push##type(L, value); \
      lua_settable(L, -3); \
      lua_setglobal(L, library.c_str()); \
    }
  void AddFloat(const std::string& name, float value, const std::string& library)
  {
    ADD_VARIABLE(number);
  }
  void AddInt(const std::string& name, int value, const std::string& library)
  {
    ADD_VARIABLE(integer);
  }
  void AddBool(const std::string& name, bool value, const std::string& library)
  {
    ADD_VARIABLE(boolean);
  }
  void AddString(const std::string& name, const char* value, const std::string& library)
  {
    ADD_VARIABLE(string);
  }
  static int FunctionDispatch(lua_State* L)
  {
    assert(lua_isnumber(L, lua_upvalueindex(1)));
    int functionIndex = (int)lua_tonumber(L, lua_upvalueindex(1));
    return (*(Functions[functionIndex]))();
  }
  void AddFunction(const std::string& name, int (*functionPointer)(), const std::string& library)
  {
    if (library.empty())
    {
      int functionIndex = (int)Functions.size();
      Functions.push_back(functionPointer);
      
      lua_pushinteger(L, functionIndex);
      lua_pushcclosure(L, FunctionDispatch, 1);
      lua_setglobal(L, name.c_str());
    }
    else
    {
      lua_getglobal(L, library.c_str());
      if (lua_isnil(L, -1))
      {
	lua_pop(L, 1);
	luaL_newmetatable(L, library.c_str());
      }
      
      int functionIndex = (int)Functions.size();
      Functions.push_back(functionPointer);
      
      lua_pushstring(L, name.c_str()); 
      lua_pushinteger(L, functionIndex);
      lua_pushcclosure(L, FunctionDispatch, 1);
      lua_settable(L, -3);
      lua_setglobal(L, library.c_str());
    }
  }
  
  float PullFloat(int index)
  {
    if (!lua_isnumber(L, index))
	  SDL_Log("LuaEmbedder::PullFloat : Element at index %d is not a number", index);
    return (float)lua_tonumber(L, index);
  }
  int PullInt(int index)
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
  bool PullBool(int index)
  {
    if (!lua_isboolean(L, index))
	  SDL_Log("LuaEmbedder::PullBool : Element at index %d is not a boolean", index);
    return (bool)lua_toboolean(L, index);
  }
  std::string PullString(int index)
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
  float PullFloat(const std::string& name, const std::string& library)
  {
    PULL_GLOBAL_VARIABLE();
    return PullFloat(-1);
  }
  int PullInt(const std::string& name, const std::string& library)
  {
    PULL_GLOBAL_VARIABLE();
    return PullInt(-1);
  }
  bool PullBool(const std::string& name, const std::string& library)
  {
    PULL_GLOBAL_VARIABLE();
    return PullBool(-1);
  }
  std::string PullString(const std::string& name, const std::string& library)
  {
    PULL_GLOBAL_VARIABLE();
    return PullString(-1);
  }
  
  void PushFloat(float value)
  {
    lua_pushnumber(L, value);
  }
  void PushInt(int value)
  {
    lua_pushinteger(L, (lua_Integer)value);
  }
  void PushBool(bool value)
  {
    lua_pushboolean(L, (int)value);
  }
  void PushString(const std::string& value)
  {
    lua_pushstring(L, value.c_str());
  }
  void PushNull()
  {
    lua_pushnil(L);
  }
  void PushFloatArray(const float* values, unsigned int size, bool remove)
  {
    LuaNumberArray<float>::Push(L, "FloatArray", values, size, remove);
  }
  void PushIntArray(const int* values, unsigned int size, bool remove)
  {
    LuaNumberArray<int>::Push(L, "IntArray", values, size, remove);
  }
  void PushUnsignedIntArray(const unsigned int* values, unsigned int size, bool remove)
  {
    LuaNumberArray<unsigned int>::Push(L, "UnsignedIntArray", values, size, remove);
  }
  void PushBoolArray(const bool* values, unsigned int size)
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
  void PushStringArray(const std::string* values, unsigned int size)
  {
    lua_newtable(L);
    for (unsigned int i = 0; i < size; ++i)
    {
      lua_pushstring(L, values[i].c_str());
      lua_rawseti(L, -2, i + 1);
    }
  }
  
  bool IsFloat(int index)
  {
    return lua_isnumber(L, index);
  }
  bool IsInt(int index)
  {
    return lua_isnumber(L, index);
  }
  bool IsBool(int index)
  {
    return lua_isboolean(L, index);
  }
  bool IsString(int index)
  {
    return lua_isstring(L, index);
  }
  bool IsFunction(int index)
  {
    return lua_isfunction(L, index);
  }
    
  void SaveFunction(int index, const std::string& key)
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
  
  bool CallSavedFunction(const std::string& key, int argumentCount)
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