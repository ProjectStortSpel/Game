#ifndef LUAWORLD_H
#define LUAWORLD_H

#include "ECSL/Framework/World.h"

namespace LuaBridge
{
  class LuaWorld
  {
  public:
    LuaWorld();
    
    static void Embed();
    
  private:
    int Update();
    int CreateComponentAndAddTo();
    int RemoveComponentFrom();
    int KillEntity();
    
  private:
    ECSL::World* m_world;
  };
}

#endif