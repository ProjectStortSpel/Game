#ifndef LUACONSOLE_H
#define LUACONSOLE_H

#include <LuaEmbedder/LuaEmbedder.h>

namespace LuaBridge
{
  namespace LuaConsole
  {
    void Embed(lua_State* L);
  }
}

#endif