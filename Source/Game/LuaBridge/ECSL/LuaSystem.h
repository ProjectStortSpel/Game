#ifndef LUASYSTEM_H
#define LUASYSTEM_H

#include "ECSL/Interfaces/System.h"

#include <string>
#include <map>

namespace LuaBridge
{
  class LuaSystem : public ECSL::System
  {
  public:
    LuaSystem();
    ~LuaSystem() { }
    
    static void Embed();
    
    void Run(float _dt);
    void Initialize();
    void OnEntityAdded(unsigned int _entityId);
    void OnEntityRemoved(unsigned int _entityId);
    
  private:
    int AddComponentTypeToFilter();
  };
}

#endif