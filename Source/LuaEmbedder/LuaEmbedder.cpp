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
    assert(!luaL_dofile(L, filepath.c_str()));
    lua_gc(L, LUA_GCCOLLECT, 0);
  }
  void RunFunction(const std::string& functionName, int argumentCount)
  {
    lua_getglobal(L, functionName.c_str());
    assert(!lua_pcall(L, argumentCount, LUA_MULTRET, 0));
    lua_gc(L, LUA_GCCOLLECT, 0);
  }
  
  #define EMBED_VARIABLE(type) \
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
  void EmbedDouble(const std::string& name, double value, const std::string& library)
  {
    EMBED_VARIABLE(number);
  }
  void EmbedInt(const std::string& name, int value, const std::string& library)
  {
    EMBED_VARIABLE(integer);
  }
  void EmbedUnsignedInt(const std::string& name, unsigned int value, const std::string& library)
  {
    EMBED_VARIABLE(unsigned);
  }
  void EmbedBool(const std::string& name, bool value, const std::string& library)
  {
    EMBED_VARIABLE(boolean);
  }
  void EmbedString(const std::string& name, const char* value, const std::string& library)
  {
    EMBED_VARIABLE(string);
  }
  static int FunctionDispatch(lua_State* L)
  {
    assert(lua_isnumber(L, lua_upvalueindex(1)));
    int functionIndex = (int)lua_tonumber(L, lua_upvalueindex(1));
    return (*(Functions[functionIndex]))();
  }
  void EmbedFunction(const std::string& name, int (*functionPointer)(), const std::string& library)
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
  
  double PullDouble(int index)
  {
    assert(lua_isnumber(L, index));
    return (double)lua_tonumber(L, index);
  }
  int PullInt(int index)
  {
    assert(lua_isnumber(L, index));
    return (int)lua_tointeger(L, index);
  }
  unsigned int PullUnsignedInt(int index)
  {
    assert(lua_isnumber(L, index));
    return (unsigned int)lua_tounsigned(L, index);
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
  double PullDouble(const std::string& name, const std::string& library)
  {
    PULL_GLOBAL_VARIABLE();
    return PullDouble(-1);
  }
  int PullInt(const std::string& name, const std::string& library)
  {
    PULL_GLOBAL_VARIABLE();
    return PullInt(-1);
  }
  unsigned int PullUnsignedInt(const std::string& name, const std::string& library)
  {
    PULL_GLOBAL_VARIABLE();
    return PullUnsignedInt(-1);
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
  
  void PushDouble(double value)
  {
    lua_pushnumber(L, (lua_Number)value);
  }
  void PushInt(int value)
  {
    lua_pushinteger(L, (lua_Integer)value);
  }
  void PushUnsignedInt(unsigned int value)
  {
    lua_pushunsigned(L, (lua_Unsigned)value);
  }
  void PushBool(bool value)
  {
    lua_pushboolean(L, (int)value);
  }
  void PushString(const std::string& value)
  {
    lua_pushstring(L, value.c_str());
  }
}