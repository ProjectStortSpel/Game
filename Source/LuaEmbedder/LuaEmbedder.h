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
#include <map>
#include <iostream>

#if defined(WIN32)
  #define EXPORT __declspec(dllexport)
  #define IMPORT __declspec(dllimport)

#ifdef _DEBUG
#include <VLD/vld.h>
#endif
#else
  #define EXPORT __attribute__((visibility("default")))
  #define IMPORT
#endif

namespace LuaEmbedder
{
  extern std::map<lua_State*, std::vector<lua_State*>> LuaStates;
  extern std::map<lua_State*, lua_State*> LuaThreads;
  
  EXPORT lua_State* CreateState();
  EXPORT lua_State* CopyState(lua_State* L);
  void EXPORT Quit();
  
  bool EXPORT Load(lua_State* L, const std::string& filepath);
  bool EXPORT CallFunction(lua_State* L, const std::string& name, int argumentCount = 0, const std::string& library = std::string());
  
  void EXPORT CollectGarbage(lua_State* L);
  void EXPORT CollectGarbage(lua_State* L, int durationInMilliseconds);
  int EXPORT GetMemoryUsage();
  void EXPORT CollectGarbage();
  
  void EXPORT AddFloat(lua_State* L, const std::string& name, float value, const std::string& library = std::string());
  void EXPORT AddInt(lua_State* L, const std::string& name, int value, const std::string& library = std::string());
  void EXPORT AddBool(lua_State* L, const std::string& name, bool value, const std::string& library = std::string());
  void EXPORT AddString(lua_State* L, const std::string& name, const char* value, const std::string& library = std::string());
  void EXPORT AddFunction(lua_State* L, const std::string& name, int (*functionPointer)(lua_State*), const std::string& library = std::string());
  void EXPORT AddFloat(const std::string& name, float value, const std::string& library = std::string());
  void EXPORT AddInt(const std::string& name, int value, const std::string& library = std::string());
  void EXPORT AddBool(const std::string& name, bool value, const std::string& library = std::string());
  void EXPORT AddString(const std::string& name, const char* value, const std::string& library = std::string());
  void EXPORT AddFunction(const std::string& name, int (*functionPointer)(lua_State*), const std::string& library = std::string());
  
  float EXPORT PullFloat(lua_State* L, int index);
  float EXPORT PullFloat(lua_State* L, const std::string& name, const std::string& library = std::string());
  int EXPORT PullInt(lua_State* L, int index);
  int EXPORT PullInt(lua_State* L, const std::string& name, const std::string& library = std::string());
  bool EXPORT PullBool(lua_State* L, int index);
  bool EXPORT PullBool(lua_State* L, const std::string& name, const std::string& library = std::string());
  std::string EXPORT PullString(lua_State* L, int index);
  std::string EXPORT PullString(lua_State* L, const std::string& name, const std::string& library = std::string());
  
  void EXPORT PushFloat(lua_State* L, float value);
  void EXPORT PushInt(lua_State* L, int value);
  void EXPORT PushBool(lua_State* L, bool value);
  void EXPORT PushString(lua_State* L, const std::string& value);
  void EXPORT PushNull(lua_State* L);
  void EXPORT PushFloatArray(lua_State* L, const float* values, unsigned int size, bool remove = true);
  void EXPORT PushIntArray(lua_State* L, const int* values, unsigned int size, bool remove = true);
  void EXPORT PushUnsignedIntArray(lua_State* L, const unsigned int* values, unsigned int size, bool remove = true);
  void EXPORT PushBoolArray(lua_State* L, const bool* values, unsigned int size, bool remove = true);
  void EXPORT PushStringArray(lua_State* L, const std::string* values, unsigned int size, bool remove = true);
  
  bool EXPORT IsFloat(lua_State* L, int index);
  bool EXPORT IsInt(lua_State* L, int index);
  bool EXPORT IsBool(lua_State* L, int index);
  bool EXPORT IsString(lua_State* L, int index);
  bool EXPORT IsFunction(lua_State* L, int index);
  
  void EXPORT SaveFunction(lua_State* L, int index, const std::string& key);
  bool EXPORT CallSavedFunction(lua_State* L, const std::string& key, int argumentCount = 0);
  
  template<typename T>
  void EXPORT EmbedClass(lua_State* L, const std::string& className, bool gc = true)
  {
    Luna<T>::Register(L, className.c_str(), gc);
  }
  template<typename T>
  void EXPORT EmbedClassFunction(lua_State* L, const std::string& className, const std::string& methodName, int (T::*functionPointer)(lua_State*))
  {
    Luna<T>::RegisterMethod(L, className.c_str(), methodName.c_str(), functionPointer);
  }
  template<typename T>
  void EXPORT EmbedClassProperty(lua_State* L, const std::string& className, const std::string& propertyName, int (T::*getFunctionPointer)(lua_State*), int (T::*setFunctionPointer)(lua_State*))
  {
    Luna<T>::RegisterProperty(L, className.c_str(), propertyName.c_str(), getFunctionPointer, setFunctionPointer);
  }
  #define ADD_OBJECT(luaState) \
    if (library.empty()) \
    { \
      Luna<T>::push(L, className.c_str(), object); \
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
      lua_pushstring(L, name.c_str()); \
      Luna<T>::push(L, className.c_str(), object); \
      lua_settable(L, -3); \
      lua_setglobal(L, library.c_str()); \
    }
  template<typename T>
  void EXPORT AddObject(lua_State* L, const std::string& className, T* object, const std::string& name, const std::string& library = std::string())
  {
    std::map<lua_State*, lua_State*>::iterator it0 = LuaThreads.find(L);
    std::map<lua_State*, std::vector<lua_State*>>::iterator it1;
    if (it0 != LuaThreads.end())
      it1 = LuaStates.find(it0->second);
    else
    {
      it1 = LuaStates.find(L);
      if (it1 == LuaStates.end())
      {
	ADD_OBJECT(L);
	return;
      }
    }
    assert(it1 != LuaStates.end());
    for (std::vector<lua_State*>::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
      ADD_OBJECT((*it2));
    ADD_OBJECT(it1->first);
  }
  template<typename T>
  int EXPORT CallMethod(lua_State* L, const std::string& className, const std::string& methodName, T* object, int argumentCount = 0)
  {
    Luna<T>::push(L, className.c_str(), object);
    lua_insert(L, -(1 + argumentCount));
    return Luna<T>::CallMethod(L, className.c_str(), methodName.c_str(), argumentCount);
  }
  template<typename T>
  void EXPORT CallMethods(lua_State* L, const std::string& className, const std::string& methodName, int argumentCount = 0)
  {
    Luna<T>::CallMethods(L, className.c_str(), methodName.c_str(), argumentCount);
  }
  template<typename T>
  T EXPORT* PullObject(lua_State* L, const std::string& className, int index)
  {
    return Luna<T>::check(L, className.c_str(), index);
  }
  template<typename T>
  T EXPORT* PullObject(lua_State* L, const std::string& className, const std::string& name, const std::string& library = std::string())
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
  void EXPORT PushObject(lua_State* L, const std::string& className, T* object, bool gc = false)
  {
    Luna<T>::push(L, className.c_str(), object, gc);
  }
  template<typename T>
  bool EXPORT HasFunction(lua_State* L, T* object, const std::string& functionName)
  {
    return Luna<T>::HasFunction(object, functionName);
  }
}

#endif