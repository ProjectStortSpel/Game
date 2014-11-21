#ifndef LUACLASSHANDLE_H
#define LUACLASSHANDLE_H
  
#include <Lua/lua.hpp>
#include <sstream>
#include <assert.h>
 
#define METHOD(class, function) {#function, &class::function}

// Used to register classes, add objects and call their methods
template <typename T> class LuaClassHandle
{
// Typdefs
public:
  typedef int (T::*FunctionPointer)();
  typedef struct { const char* name; FunctionPointer function; } Method;
private:
  typedef struct { T* object; } Type;
   
// Functions
public:
  // Register class
  static void Register(lua_State* L)
  {
    // Create table for class methods
    lua_newtable(L);
    int methods = lua_gettop(L);
 
    // Create metatable for class events
    luaL_newmetatable(L, T::className);
    int events = lua_gettop(L);
 
    // Access methods through class name in global memory
    // GLOBAL[className] = methods[]
    lua_pushglobaltable(L);
    lua_pushstring(L, T::className);
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
   
  static int PushObject(lua_State* L, T* object, bool gc = false)
  {
    if (!object)
    {
      lua_pushnil(L);
      return 0;
    }
    luaL_getmetatable(L, T::className);
    assert(!lua_isnil(L, -1));
    int methods = lua_gettop(L);
    Subtable(L, methods, "userdata", "v");
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
	Subtable(L, methods, "no_gc", "k");
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
   
  static T* GetObject(lua_State* L, int parameterIndex)
  {
    Type* type = static_cast<Type*>(luaL_checkudata(L, parameterIndex, T::className));
    if (!type)
      return nullptr;
    return type->object;
  }
  
  static int CallMethod(lua_State* L, const char* method, int parameterCount = 0)
  {
    // Get userdata
    int base = lua_gettop(L) - parameterCount;
    if (!luaL_checkudata(L, base, T::className))
    {
      lua_settop(L, base - 1);
      lua_pushfstring(L, "Invalid %s userdata", T::className);
      return -1;
    }
    // Get method
    lua_pushstring(L, method);
    lua_gettable(L, base);
    if (lua_isnil(L, -1))
    {
      lua_settop(L, base - 1);
      lua_pushfstring(L, "%s has no method '%s'", T::className, method);
      return -1;
    }
    lua_insert(L, base);
    // Call method
    int status = lua_pcall(L, 1 + parameterCount, LUA_MULTRET, 0);
    if (status != 0)
    {
      const char* errorMessage = lua_tostring(L, -1);
      if (!errorMessage)
	errorMessage = "Unknown error";
      lua_pushfstring(L, "%s::%s : %s", T::className, method, errorMessage);
      lua_remove(L, base);
      return -1;
    }
    // Return number of results
    return lua_gettop(L) - base + 1;
  }
  
private:
  static int CallFunction(lua_State* L)
  {
    // Get object
    Type* type = static_cast<Type*>(luaL_checkudata(L, 1, T::className));
    T* object = type->object;
    // Remove object from stack
    lua_remove(L, 1);
    // Get function
    Method* method = static_cast<Method*>(lua_touserdata(L, lua_upvalueindex(1)));
    // Call function and return number of parameters
    return (object->*(method->function))();
  }
   
  static int ToString(lua_State* L)
  {
    // Get object
    Type* type = static_cast<Type*>(lua_touserdata(L, 1));
    T* object = type->object;
    // Create string
    std::ostringstream oss;
    oss << T::className << " (" << (void*)object << ")";
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
    T* object = new T();
    PushObject(L, object, true);
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
  
private:
  LuaClassHandle() { }
  LuaClassHandle(LuaClassHandle const&);
  void operator=(LuaClassHandle const&);
};

#endif