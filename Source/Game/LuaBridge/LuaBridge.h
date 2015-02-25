#ifndef LUABRIDGE_H
#define LUABRIDGE_H

#include "LuaEmbedder/LuaEmbedder.h"

namespace LuaBridge
{

	extern lua_State* g_IOLuaState;
	void SetIOLuaState(lua_State* L);

  void Embed(lua_State* L);
}

#endif