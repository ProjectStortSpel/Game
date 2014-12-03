#include "LuaEmbedder.h"

namespace LuaEmbedder
{
  lua_State* L = nullptr;
  std::vector<int (*)()> Functions = std::vector<int (*)()>();
  
  void Init()
  {
    L = luaL_newstate();
    luaL_openlibs(L);
  }
  void Quit()
  {
    lua_gc(L, LUA_GCCOLLECT, 0);
    lua_close(L);
  }
  
  void Load(const std::string& filepath)
  {
	  bool isRight = luaL_dofile(L, filepath.c_str());
    assert(!isRight);
    lua_gc(L, LUA_GCCOLLECT, 0);
  }
  void CallFunction(const std::string& name, int argumentCount, const std::string& library)
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
    assert(!lua_pcall(L, argumentCount, LUA_MULTRET, 0));
    lua_gc(L, LUA_GCCOLLECT, 0);
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
    lua_remove(L, 1);
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
    assert(lua_isnumber(L, index));
    return (float)lua_tonumber(L, index);
  }
  int PullInt(int index)
  {
    assert(lua_isnumber(L, index));
    return (int)lua_tointeger(L, index);
  }
  bool PullBool(int index)
  {
    assert(lua_isboolean(L, index));
    return (bool)lua_toboolean(L, index);
  }
  std::string PullString(int index)
  {
    assert(lua_isstring(L, index));
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
}