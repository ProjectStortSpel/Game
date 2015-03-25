#include "LuaWorldCreator.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "LuaComponentType.h"
#include "LuaWorld.h"

namespace LuaBridge
{
    LuaWorldCreator::LuaWorldCreator() : WorldCreator(), m_skipComponentTypes(false) { }

    LuaWorldCreator::LuaWorldCreator(lua_State* L) : m_skipComponentTypes(false) { }
  
  void LuaWorldCreator::Embed(lua_State* L)
  {
    LuaEmbedder::EmbedClass<LuaWorldCreator>(L, "WorldCreator", false);
    LuaEmbedder::EmbedClassFunction<LuaWorldCreator>(L, "WorldCreator", "AddComponentType", &LuaWorldCreator::AddComponentType);
    LuaEmbedder::EmbedClassFunction<LuaWorldCreator>(L, "WorldCreator", "AddSystemToCurrentGroup", &LuaWorldCreator::AddSystemToCurrentGroup);      
    LuaEmbedder::EmbedClassFunction<LuaWorldCreator>(L, "WorldCreator", "AddSystemGroup", &LuaWorldCreator::AddSystemGroup);
    LuaEmbedder::EmbedClassFunction<LuaWorldCreator>(L, "WorldCreator", "CreateWorld", &LuaWorldCreator::CreateWorld);
    LuaEmbedder::EmbedClassFunction<LuaWorldCreator>(L, "WorldCreator", "IsWorldInitialized", &LuaWorldCreator::IsWorldInitialized);
	LuaEmbedder::EmbedClassFunction<LuaWorldCreator>(L, "WorldCreator", "SetMaxNumberOfEntities", &LuaWorldCreator::SetMaxNumberOfEntities);
  }
  
  int LuaWorldCreator::AddComponentType(lua_State* L)
  {
      if (!m_skipComponentTypes)
      {
          if (LuaEmbedder::IsString(L, 1))
          {
              WorldCreator::AddComponentType(LuaEmbedder::PullString(L, 1));
          }
          else
          {
              LuaComponentType* componentType = LuaEmbedder::PullObject<LuaComponentType>(L, "ComponentType", 1);
              if (componentType)
              {
                  WorldCreator::AddComponentType(*componentType->CreateComponentType());
              }
          }

      }
      return 0;
  }

  int LuaWorldCreator::AddSystemToCurrentGroup(lua_State* L)
  {
    LuaSystem* system = LuaEmbedder::PullObject<LuaSystem>(L, "System", 1);
    WorldCreator::AddLuaSystemToCurrentGroup(system);
    return 0;
  }
  
  int LuaWorldCreator::AddSystemGroup(lua_State* L)
  {
    WorldCreator::AddSystemGroup();
    return 0;
  }

  int LuaWorldCreator::CreateWorld(lua_State* L)
  {
    unsigned int entityCount = (unsigned int)LuaEmbedder::PullInt(L, 1);
    ECSL::World* world = WorldCreator::CreateWorld(entityCount);
    LuaEmbedder::PushObject<ECSL::World>(L, "World", world, true);
    return 1;
  }

  int LuaWorldCreator::IsWorldInitialized(lua_State* L)
  {
    LuaEmbedder::PushBool(L, WorldCreator::IsWorldInitialized());
    return 1;
  }

  int LuaWorldCreator::SetMaxNumberOfEntities(lua_State* L)
  {
	  WorldCreator::SetMaxNumberOfEntities(LuaEmbedder::PullInt(L, 1));
	  return 0;
  }
}