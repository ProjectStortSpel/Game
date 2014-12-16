#ifndef LUAWORLD_H
#define LUAWORLD_H

#include "ECSL/Framework/World.h"

namespace LuaBridge
{
  class LuaWorld : public ECSL::World
  {
  public:
    LuaWorld();
    ~LuaWorld();
    
    static void Embed();
    
  private:
    int Update();
    int CreateNewEntity();
    int CreateComponentAndAddTo();
    int RemoveComponentFrom();
    int KillEntity();
	int SetComponent();
  };
}

#endif