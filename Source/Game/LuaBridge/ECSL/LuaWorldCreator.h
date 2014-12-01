#ifndef LUAWORLDCREATOR_H
#define LUAWORLDCREATOR_H

#include "ECSL/Framework/WorldCreator.h"

#include <string>
#include <map>

namespace LuaBridge
{
  class LuaWorldCreator : public ECSL::WorldCreator
  {
  public:
    LuaWorldCreator();
    
    static void Embed();
    
    int AddComponentType();
    int AddSystemGroup();
  };
}

#endif