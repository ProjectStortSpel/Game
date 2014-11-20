#ifndef LUAEMBEDDER_H
#define LUAEMBEDDER_H

#if defined(WIN32)
  #define EXPORT __declspec(dllexport)
  #define IMPORT __declspec(dllimport)
#else
  #define EXPORT __attribute__((visibility("default")))
  #define IMPORT
#endif

namespace LuaEmbedder
{
  EXPORT void Init();
}

#endif