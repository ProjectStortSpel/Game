#ifndef LUANUMBERARRAY_H
#define LUANUMBERARRAY_H

#include <Lua/lua.hpp>
#include <assert.h>
#include <string.h>

namespace LuaEmbedder
{
  template<class T> class LuaNumberArray
  {
  public:
    static void Embed(lua_State* L, const char* name)
    {
      luaL_newmetatable(L, name);
      int metatable = lua_gettop(L);
      
      lua_pushglobaltable(L);
      lua_pushstring(L, name);
      lua_pushvalue(L, metatable);
      lua_settable(L, -3);
      
      lua_pushstring(L, "__index");
      lua_pushstring(L, name);
      lua_pushcclosure(L, &Get, 1);
      lua_settable(L, metatable);

      lua_pushstring(L, "__newindex");
      lua_pushstring(L, name);
      lua_pushcclosure(L, &Set, 1);
      lua_settable(L, metatable);
      
      lua_pushstring(L, "__gc");
      lua_pushcfunction(L, &Remove);
      lua_settable(L, metatable);
      
      lua_pushstring(L, "__len");
      lua_pushstring(L, name);
      lua_pushcclosure(L, &Length, 1);
      lua_settable(L, metatable);
      
      lua_newtable(L);
      lua_pushstring(L, "__call");
      lua_pushstring(L, name);
      lua_pushcclosure(L, &Create, 1);
      lua_pushstring(L, "new");
      lua_pushvalue(L, -2);
      lua_settable(L, metatable);
      lua_settable(L, -3);
      lua_setmetatable(L, metatable);
      
      lua_pop(L, 2);
    }
    
    static void Push(lua_State* L, const char* name, const T* array, unsigned int size, bool remove = true)
    {
      // If the array is empty or only contains one element
      if (!array || size < 2)
      {
	lua_newtable(L);
	for (unsigned int i = 0; i < size; ++i)
	{
	  lua_pushnumber(L, (int)array[i]);
	  lua_rawseti(L, -2, i + 1);
	}
	lua_pushliteral(L, "n");
	lua_pushinteger(L, (int)size);
	lua_rawset(L, -3);
      }
      else
      {
	luaL_getmetatable(L, name);
	assert(!lua_isnil(L, -1));
	int metatable = lua_gettop(L);
	Subtable(L, metatable, "userdata", "v");
	lua_pushlightuserdata(L, (T*)array);
	lua_gettable(L, -2);
	T** pArray = nullptr;
	if (lua_isnil(L, -1))
	{
	  lua_pop(L, 1);
	  lua_checkstack(L, 3);
	  pArray = (T**)lua_newuserdata(L, sizeof(T*));
	  lua_pushlightuserdata(L, (T*)array);
	  lua_pushinteger(L, (int)size);
	  lua_settable(L, -4);
	}
	if (pArray)
	{
	  *pArray = (T*)array;
	  lua_pushvalue(L, metatable);
	  lua_setmetatable(L, -2);
	  if (!remove)
	  {
	    lua_checkstack(L, 3);
	    Subtable(L, metatable, "no_gc", "k");
	    lua_pushvalue(L, -2);
	    lua_pushboolean(L, 1);
	    lua_settable(L, -3);
	    lua_pop(L, 1);
	  }
	}
	lua_replace(L, metatable);
	lua_settop(L, metatable);
      }
    }
    
  private:
    static int Get(lua_State* L)
    {
      const char* name = lua_tostring(L, lua_upvalueindex(1));
      T** pArray = (T**)luaL_checkudata(L, 1, name);
      if (lua_isnumber(L, 2))
      {
	int index = lua_tointeger(L, 2);
	lua_pushnumber(L, (*pArray)[index - 1]);
      }
      else
	lua_pushnil(L);
      return 1;
    }
    
    static int Set(lua_State* L)
    {
      const char* name = lua_tostring(L, lua_upvalueindex(1));
      T** pArray = (T**)luaL_checkudata(L, 1, name);
      if (lua_isnumber(L, 2) && lua_isnumber(L, 3))
      {
	int index = lua_tointeger(L, 2);
	T value = (T)lua_tonumber(L, 3);
	(*pArray)[index - 1] = value;
      }
      return 0;
    }
    
    static int Create(lua_State* L)
    {
      lua_remove(L, 1);
      int size = lua_tointeger(L, 1);
      T* array = new T[size];
      const char* name = lua_tostring(L, lua_upvalueindex(1));
      Push(L, name, array, (unsigned int)size, true);
      return 1;
    }
    
    static int Remove(lua_State* L)
    {
      if (luaL_getmetafield(L, 1, "no_gc"))
      {
	lua_pushvalue(L, 1);
	lua_gettable(L, -2);
	if (!lua_isnil(L, -1))
	  return 0;
      }
      T** pArray = (T**)lua_touserdata(L, 1);
      if (pArray && *pArray)
      {
	delete [] (*pArray);
	*pArray = nullptr;
	pArray = nullptr;
      }
      return 0;
    }
    
    static int Length(lua_State* L)
    {
      const char* name = lua_tostring(L, lua_upvalueindex(1));
      T** pArray = (T**)luaL_checkudata(L, 1, name);
      luaL_getmetatable(L, name);
      assert(!lua_isnil(L, -1));
      int metatable = lua_gettop(L);
      Subtable(L, metatable, "userdata", "v");
      lua_pushlightuserdata(L, (T*)(*pArray));
      lua_gettable(L, -2);
      lua_insert(L, metatable);
      lua_settop(L, metatable);
      return 1;
    }
    
    static void Subtable(lua_State* L, int table, const char* name, const char* mode)
    {
      lua_pushstring(L, name);
      lua_gettable(L, table);
      if (lua_isnil(L, -1))
      {
	lua_pop(L, 1);
	lua_checkstack(L, 3);
	WeakTable(L, mode);
	lua_pushstring(L, name);
	lua_pushvalue(L, -2);
	lua_settable(L, table);
      }
    }
    
    static void WeakTable(lua_State* L, const char* mode)
    {
      lua_newtable(L);
      lua_pushvalue(L, -1);
      lua_setmetatable(L, -2);
      lua_pushliteral(L, "__mode");
      lua_pushstring(L, mode);
      lua_settable(L, -3);
    }
  };
}

#endif