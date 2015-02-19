#ifndef LUARESOURCE_H
#define LUARESOURCE_H

#include <LuaEmbedder/LuaEmbedder.h>

namespace LuaBridge
{
	namespace LuaResource
	{
		void Embed(lua_State* L);
		void SetServerState(lua_State* L);
	}
}


#endif