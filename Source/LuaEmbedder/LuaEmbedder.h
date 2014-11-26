#ifndef LUAEMBEDDER_H
#define LUAEMBEDDER_H

#include "Luna.h"

#include <Lua/lua.hpp>
#include <string>

#if defined(WIN32)
  #define EXPORT __declspec(dllexport)
  #define IMPORT __declspec(dllimport)
#else
  #define EXPORT __attribute__((visibility("default")))
  #define IMPORT
#endif

// Bridge between Lua and everything else to make the transition easy
class EXPORT LuaEmbedder
{
// Initialization
public:
  static LuaEmbedder& GetInstance();
private:
  LuaEmbedder();
  LuaEmbedder(LuaEmbedder const&);
  void operator=(LuaEmbedder const&);
  ~LuaEmbedder();

// Functions
public:
  void Run(const std::string& filepath);
  
  template<typename T> void EmbedClass(const char* className);
  template<typename T> void EmbedClassFunction(const char* className, const char* methodName, int (T::*functionPointer)());
  template<typename T> void EmbedClassProperty(const char* className, const char* propertyName, int (T::*getFunctionPointer)(), int (T::*setFunctionPointer)());
  template<typename T> void AddObject(const char* className, T* object, const char* name);
  template<typename T> int CallMethod(const char* className, const char* methodName, T* object, int argumentCount);
  template<typename T> T* GetParameterObject(const char* className, int index);
  template<typename T> void SetParameterObject(const char* className, T* object);
  
  int GetParameterInt(int index);
  void SetParameterInt(int parameter);
  
// Variables
private:
  lua_State* m_L;
};

template<typename T>
void LuaEmbedder::EmbedClass(const char* className)
{
  Luna<T>::Register(m_L, className);
}
template<typename T>
void LuaEmbedder::EmbedClassFunction(const char* className, const char* methodName, int (T::*functionPointer)())
{
  Luna<T>::RegisterMethod(m_L, className, methodName, functionPointer);
}
template<typename T>
void LuaEmbedder::EmbedClassProperty(const char* className, const char* propertyName, int (T::*getFunctionPointer)(), int (T::*setFunctionPointer)())
{
  Luna<T>::RegisterProperty(m_L, className, propertyName, getFunctionPointer, setFunctionPointer);
}
template<typename T>
void LuaEmbedder::AddObject(const char* className, T* object, const char* name)
{
  Luna<T>::push(m_L, className, object);
  lua_setglobal(m_L, name);
}
template<typename T>
int LuaEmbedder::CallMethod(const char* className, const char* methodName, T* object, int argumentCount)
{
  Luna<T>::push(m_L, className, object);
  lua_insert(m_L, -(argumentCount + 1));
  return Luna<T>::CallMethod(m_L, className, methodName, argumentCount);
}
template<typename T>
T* LuaEmbedder::GetParameterObject(const char* className, int index)
{
  return Luna<T>::check(m_L, className, index);
}
template<typename T>
void LuaEmbedder::SetParameterObject(const char* className, T* object)
{
  Luna<T>::push(m_L, className, object);
}

#endif