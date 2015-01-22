#ifndef LUANETWORK_H
#define LUANETWORK_H

#include <LuaEmbedder/LuaEmbedder.h>

namespace LuaBridge
{
	namespace LuaNetwork
	{
		void Embed(lua_State* L);
	}

	namespace LuaClientNetwork
	{
		void Embed(lua_State* L);
	}

	namespace LuaServerNetwork
	{
		void Embed(lua_State* L);
	}
}

#endif