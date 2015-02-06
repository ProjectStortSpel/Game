/*
 * Summary:
 * 	Class embedding.
 * 
 * Function list:
 * 	Register
 * 		Embed class in Lua.
 * 	RegisterMethod
 * 		Embed class function in Lua.
 * 	RegisterProperty
 * 		Embed class variable in Lua.
 * 	push
 * 		Push given object onto the Lua stack.
 * 	CallMethod
 * 		Call class method in Lua.
 * 	CallMethods
 * 		Call all objects with given method in Lua.
 * 	check/lightcheck
 * 		Get object from given argument of class method call in Lua.
 * 
 * Created by:
 * 	Marcus Svensson
 * 
 * Other:
 * 	This is a modified version of LunaFive and Lunar, available at
 * 	http://lua-users.org/wiki/LunaFive and
 * 	http://lua-users.org/wiki/CppBindingWithLunar respectively.
 */

#ifndef LUNA_H
#define LUNA_H

#include <Lua/lua.hpp>
#include <SDL/SDL.h>
#include <string.h> // For strlen
#include <vector>
#include <assert.h>
#include <string>
#include <iostream>
#include <map>
#include <algorithm>
#include <string>

namespace LuaEmbedder
{
  template <class T>
  struct PropertyType
  {
    std::string name;
    int (T::*getter)(lua_State*);
    int (T::*setter)(lua_State*);
  };

  template <class T>
  struct FunctionType
  {
    std::string name;
    int (T::*func)(lua_State*);
  };

  template <class T> class Luna
  {
  private:
    static std::vector<PropertyType<T>> m_properties;
    static std::vector<FunctionType<T>> m_methods;
    static std::map<T*, std::vector<std::string>> m_objectFunctionsMap;
    
  public:
    /*
      @ check
      Arguments:
	* L - Lua State
	* narg - Position to check

      Description:
	Retrieves a wrapped class from the arguments passed to the func, specified by narg (position).
	This func will raise an exception if the argument is not of the correct type.
    */
    static T* check(lua_State* L, const char* className, int narg)
    {
      T** obj = static_cast<T**>(luaL_checkudata(L, narg, className));
      if (!obj)
	return nullptr; // lightcheck returns nullptr if not found.
      return *obj;		// pointer to T object
    }

    /*
      @ lightcheck
      Arguments:
	* L - Lua State
	* narg - Position to check

      Description:
	Retrieves a wrapped class from the arguments passed to the func, specified by narg (position).
	This func will return nullptr if the argument is not of the correct type.  Useful for supporting
	multiple types of arguments passed to the func
    */ 
    static T* lightcheck(lua_State* L, const char* className, int narg)
    {
      T** obj = static_cast<T**>(luaL_testudata(L, narg, className));
      if (!obj)
	return nullptr; // lightcheck returns nullptr if not found.
      return *obj;		// pointer to T object
    }

    /*
      @ Register
      Arguments:
	* L - Lua State
	* namespac - Namespace to load into

      Description:
	Registers your class with Lua.  Leave namespac "" if you want to load it into the global space.
    */
    // REGISTER CLASS AS A GLOBAL TABLE 
    static void Register(lua_State* L, const char* className, bool gc = true)
    {
      luaL_newmetatable(L, className);
      int metatable = lua_gettop(L);
      
      lua_pushglobaltable(L);
      lua_pushstring(L, className);
      lua_pushvalue(L, metatable);
      lua_settable(L, -3);
      
	  lua_pushstring(L, "__gc");
	  lua_pushboolean(L, (int)gc);
	  lua_pushcclosure(L, &Luna<T>::gc_obj, 1);
	  lua_settable(L, metatable);
      
      lua_pushstring(L, "__tostring");
      lua_pushstring(L, className);
      lua_pushcclosure(L, &Luna<T>::to_string, 1);
      lua_settable(L, metatable);

      lua_pushstring(L, "__eq");		// To be able to compare two Luna objects (not natively possible with full userdata)
      lua_pushcfunction(L, &Luna<T>::equals);
      lua_settable(L, metatable);

      lua_pushstring(L, "__index");
      lua_pushstring(L, className);
      lua_pushcclosure(L, &Luna<T>::property_getter, 1);
      lua_settable(L, metatable);

      lua_pushstring(L, "__newindex");
      lua_pushstring(L, className);
      lua_pushcclosure(L, &Luna<T>::property_setter, 1);
      lua_settable(L, metatable);
      
      lua_pushstring(L, "clone");
      lua_pushstring(L, className);
      lua_pushcclosure(L, &Luna<T>::Clone, 1);
      lua_settable(L, metatable);
      
      lua_newtable(L);
      lua_pushstring(L, "__call");
      lua_pushstring(L, className);
      lua_pushcclosure(L, &Luna<T>::constructor, 1);
      lua_pushstring(L, "new");
      lua_pushvalue(L, -2);
      lua_settable(L, metatable);
      lua_settable(L, -3);
      lua_setmetatable(L, metatable);
      
      lua_pop(L, 2);
    }
    
    static void RegisterProperty(lua_State* L, const char* className, const char* propertyName, int (T::*getter)(lua_State*), int (T::*setter)(lua_State*))
    {
	  int propertyIndex = -1;
	  for (int i = 0; i < (int)m_properties.size(); i++)
	  {
		  if (m_properties[i].name == std::string(propertyName))
		  {
			  propertyIndex = i;
			  break;
		  }
	  }

	  if (propertyIndex < 0)
	  {
		  propertyIndex = (int)m_properties.size();
		  PropertyType<T> property = { std::string(propertyName), getter, setter };
		  m_properties.push_back(property);
	  }
      
      luaL_getmetatable(L, className);
      int metatable = lua_gettop(L);
      
      lua_pushstring(L, propertyName);
      lua_pushnumber(L, propertyIndex);
      lua_settable(L, metatable);
      
      lua_pop(L, 1);
    }
    
    static void RegisterMethod(lua_State* L, const char* className, const char* methodName, int (T::*func)(lua_State*))
    {
	  int methodIndex = -1;
	  for (int i = 0; i < (int)m_methods.size(); i++)
	  {
		if (m_methods[i].name == std::string(methodName))
		{
		  methodIndex = i;
		  break;
		}
	  }
      
	  if (methodIndex < 0)
	  {
		methodIndex = (int)m_methods.size();
		FunctionType<T> method = { std::string(methodName), func };
		m_methods.push_back(method);
	  }
      
      luaL_getmetatable(L, className);
      int metatable = lua_gettop(L);
      
      lua_pushstring(L, methodName);
      lua_pushnumber(L, methodIndex | ( 1 << 8 ));
      lua_settable(L, metatable);
      
      lua_pop(L, 1);
    }

    /*
      @ constructor (internal)
      Arguments:
	* L - Lua State
    */
    static int constructor(lua_State* L)
    {
      lua_remove(L, 1);
      T* ap = new T(L);
      const char* className = lua_tostring(L, lua_upvalueindex(1));
      push(L, className, ap, true);
      return 1;
    }
    
    static int Clone(lua_State* L)
    {
      lua_remove(L, 1);
      T* ap = new T(L);
      const char* className = lua_tostring(L, lua_upvalueindex(1));
      push(L, className, ap, true);
      int object = lua_gettop(L);
      
      luaL_getmetatable(L, className);
      int metatable = lua_gettop(L);
      lua_pushstring(L, "object_members");
      lua_gettable(L, -2);
      if (lua_isnil(L, -1))
      {
	lua_pushvalue(L, object);
	lua_settop(L, -1);
	return 1;
      }
      int objects = lua_gettop(L);
      T** obj = static_cast<T**>(lua_touserdata(L, 1));
      lua_pushlightuserdata(L, *obj);
      lua_gettable(L, -2);
      if (lua_isnil(L, -1))
      {
	lua_pushvalue(L, object);
	lua_settop(L, -1);
	return 1;
      }
      lua_newtable(L);
      int newMembers = lua_gettop(L);
      lua_pushvalue(L, -1);
      lua_setmetatable(L, -2);
      lua_pushlightuserdata(L, ap);
      lua_pushvalue(L, -2);
      lua_settable(L, objects);
      lua_pushvalue(L, -2);
      int members = lua_gettop(L);
      lua_pushnil(L);
      while (lua_next(L, members) != 0)
      {
	lua_pushvalue(L, -2);
	lua_pushvalue(L, -2);
	lua_settable(L, newMembers);
	lua_pop(L, 1);
      }
      lua_pushvalue(L, object);
      lua_settop(L, -1);
      return 1;
    }

    /*
      @ createNew
      Arguments:
	* L - Lua State
	    T*	- Instance to push

      Description:
	Loads an instance of the class into the Lua stack, and provides you a pointer so you can modify it.
    */
    static int push(lua_State* L, const char* className, T* instance, bool gc = false)
    {
      if (!instance)
      {
	lua_pushnil(L);
	return 0;
      }
      
      luaL_getmetatable(L, className);
      assert(!lua_isnil(L, -1));
      int metatable = lua_gettop(L);
      Subtable(L, metatable, "userdata", "v");
      lua_pushlightuserdata(L, instance);
      lua_gettable(L, -2);
      T** a = nullptr;
      if (lua_isnil(L, -1))
      {
	lua_pop(L, 1);
	lua_checkstack(L, 3);
	a = (T**)lua_newuserdata(L, sizeof(T*)); // Create userdata
	lua_pushlightuserdata(L, instance);
	lua_pushvalue(L, -2);
	lua_settable(L, -4);
      }
      if (a)
      {
	*a = instance;
	lua_pushvalue(L, metatable);
	lua_setmetatable(L, -2);
	if (!gc)
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
      return metatable;
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
    
    static int CallMethod(lua_State* L, const char* className, const char* methodName, int argumentCount = 0)
    {
      int base = lua_gettop(L) - argumentCount;
      if (!luaL_checkudata(L, base, className))
      {
	SDL_Log("Luna::CallMethod : Object of class %s undefined in Lua", className);
	lua_pop(L, argumentCount + 1);
	return -1;
      }
      lua_pushstring(L, methodName);
      lua_gettable(L, base);
      if (lua_isnil(L, -1))
      {
	SDL_Log("Luna::CallMethod : Method %s of class %s undefined", methodName, className);
	lua_pop(L, argumentCount + 2);
	return -1;
      }
      lua_insert(L, base);
      int status = lua_pcall(L, 1 + argumentCount, LUA_MULTRET, 0);
      if (status != 0)
      {
	SDL_Log("Luna::CallMethod : %s", (lua_isstring(L, -1) ? lua_tostring(L, -1) : "Unknown error"));
	lua_pop(L, 1);
	return -1;
      }
      return lua_gettop(L) - base + 1;
    }
    
    static void CallMethods(lua_State* L, const char* className, const char* methodName, int argumentCount = 0)
    {
      int arguments = lua_gettop(L);
      luaL_getmetatable(L, className);
      assert(!lua_isnil(L, -1));
      int metatable = lua_gettop(L);
      lua_pushstring(L, "userdata");
      lua_gettable(L, metatable);
      int userdata = lua_gettop(L);
      if (!lua_istable(L, userdata))
	return;
      lua_pushnil(L);
      while (lua_next(L, userdata) != 0)
      {
	if (lua_isuserdata(L, -1))
	{
	  T** obj = static_cast<T**>(lua_touserdata(L, -1));
	  if (HasFunction(*obj, std::string(methodName)))
	  {
	    for (int i = arguments - argumentCount + 1; i <= arguments; i++)
	      lua_pushvalue(L, i);
	    CallMethod(L, className, methodName, argumentCount);
	  }
	  else
	    lua_pop(L, 1);
	}
	else
	  lua_pop(L, 1);
      }
    }

    /*
      @ property_getter (internal)
      Arguments:
	* L - Lua State
    */
    static int property_getter(lua_State* L)
    {
      lua_getmetatable(L, 1); // Look up the index of a name
      lua_pushvalue(L, 2);	// Push the name
      lua_rawget(L, -2);		// Get the index
      int index = lua_gettop(L);
      
      if (lua_isnumber(L, -1))   // Check if we got a valid index
      {    
	int _index = (int)lua_tonumber(L, -1);
	
	T** obj = static_cast<T**>(lua_touserdata(L, 1));
	
	lua_pushvalue(L, 3);
	
	if(_index & (1 << 8)) // A func
	{
	  lua_pushnumber(L, _index ^ (1 << 8)); // Push the right func index
	  lua_pushlightuserdata(L, obj);
	  lua_pushcclosure(L, &Luna<T>::function_dispatch, 2);
	  return 1; // Return a func
	}
	
	lua_pop(L, 2);    // Pop metatable and _index
	lua_remove(L, 1); // Remove userdata
	lua_remove(L, 1); // Remove [key]
	
	return ((*obj)->*(m_properties[_index].getter))(L);
      }
      else
      {
	const char* className = lua_tostring(L, lua_upvalueindex(1));
	luaL_getmetatable(L, className);
	int metatable = lua_gettop(L);
	lua_pushstring(L, "object_members");
	lua_gettable(L, -2);
	if (lua_isnil(L, -1))
	{
	  lua_pushvalue(L, index);
	  lua_settop(L, -1);
	  return 1;
	}
	int objects = lua_gettop(L);
	T** obj = static_cast<T**>(lua_touserdata(L, 1));
	lua_pushlightuserdata(L, *obj);
	lua_gettable(L, -2);
	if (lua_isnil(L, -1))
	{
	  lua_pushvalue(L, index);
	  lua_settop(L, -1);
	  return 1;
	}
	int members = lua_gettop(L);
	lua_pushvalue(L, 2);
	lua_gettable(L, members);
	lua_settop(L, -1);
      }
      
      return 1;
    }

    /*
      @ property_setter (internal)
      Arguments:
	* L - Lua State
    */
    static int property_setter(lua_State* L)
    {	
      lua_getmetatable(L, 1); // Look up the index from name
      lua_pushvalue(L, 2);	//
      lua_rawget(L, -2);		//
      
      if (lua_isnumber(L, -1)) // Check if we got a valid index
      {   
	int _index = (int)lua_tonumber(L, -1);
	
	T** obj = static_cast<T**>(lua_touserdata(L, 1));
	
	if(!obj || !*obj)
	{
	  luaL_error(L, "Internal error, no object given!");
	  return 0;
	}
	
	if(_index >> 8) // Try to set a func
	{
	  return 1;
	}
	else
	{
	  lua_pop(L, 2);    // Pop metatable and _index
	  lua_remove(L, 1); // Remove userdata
	  lua_remove(L, 1); // Remove [key]
	  
	  return ((*obj)->*(m_properties[_index].setter))(L);
	}
      }
      else
      {
	const char* className = lua_tostring(L, lua_upvalueindex(1));
	luaL_getmetatable(L, className);
	int metatable = lua_gettop(L);
	lua_pushstring(L, "object_members");
	lua_gettable(L, -2);
	if (lua_isnil(L, -1))
	{
	  lua_newtable(L);
	  lua_pushvalue(L, -1);
	  lua_setmetatable(L, -2);
	  lua_pushstring(L, "object_members");
	  lua_pushvalue(L, -2);
	  lua_settable(L, metatable);
	}
	int objects = lua_gettop(L);
	T** obj = static_cast<T**>(lua_touserdata(L, 1));
	lua_pushlightuserdata(L, *obj);
	lua_gettable(L, -2);
	if (lua_isnil(L, -1))
	{
	  lua_newtable(L);
	  lua_pushvalue(L, -1);
	  lua_setmetatable(L, -2);
	  lua_pushlightuserdata(L, *obj);
	  lua_pushvalue(L, -2);
	  lua_settable(L, objects);
	}
	int members = lua_gettop(L);
	lua_pushvalue(L, 2);
	lua_pushvalue(L, 3);
	lua_settable(L, members);
	if (lua_isfunction(L, 3))
	  m_objectFunctionsMap[*obj].push_back(lua_tostring(L, 2));
      }
      lua_settop(L, 0);
      return 0;
    }
    
    static bool HasFunction(T* object, const std::string& functionName)
    {
      typename std::map<T*, std::vector<std::string>>::iterator objectFunctionsIt = m_objectFunctionsMap.find(object);
      if (objectFunctionsIt != m_objectFunctionsMap.end() &&
	  std::find(objectFunctionsIt->second.begin(), objectFunctionsIt->second.end(), functionName) != objectFunctionsIt->second.end())
	  return true;
      return false;
    }

    /*
      @ function_dispatch (internal)
      Arguments:
	* L - Lua State
    */
    static int function_dispatch(lua_State* L)
    {
      int i = (int)lua_tonumber(L, lua_upvalueindex(1));
      T** obj = static_cast<T**>(lua_touserdata(L, lua_upvalueindex(2)));
      
	  // Small fix for now
	  if (lua_gettop(L) > 0)
		lua_remove(L, 1);
      
      return ((*obj)->*(m_methods[i].func))(L);
    }

    /*
      @ gc_obj (internal)
      Arguments:
	* L - Lua State
    */
    static int gc_obj(lua_State* L)
    {
		T** obj = static_cast<T**>(lua_touserdata(L, 1));

		bool gc = (bool)lua_toboolean(L, lua_upvalueindex(1));

		bool no_gc = false;
		if (luaL_getmetafield(L, 1, "no_gc"))
		{
			lua_pushvalue(L, 1);
			lua_gettable(L, -2);
			if (!lua_isnil(L, -1))
				no_gc = true;
		}

		if (!gc || no_gc)
		{
			*obj = NULL;
			obj = NULL;
			return 0;
		}
      
      if(obj && *obj)
      {
		delete(*obj);
		*obj = NULL;
		obj = NULL;
      }
      
      return 0;
    }
	  
    static int to_string(lua_State* L)
    {
      T** obj = static_cast<T**>(lua_touserdata(L, -1));
      
      const char* className = lua_tostring(L, lua_upvalueindex(1));
      
      if (obj)
	lua_pushfstring(L, "%s (%p)", className, (void*)*obj);
      else
	lua_pushstring(L, "Empty object");
      
      return 1;
    }

    /*
      * Method which compares two Luna objects.
      * The full userdatas (as opposed to light userdata) can't be natively compared one to other, we have to had this to do it.
      */
    static int equals(lua_State* L)
    {
      T** obj1 = static_cast<T**>(lua_touserdata(L, -1));
      T** obj2 = static_cast<T**>(lua_touserdata(L, 1));

      lua_pushboolean(L, *obj1 == *obj2);
	  
      return 1;
    }

	static int WriteFunction(lua_State* L, const char* p, size_t sz, luaL_Buffer* ud)
	{
		luaL_addlstring(ud, p, sz);
		return 0;
	}

	static bool DumpFunction(lua_State* L, luaL_Buffer* buffer)
	{
#if defined(__OSX__) || defined(__IOS__)
        int chunkSetting = 0;
#else
        int chunkSetting = 1;
#endif
        
		int error = lua_dump(L, (lua_Writer)WriteFunction, buffer, chunkSetting);
		luaL_pushresult(buffer);
		if (error != 0)
		{
			SDL_Log("Luna::DumpFunction : Error");
			return false;
		}
		return true;
	}

	static bool LoadFunction(lua_State* L, luaL_Buffer* buffer)
	{
		int error = luaL_loadbuffer(L, buffer->b, buffer->size, NULL);
		if (error != 0)
		{
			SDL_Log("Luna::LoadFunction : Error");
			return false;
		}
		return true;
	}

	static void CopyTable(lua_State* A, lua_State* B, int tableA, int tableB)
	{
		lua_pushnil(A);
		while (lua_next(A, tableA) != 0)
		{
			// Key
			if (lua_isboolean(A, -2))
			{
				bool value = lua_toboolean(A, -2);
				lua_pushboolean(B, value);
			}
			else if (lua_iscfunction(A, -2))
			{
				lua_CFunction value = lua_tocfunction(A, -2);
				lua_pushcfunction(B, value);
			}
			else if (lua_isfunction(A, -2))
			{
				lua_pushvalue(A, -2);
				luaL_Buffer buffer;
				int top = lua_gettop(A);
				luaL_buffinit(A, &buffer);
				DumpFunction(A, &buffer);
				lua_settop(A, top);
				lua_pop(A, 1);
				LoadFunction(B, &buffer);
			}
			else if (lua_isinteger(A, -2))
			{
				int value = lua_tointeger(A, -2);
				lua_pushinteger(B, value);
			}
			else if (lua_islightuserdata(A, -2))
			{
				void* value = lua_touserdata(A, -2);
				lua_pushlightuserdata(B, value);
			}
			else if (lua_isnil(A, -2))
			{
				lua_pushnil(B);
			}
			else if (lua_isnumber(A, -2))
			{
				lua_Number value = lua_tonumber(A, -2);
				lua_pushnumber(B, value);
			}
			else if (lua_isstring(A, -2))
			{
				const char* value = lua_tostring(A, -2);
				lua_pushstring(B, value);
			}
			else if (lua_istable(A, -2))
			{
				int a = lua_gettop(A) - 1;
				lua_pushstring(A, "__mode");
				lua_pushstring(A, "k");
				lua_settable(A, a);
				
				lua_newtable(B);
				int b = lua_gettop(B);
				lua_pushstring(B, "__mode");
				lua_pushstring(B, "k");
				lua_settable(B, b);
				
				CopyTable(A, B, a, b);
			}
			else if (lua_isuserdata(A, -2))
			{
				SDL_Log("Luna::CopyTable : Possible unsafe use of userdata");
			}

			// Value
			if (lua_isboolean(A, -1))
			{
				bool value = lua_toboolean(A, -1);
				lua_pushboolean(B, value);
			}
			else if (lua_iscfunction(A, -1))
			{
				lua_CFunction value = lua_tocfunction(A, -1);
				lua_pushcfunction(B, value);
			}
			else if (lua_isfunction(A, -1))
			{
				lua_pushvalue(A, -1);
				luaL_Buffer buffer;
				int top = lua_gettop(A);
				luaL_buffinit(A, &buffer);
				DumpFunction(A, &buffer);
				lua_settop(A, top);
				lua_pop(A, 1);
				LoadFunction(B, &buffer);
			}
			else if (lua_isinteger(A, -1))
			{
				int value = lua_tointeger(A, -1);
				lua_pushinteger(B, value);
			}
			else if (lua_islightuserdata(A, -1))
			{
				void* value = lua_touserdata(A, -1);
				lua_pushlightuserdata(B, value);
			}
			else if (lua_isnil(A, -1))
			{
				lua_pushnil(B);
			}
			else if (lua_isnumber(A, -1))
			{
				lua_Number value = lua_tonumber(A, -1);
				lua_pushnumber(B, value);
			}
			else if (lua_isstring(A, -1))
			{
				const char* value = lua_tostring(A, -1);
				lua_pushstring(B, value);
			}
			else if (lua_istable(A, -1))
			{
				int a = lua_gettop(A);
				lua_pushstring(A, "__mode");
				lua_pushstring(A, "k");
				lua_settable(A, a);
				
				lua_newtable(B);
				int b = lua_gettop(B);
				lua_pushstring(B, "__mode");
				lua_pushstring(B, "k");
				lua_settable(B, b);
				
				CopyTable(A, B, a, b);
			}
			else if (lua_isuserdata(A, -1))
			{
				SDL_Log("Luna::CopyTable : Possible unsafe use of userdata");
			}

			// Set key/value in table B
			lua_settable(B, tableB);

			lua_pop(A, 1);
		}
	}
    
    static void Copy(lua_State* A, lua_State* B, const char* className, T* instance)
    {
      push(B, className, instance);
      
      luaL_getmetatable(A, className);
      int metatableA = lua_gettop(A);
      lua_pushstring(A, "object_members");
      lua_gettable(A, -2);
      if (lua_isnil(A, -1))
      {
	SDL_Log("No table 'object_members' in class %s", className);
	return;
      }
      int objectsA = lua_gettop(A);
      lua_pushlightuserdata(A, instance);
      lua_gettable(A, -2);
      if (lua_isnil(A, -1))
      {
	SDL_Log("No table '%p' in table 'object_members' in class %s", className);
	return;
      }
      int membersA = lua_gettop(A);
      
      luaL_getmetatable(B, className);
      int metatableB = lua_gettop(B);
      lua_pushstring(B, "object_members");
      lua_gettable(B, -2);
      if (lua_isnil(B, -1))
      {
	lua_newtable(B);
	lua_pushvalue(B, -1);
	lua_setmetatable(B, -2);
	lua_pushstring(B, "object_members");
	lua_pushvalue(B, -2);
	lua_settable(B, metatableB);
      }
      int objectsB = lua_gettop(B);
      lua_pushlightuserdata(B, instance);
      lua_gettable(B, -2);
      if (lua_isnil(B, -1))
      {
	lua_newtable(B);
	lua_pushvalue(B, -1);
	lua_setmetatable(B, -2);
	lua_pushlightuserdata(B, instance);
	lua_pushvalue(B, -2);
	lua_settable(B, objectsB);
      }
      else
	SDL_Log("Warning! There's already another Lua copy of this class %s", className);
      int membersB = lua_gettop(B);
      
	  CopyTable(A, B, membersA, membersB);
      
      lua_settop(A, 0);
      lua_settop(B, 0);
    }

	static void NilTable(lua_State* L, int table)
	{
		lua_pushnil(L);
		while (lua_next(L, table) != 0)
		{
			if (lua_istable(L, -1))
				NilTable(L, lua_gettop(L));

			lua_pushvalue(L, -2);
			lua_pushnil(L);
			lua_settable(L, table);

			lua_pop(L, 1);
		}
	}

	static void Clear(lua_State* L, const char* className, T* instance)
	{
		if (m_objectFunctionsMap.find(instance) != m_objectFunctionsMap.end())
			m_objectFunctionsMap.erase(instance);

		luaL_getmetatable(L, className);
		int metatable = lua_gettop(L);
		lua_pushstring(L, "object_members");
		lua_gettable(L, metatable);
		if (!lua_isnil(L, -1))
		{
			int objects = lua_gettop(L);
			lua_pushlightuserdata(L, instance);
			lua_gettable(L, -2);
			if (!lua_isnil(L, -1))
			{
				// Nil table recursivly
				int members = lua_gettop(L);
				NilTable(L, members);

				lua_pushlightuserdata(L, instance);
				lua_pushnil(L);
				lua_settable(L, objects);
			}
		}
		lua_settop(L, 0);
	}
  };

  template <class T> std::vector<PropertyType<T>> Luna<T>::m_properties = std::vector<PropertyType<T>>();
  template <class T> std::vector<FunctionType<T>> Luna<T>::m_methods = std::vector<FunctionType<T>>();
  template <class T> std::map<T*, std::vector<std::string>> Luna<T>::m_objectFunctionsMap = std::map<T*, std::vector<std::string>>();
}

#endif