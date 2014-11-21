#ifndef LUAEMBEDDER_H
#define LUAEMBEDDER_H

#include "LuaClassHandle.h"

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
EXPORT class LuaEmbedder
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
  
  template<typename T> void EmbedClass();
  template<typename T> void AddObject(T* object, const std::string& name);
  template<typename T> int CallMethod(const std::string& method, int parameterCount);
  template<typename T> T* GetParameterObject(int index);
  template<typename T> void SetParameterObject(T* object);
  
  int GetParameterInt(int index);
  
// Variables
private:
  lua_State* m_L;
};

template<typename T>
void LuaEmbedder::EmbedClass()
{
  LuaClassHandle<T>::Register(m_L);
}

template<typename T>
void LuaEmbedder::AddObject(T* object, const std::string& name)
{
  LuaClassHandle<T>::PushObject(m_L, object);
  lua_setglobal(m_L, name.c_str());
}

template<typename T>
int LuaEmbedder::CallMethod(const std::string& method, int parameterCount)
{
  return LuaClassHandle<T>::CallMethod(m_L, method.c_str(), parameterCount);
}


template<typename T>
T* LuaEmbedder::GetParameterObject(int index)
{
  return LuaClassHandle<T>::GetObject(m_L, index);
}

template<typename T>
void LuaEmbedder::SetParameterObject(T* object)
{
  LuaClassHandle<T>::PushObject(m_L, object);
}

#endif