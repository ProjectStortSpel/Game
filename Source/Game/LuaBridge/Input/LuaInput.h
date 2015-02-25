#ifndef LUAINPUT_H
#define LUAINPUT_H

#include "Input/InputWrapper.h"
#include <LuaEmbedder/LuaEmbedder.h>

namespace LuaBridge
{
  namespace LuaInput
  {
    void Embed(lua_State* L);
  }
}

#endif