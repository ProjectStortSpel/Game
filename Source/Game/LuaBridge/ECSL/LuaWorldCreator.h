#ifndef LUAWORLDCREATOR_H
#define LUAWORLDCREATOR_H

#include "ECSL/Framework/WorldCreator.h"

namespace LuaBridge
{
  class LuaWorldCreator : public ECSL::WorldCreator
  {
  public:
    LuaWorldCreator();
    
    static void Embed();
    
  private:
    int AddComponentType();
    int AddSystemToCurrentGroup();
    int AddSystemGroup();
    int CreateWorld();
    int IsWorldInitialized();
  };
}

#endif