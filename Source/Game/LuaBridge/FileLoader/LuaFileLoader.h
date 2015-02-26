#ifndef LUAFILELOADER_H
#define LUAFILELOADER_H

#include <LuaEmbedder/LuaEmbedder.h>

int LoadMap(lua_State* L);
int GenerateIslandMesh(lua_State* L);

#endif