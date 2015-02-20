#ifndef LUAFILE_H
#define LUAFILE_H

#include <LuaEmbedder/LuaEmbedder.h>

int LoadMap();


namespace LuaBridge
{
	namespace LuaFile
	{
		void Embed(lua_State* L);
	}
}


#endif