#ifndef LUACLASSHANDLE_H
#define LUACLASSHANDLE_H
 
#include <Lua/lua.hpp>
#include <ostream>
#include <assert.h>

#define METHOD(class, function) {#function, &class::function}

template <typename T> class LuaClassHandle
{
// Typdefs
public:
  typedef int (T::*FunctionPointer)(lua_State* L);
  typedef struct { const char* name; FunctionPointer function; } Method;
  typedef struct { T* object; } Type;
  
// Functions
public:
  // Register class
  static void Register(lua_State* L, const std::string& className)
  {
    // Create table for class methods
    lua_newtable(L);
    int methods = lua_gettop(L);

    // Create metatable for class events
    luaL_newmetatable(L, T::s_className);
    int events = lua_gettop(L);

    // Access methods through class name in global memory
    // GLOBAL[className] = methods[]
    lua_pushglobaltable(L);
    lua_pushstring(L, T::s_className);
    lua_pushvalue(L, methods);
    lua_settable(L, -3);

    // If trying to access event table, return method table instead
    // events[__metatable] = methods[]
    lua_pushliteral(L, "__metatable");
    lua_pushvalue(L, methods);
    lua_settable(L, events);

    // If key is not present in event table, try key in method table
    // events[__index] = methods[]
    lua_pushliteral(L, "__index");
    lua_pushvalue(L, methods);
    lua_settable(L, events);

    // Add tostring() implementation for class
    // events[__tostring] = toString()
    lua_pushliteral(L, "__tostring");
    lua_pushcfunction(L, ToString);
    lua_settable(L, events);

    // Add garbage collection for class
    // events[__gc] = deleteObject()
    lua_pushliteral(L, "__gc");
    lua_pushcfunction(L, DeleteObject);
    lua_settable(L, events);

    // Create table for class constructor
    // methods[new] = createObject()
    // constructor[__call] = createObject()
    lua_newtable(L);
    lua_pushliteral(L, "__call");
    lua_pushcfunction(L, CreateObject);
    lua_pushliteral(L, "new");
    lua_pushvalue(L, -2);
    lua_settable(L, methods);
    lua_settable(L, -3);
    lua_setmetatable(L, methods);
    
    // Add each method to method table
    for (Method* method = T::methods; method->name; method++)
    {
      // methods[method->name] = CallFunction(method)
      lua_pushstring(L, method->name);
      lua_pushlightuserdata(L, (void*)method);
      lua_pushcclosure(L, CallFunction, 1);
      lua_settable(L, methods);
    }
    
    // Remove method and event table from stack
    lua_pop(L, 2);
  }
  
  static int Push(lua_State* L, T* object, bool gc = false)
  {
    if (!object)
    {
      lua_pushnil(L);
      return 0;
    }
    luaL_getmetatable(L, T::s_className);
    assert(!lua_isnil(L, -1));
    int methods = lua_gettop(L);
    SubTable(L, methods, "userdata", "v");
    void* userData = nullptr;
    lua_pushlightuserdata(L, object);
    lua_gettable(L, -2);
    if (lua_isnil(L, -1))
    {
      lua_pop(L, 1);
      lua_checkstack(L, 3);
      userData = lua_newuserdata(L, sizeof(Type));
      lua_pushlightuserdata(L, object);
      lua_pushvalue(L, -2);
      lua_settable(L, -4);
    }
    Type* type = static_cast<Type*>(userData);
    if (type)
    {
      type->object = object;
      lua_pushvalue(L, methods);
      lua_setmetatable(L, -2);
      if (!gc)
      {
	lua_checkstack(L, 3);
	SubTable(L, methods, "no_gc", "k");
	lua_pushvalue(L, -2);
	lua_pushboolean(L, 1);
	lua_settable(L, -3);
	lua_pop(L, 1);
      }
    }
    lua_replace(L, methods);
    lua_settop(L, methods);
    return methods;
  }
  
  static T* Get(lua_State* L, int narg)
  {
    Type* type = static_cast<Type*>(luaL_checkudata(L, narg, T::className));
    if (!type)
      return nullptr;
    return type->object;
  }
 
private:
  LuaClass() { }
  LuaClass(LuaClass const&);
  void operator=(LuaClass const&);
 
private:
  static int CallFunction(lua_State* L)
  {
    // Get object
    Type* type = static_cast<Type*>(luaL_checkudata(L, 1, T::s_className));
    T* object = type->object;
    // Remove object from stack
    lua_remove(L, 1);
    // Get function
    Method* method = static_cast<Method*>(lua_touserdata(L, lua_upvalueindex(1)));
    // Call function and return number of parameters
    return (object->*(method->function))(L);
  }
  
  static int ToString(lua_State* L)
  {
    // Get object
    Type* type = static_cast<Type*>(lua_touserdata(L, 1));
    T* object = type->object;
    // Create string
    std::ostringstream oss;
    oss << T::s_className << " (" << (void*)object << ")";
    // Push string to stack
    lua_pushstring(L, oss.str().c_str());
    // Return number of parameters
    return 1;
  }

  static int DeleteObject(lua_State* L)
  {
    // Don't delete object if it has "no_gc"-metafield
    if (luaL_getmetafield(L, 1, "no_gc"))
    {
      lua_pushvalue(L, 1);
      lua_gettable(L, -2);
      if (!lua_isnil(L, -1))
	return 0;
    }
    // Get object
    Type* type = static_cast<Type*>(lua_touserdata(L, 1));
    T* object = type->object;
    // Delete object
    if (object)
      delete object;
    return 0;
  }

  static int CreateObject(lua_State* L)
  {
    lua_remove(L, 1);
    T* object = new T(L);
    Push(L, object, true);
    return 1;
  }
  
  static void SubTable(lua_State* L, int table, const char* name, const char* mode)
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
 
#endif