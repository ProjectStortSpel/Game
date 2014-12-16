#include "LuaNumberArray.h"

namespace LuaEmbedder
{
  template<>
  int LuaNumberArray<int>::Get(lua_State* L)
  {
    const char* name = lua_tostring(L, lua_upvalueindex(1));
    int** pArray = (int**)luaL_checkudata(L, 1, name);
    if (lua_isnumber(L, 2))
    {
      int index = lua_tointeger(L, 2);
      lua_pushinteger(L, (*pArray)[index - 1]);
    }
    else
      lua_pushnil(L);
    return 1;
  }
  template<>
  int LuaNumberArray<unsigned int>::Get(lua_State* L)
  {
    const char* name = lua_tostring(L, lua_upvalueindex(1));
    unsigned int** pArray = (unsigned int**)luaL_checkudata(L, 1, name);
    if (lua_isnumber(L, 2))
    {
      int index = lua_tointeger(L, 2);
      lua_pushinteger(L, (unsigned int)((*pArray)[index - 1]));
    }
    else
      lua_pushnil(L);
    return 1;
  }
  
  template<>
  int LuaNumberArray<int>::Set(lua_State* L)
  {
    const char* name = lua_tostring(L, lua_upvalueindex(1));
    int** pArray = (int**)luaL_checkudata(L, 1, name);
    if (lua_isnumber(L, 2) && lua_isnumber(L, 3))
    {
      int index = lua_tointeger(L, 2);
      int value = lua_tointeger(L, 3);
      (*pArray)[index - 1] = value;
    }
    return 0;
  }
  template<>
  int LuaNumberArray<unsigned int>::Set(lua_State* L)
  {
    const char* name = lua_tostring(L, lua_upvalueindex(1));
    unsigned int** pArray = (unsigned int**)luaL_checkudata(L, 1, name);
    if (lua_isnumber(L, 2) && lua_isnumber(L, 3))
    {
      int index = lua_tointeger(L, 2);
      unsigned int value = (unsigned int)lua_tointeger(L, 3);
      (*pArray)[index - 1] = value;
    }
    return 0;
  }
}