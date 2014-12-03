#include "LuaWorldCreator.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "LuaComponentType.h"
#include "LuaSystem.h"
#include "LuaWorld.h"

namespace LuaBridge
{
  LuaWorldCreator::LuaWorldCreator() { }
  
  void LuaWorldCreator::Embed()
  {
    LuaEmbedder::EmbedClass<LuaWorldCreator>("WorldCreator");
    LuaEmbedder::EmbedClassFunction<LuaWorldCreator>("WorldCreator", "AddComponentType", &LuaWorldCreator::AddComponentType);
    LuaEmbedder::EmbedClassFunction<LuaWorldCreator>("WorldCreator", "AddSystemToCurrentGroup", &LuaWorldCreator::AddSystemToCurrentGroup);      
    LuaEmbedder::EmbedClassFunction<LuaWorldCreator>("WorldCreator", "AddSystemGroup", &LuaWorldCreator::AddSystemGroup);
    LuaEmbedder::EmbedClassFunction<LuaWorldCreator>("WorldCreator", "CreateWorld", &LuaWorldCreator::CreateWorld);
    LuaEmbedder::EmbedClassFunction<LuaWorldCreator>("WorldCreator", "IsWorldInitialized", &LuaWorldCreator::IsWorldInitialized);
  }
  
  int LuaWorldCreator::AddComponentType()
  {
    if (LuaEmbedder::IsString(1))
    {
      WorldCreator::AddComponentType(LuaEmbedder::PullString(1));
    }
    else
    {
      LuaComponentType* componentType = LuaEmbedder::PullObject<LuaComponentType>("ComponentType", 1);
      if (componentType)
      {
	WorldCreator::AddComponentType(*componentType->CreateComponentType());
      }
    }
    return 0;
  }

  int LuaWorldCreator::AddSystemToCurrentGroup()
  {
    WorldCreator::AddLuaSystemToCurrentGroup(LuaEmbedder::PullObject<LuaSystem>("System", 1));
    return 0;
  }
  
  int LuaWorldCreator::AddSystemGroup()
  {
    WorldCreator::AddSystemGroup();
    return 0;
  }

  int LuaWorldCreator::CreateWorld()
  {
    unsigned int entityCount = (unsigned int)LuaEmbedder::PullInt(1);
    ECSL::World* world = WorldCreator::CreateWorld(entityCount);
    LuaEmbedder::PushObject<ECSL::World>("World", world, true);
    return 1;
  }

  int LuaWorldCreator::IsWorldInitialized()
  {
    LuaEmbedder::PushBool(WorldCreator::IsWorldInitialized());
    return 1;
  }
}