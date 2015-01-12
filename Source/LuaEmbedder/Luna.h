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
#include <string.h> // For strlen
#include <vector>
#include <assert.h>
#include <string>
#include <iostream>
#include <map>
#include <algorithm>

namespace LuaEmbedder
{
  template <class T>
  struct PropertyType
  {
    const char* name;
    int (T::*getter)();
    int (T::*setter)();
  };

  template <class T>
  struct FunctionType
  {
    const char* name;
    int (T::*func)();
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
      m_properties.clear();
      m_methods.clear();
      m_objectFunctionsMap.clear();
      
      luaL_newmetatable(L, className);
      int metatable = lua_gettop(L);
      
      lua_pushglobaltable(L);
      lua_pushstring(L, className);
      lua_pushvalue(L, metatable);
      lua_settable(L, -3);
      
      if (gc)
      {
	lua_pushstring(L, "__gc");
	lua_pushcfunction(L, &Luna<T>::gc_obj);
	lua_settable(L, metatable);
      }
      
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
    
    static void RegisterProperty(lua_State* L, const char* className, const char* propertyName, int (T::*getter)(), int (T::*setter)())
    {
      int propertyIndex = (int)m_properties.size();
      PropertyType<T> property = { propertyName, getter, setter };
      m_properties.push_back(property);
      
      luaL_getmetatable(L, className);
      int metatable = lua_gettop(L);
      
      lua_pushstring(L, propertyName);
      lua_pushnumber(L, propertyIndex);
      lua_settable(L, metatable);
      
      lua_pop(L, 1);
    }
    
    static void RegisterMethod(lua_State* L, const char* className, const char* methodName, int (T::*func)())
    {
      int methodIndex = (int)m_methods.size();
      FunctionType<T> method = { methodName, func };
      m_methods.push_back(method);
      
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
      T* ap = new T();
      const char* className = lua_tostring(L, lua_upvalueindex(1));
      push(L, className, ap, true);
      return 1;
    }
    
    static int Clone(lua_State* L)
    {
      lua_remove(L, 1);
      T* ap = new T();
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
	std::cerr << "Luna::CallMethod : Object of class " << className << " undefined in Lua" << std::endl;
	lua_pop(L, argumentCount + 1);
	return -1;
      }
      lua_pushstring(L, methodName);
      lua_gettable(L, base);
      if (lua_isnil(L, -1))
      {
	std::cerr << "Luna::CallMethod : Method " << methodName << " of class " << className << " undefined" << std::endl;
	lua_pop(L, argumentCount + 2);
	return -1;
      }
      lua_insert(L, base);
      int status = lua_pcall(L, 1 + argumentCount, LUA_MULTRET, 0);
      lua_gc(L, LUA_GCCOLLECT, 0);
      if (status != 0)
      {
	std::cerr << "Luna::CallMethod : " << (lua_isstring(L, -1) ? lua_tostring(L, -1) : "Unknown error") << std::endl;
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
	
	return ((*obj)->*(m_properties[_index].getter))();
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
	if (lua_isnil(L, -1))
	  lua_pushvalue(L, index);
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
	  
	  return ((*obj)->*(m_properties[_index].setter))();
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
      
      return ((*obj)->*(m_methods[i].func))();
    }

    /*
      @ gc_obj (internal)
      Arguments:
	* L - Lua State
    */
    static int gc_obj(lua_State* L)
    {
      if (luaL_getmetafield(L, 1, "no_gc"))
      {
	lua_pushvalue(L, 1);
	lua_gettable(L, -2);
	if (!lua_isnil(L, -1))
	  return 0;
      }
      
      T** obj = static_cast<T**>(lua_touserdata(L, 1));
      
      if(obj && *obj)
      {
	delete(*obj);
	*obj = nullptr;
	obj = nullptr;
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
  };

  template <class T> std::vector<PropertyType<T>> Luna<T>::m_properties = std::vector<PropertyType<T>>();
  template <class T> std::vector<FunctionType<T>> Luna<T>::m_methods = std::vector<FunctionType<T>>();
  template <class T> std::map<T*, std::vector<std::string>> Luna<T>::m_objectFunctionsMap = std::map<T*, std::vector<std::string>>();
}

#endif