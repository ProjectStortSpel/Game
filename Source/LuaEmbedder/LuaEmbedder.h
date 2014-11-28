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
  void EXPORT RunFunction(const std::string& functionName, int argumentCount = 0);
  
  void EXPORT EmbedDouble(const std::string& name, double value, const std::string& library = std::string());
  void EXPORT EmbedInt(const std::string& name, int value, const std::string& library = std::string());
  void EXPORT EmbedUnsignedInt(const std::string& name, unsigned int value, const std::string& library = std::string());
  void EXPORT EmbedBool(const std::string& name, bool value, const std::string& library = std::string());
  void EXPORT EmbedString(const std::string& name, const char* value, const std::string& library = std::string());
  void EXPORT EmbedFunction(const std::string& name, int (*functionPointer)(), const std::string& library = std::string());
  
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
  void EXPORT AddObject(const std::string& className, T* object, const std::string& name)
  {
    Luna<T>::push(L, className.c_str(), object);
    lua_setglobal(L, name.c_str());
  }
  template<typename T>
  int EXPORT CallMethod(const std::string& className, const std::string& methodName, T* object, int argumentCount)
  {
    Luna<T>::push(L, className.c_str(), object);
    lua_insert(L, -(1 + argumentCount));
    return Luna<T>::CallMethod(L, className.c_str(), methodName.c_str(), argumentCount);
  }
  template<typename T>
  T EXPORT* PullObject(const std::string& className, int index)
  {
    return Luna<T>::check(L, className.c_str(), index);
  }
  template<typename T>
  void EXPORT PushObject(const std::string& className, T* object)
  {
    Luna<T>::push(L, className.c_str(), object);
  }
}

#endif