#include "LuaEmbedder.h"

#include <Lua/lua.hpp>

namespace LuaEmbedder
{
  lua_State* g_L = nullptr;
  
  void Init()
  {
    // Make sure you don't initiate twice
    if (g_L != nullptr)
      return;
    
    g_L = luaL_newstate();
  }
}