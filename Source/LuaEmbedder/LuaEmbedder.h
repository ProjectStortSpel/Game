/*
 * Summary:
 * 	General Lua embedding.
 * 
 * Function list:
 * 	Init
 * 		Initiate Lua.
 * 	Quit
 * 		Close Lua.
 * 	Load
 * 		Load Lua script from file.
 * 	CallFunction
 * 		Call global function. Push arguments in order. First argument first etc.
 * 	AddDouble
 * 		Add global double to Lua instance.
 * 	AddInt
 * 		Add global int to Lua instance.
 * 	AddUnsignedInt
 * 		Add global unsigned int to Lua instance.
 * 	AddBool
 * 		Add global bool to Lua instance.
 * 	AddString
 * 		Add global string to Lua instance.
 * 	AddFunction
 * 		Add global function to Lua instance.
 * 	PullDouble
 * 		Pull double from Lua stack.
 * 	PullInt
 * 		Pull int from Lua stack.
 * 	PullUnsignedInt
 * 		Pull unsigned int from Lua stack.
 * 	PullBool
 * 		Pull bool from Lua stack.
 * 	PullString
 * 		Pull string from Lua stack.
 * 	PushDouble
 * 		Push double to Lua stack.
 * 	PushInt
 * 		Push int to Lua stack.
 * 	PushUnsigned
 * 		Push unsigned int to Lua stack.
 * 	PushBool
 * 		Push bool to Lua stack.
 * 	PushString
 * 		Push string to Lua stack.
 * 	EmbedClass
 * 		Embed class in Lua.
 * 	EmbedClassFunction
 * 		Embed class function in Lua.
 * 	EmbedClassProperty
 * 		Embed class property in Lua.
 * 	AddObject
 * 		Add global object to Lua instance.
 * 	CallMethod
 * 		Call class method in Lua.
 * 	CallMethods
 * 		Call all objects with given method in Lua.
 * 	PullObject
 * 		Pull object from Lua stack.
 * 	PushObject
 * 		Push object to Lua stack.
 * 
 * Created by:
 * 	Marcus Svensson
 */

#ifndef LUAEMBEDDER_H
#define LUAEMBEDDER_H

#include "Luna.h"

#include <Lua/lua.hpp>
#include <assert.h>
#include <string>
#include <vector>

#if defined(WIN32)
  #define EXPORT __declspec(dllexport)
  #define IMPORT __declspec(dllimport)
#else
  #define EXPORT __attribute__((visibility("default")))
  #define IMPORT
#endif

namespace LuaEmbedder
{
  extern lua_State IMPORT* L;
  
  void EXPORT Init();
  void EXPORT Quit();
  
  void EXPORT Load(const std::string& filepath);
  void EXPORT CallFunction(const std::string& name, int argumentCount = 0, const std::string& library = std::string());
  
  void EXPORT AddDouble(const std::string& name, double value, const std::string& library = std::string());
  void EXPORT AddInt(const std::string& name, int value, const std::string& library = std::string());
  void EXPORT AddUnsignedInt(const std::string& name, unsigned int value, const std::string& library = std::string());
  void EXPORT AddBool(const std::string& name, bool value, const std::string& library = std::string());
  void EXPORT AddString(const std::string& name, const char* value, const std::string& library = std::string());
  void EXPORT AddFunction(const std::string& name, int (*functionPointer)(), const std::string& library = std::string());
  
  double EXPORT PullDouble(int index);
  double EXPORT PullDouble(const std::string& name, const std::string& library = std::string());
  int EXPORT PullInt(int index);
  int EXPORT PullInt(const std::string& name, const std::string& library = std::string());
  unsigned int EXPORT PullUnsignedInt(int index);
  unsigned int EXPORT PullUnsignedInt(const std::string& name, const std::string& library = std::string());
  bool EXPORT PullBool(int index);
  bool EXPORT PullBool(const std::string& name, const std::string& library = std::string());
  std::string EXPORT PullString(int index);
  std::string EXPORT PullString(const std::string& name, const std::string& library = std::string());
  
  void EXPORT PushDouble(double value);
  void EXPORT PushInt(int value);
  void EXPORT PushUnsignedInt(unsigned int value);
  void EXPORT PushBool(bool value);
  void EXPORT PushString(const std::string& value);
  
  template<typename T>
  void EXPORT EmbedClass(const std::string& className)
  {
    Luna<T>::Register(L, className.c_str());
  }
  template<typename T>
  void EXPORT EmbedClassFunction(const std::string& className, const std::string& methodName, int (T::*functionPointer)())
  {
    Luna<T>::RegisterMethod(L, className.c_str(), methodName.c_str(), functionPointer);
  }
  template<typename T>
  void EXPORT EmbedClassProperty(const std::string& className, const std::string& propertyName, int (T::*getFunctionPointer)(), int (T::*setFunctionPointer)())
  {
    Luna<T>::RegisterProperty(L, className.c_str(), propertyName.c_str(), getFunctionPointer, setFunctionPointer);
  }
  template<typename T>
  void EXPORT AddObject(const std::string& className, T* object, const std::string& name, const std::string& library = std::string())
  {
    if (library.empty())
    {
      Luna<T>::push(L, className.c_str(), object);
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
      lua_pushstring(L, name.c_str());
      Luna<T>::push(L, className.c_str(), object);
      lua_settable(L, -3);
      lua_setglobal(L, library.c_str());
    }
  }
  template<typename T>
  int EXPORT CallMethod(const std::string& className, const std::string& methodName, T* object, int argumentCount = 0)
  {
    Luna<T>::push(L, className.c_str(), object);
    lua_insert(L, -(1 + argumentCount));
    return Luna<T>::CallMethod(L, className.c_str(), methodName.c_str(), argumentCount);
  }
  template<typename T>
  void EXPORT CallMethods(const std::string& className, const std::string& methodName, int argumentCount = 0)
  {
    Luna<T>::CallMethods(L, className.c_str(), methodName.c_str(), argumentCount);
  }
  template<typename T>
  T EXPORT* PullObject(const std::string& className, int index)
  {
    return Luna<T>::check(L, className.c_str(), index);
  }
  template<typename T>
  T EXPORT* PullObject(const std::string& className, const std::string& name, const std::string& library = std::string())
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
    return Luna<T>::check(L, className.c_str(), -1);
  }
  template<typename T>
  void EXPORT PushObject(const std::string& className, T* object)
  {
    Luna<T>::push(L, className.c_str(), object);
  }
}

#endif