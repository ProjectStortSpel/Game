#include "LuaSystem.h"
#include "LuaEmbedder/LuaEmbedder.h"

#include <cstdio>
#include <stdio.h>

namespace LuaBridge
{
  LuaSystem::LuaSystem() { }
  
  void LuaSystem::Embed()
  {
    LuaEmbedder::EmbedClass<LuaSystem>("System", false);
    LuaEmbedder::EmbedClassFunction<LuaSystem>("System", "AddComponentTypeToFilter", &LuaSystem::AddComponentTypeToFilter);
    
    LuaEmbedder::AddInt("Mandatory", (int)ECSL::FilterType::Mandatory, "FilterType");
    LuaEmbedder::AddInt("RequiresOneOf", (int)ECSL::FilterType::RequiresOneOf, "FilterType");
    LuaEmbedder::AddInt("Excluded", (int)ECSL::FilterType::Excluded, "FilterType");
  }
  
  void LuaSystem::Update(float _dt)
  {
    LuaEmbedder::PushFloat(_dt);
    LuaEmbedder::CallMethod<LuaSystem>("System", "Update", this, 1);
  }

  void LuaSystem::Initialize()
  {
    LuaEmbedder::CallMethod<LuaSystem>("System", "Initialize", this);
  }

  void LuaSystem::OnEntityAdded(unsigned int _entityId)
  {
    LuaEmbedder::PushInt((int)_entityId);
    LuaEmbedder::CallMethod<LuaSystem>("System", "OnEntityAdded", this, 1);
  }

  void LuaSystem::OnEntityRemoved(unsigned int _entityId)
  {
    LuaEmbedder::PushInt((int)_entityId);
    LuaEmbedder::CallMethod<LuaSystem>("System", "OnEntityRemoved", this, 1);
  }
  
  int LuaSystem::AddComponentTypeToFilter()
  {
    std::string componentType = LuaEmbedder::PullString(1);
    ECSL::FilterType filterType = (ECSL::FilterType)LuaEmbedder::PullInt(2);
    System::AddComponentTypeToFilter(componentType, filterType);
    return 0;
  }
}