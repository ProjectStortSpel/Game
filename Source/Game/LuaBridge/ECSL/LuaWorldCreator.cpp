#include "LuaWorldCreator.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "LuaComponent.h"
#include "LuaSystem.h"

namespace LuaBridge
{
  LuaWorldCreator::LuaWorldCreator() { }
  
  void LuaWorldCreator::Embed()
  {
    LuaEmbedder::EmbedClass<LuaWorldCreator>("WorldCreator");
    LuaEmbedder::EmbedClassFunction<LuaWorldCreator>("WorldCreator", "AddComponentType", &LuaWorldCreator::AddComponentType);
    LuaEmbedder::EmbedClassFunction<LuaWorldCreator>("WorldCreator", "AddSystemToCurrentGroup", &LuaWorldCreator::AddSystemToCurrentGroup);      
  }
  
  int LuaWorldCreator::AddComponentType()
  {
    if (LuaEmbedder::IsString(1))
    {
      WorldCreator::AddComponentType(LuaEmbedder::PullString(1));
    }
    else
    {
      LuaComponent* luaComponent = LuaEmbedder::PullObject<LuaComponent>("Component", 1);
      if (luaComponent)
      {
	WorldCreator::AddComponentType(*luaComponent->CreateComponentType());
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
    //LuaEmbedder::PushObject<>();
    return 1;
  }

  int LuaWorldCreator::IsWorldInitialized()
  {
    LuaEmbedder::PushBool(WorldCreator::IsWorldInitialized());
    return 1;
  }
}