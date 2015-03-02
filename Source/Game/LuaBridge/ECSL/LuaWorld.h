#ifndef LUAWORLD_H
#define LUAWORLD_H

#include "ECSL/Framework/World.h"
#include <LuaEmbedder/LuaEmbedder.h>

namespace LuaBridge
{
  class LuaWorld : public ECSL::World
  {
  public:
    LuaWorld(lua_State* L);
    ~LuaWorld();
    
    static void Embed(lua_State* L);
    
  private:
    int Update(lua_State* L);
    int CreateNewEntity(lua_State* L);
    int CreateComponentAndAddTo(lua_State* L);
    int RemoveComponentFrom(lua_State* L);
    int KillEntity(lua_State* L);
    int SetComponent(lua_State* L);
    int GetComponent(lua_State* L);
	int GetComponents(lua_State* L);
    int HasComponent(lua_State* L);
  };
}

#endif