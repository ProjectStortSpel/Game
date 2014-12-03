#include "LuaWorld.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "LuaSystem.h"

namespace LuaBridge
{
  LuaWorld::LuaWorld()
    : ECSL::World(0, nullptr, nullptr) { }
  
  LuaWorld::~LuaWorld() { }
  
  void LuaWorld::Embed()
  {
    LuaEmbedder::EmbedClass<LuaWorld>("World");
    LuaEmbedder::EmbedClassFunction<LuaWorld>("World", "Update", &LuaWorld::Update);
    LuaEmbedder::EmbedClassFunction<LuaWorld>("World", "CreateNewEntity", &LuaWorld::CreateNewEntity);
    LuaEmbedder::EmbedClassFunction<LuaWorld>("World", "CreateComponentAndAddTo", &LuaWorld::CreateComponentAndAddTo);
    LuaEmbedder::EmbedClassFunction<LuaWorld>("World", "RemoveComponentFrom", &LuaWorld::RemoveComponentFrom);
    LuaEmbedder::EmbedClassFunction<LuaWorld>("World", "KillEntity", &LuaWorld::KillEntity);
  }
  
  int LuaWorld::Update()
  {
    float dt = LuaEmbedder::PullFloat(1);
    World::Update(dt);
    return 0;
  }
  
  int LuaWorld::CreateNewEntity()
  {
    LuaEmbedder::PushInt((int)World::CreateNewEntity());
    return 1;
  }

  int LuaWorld::CreateComponentAndAddTo()
  {
    std::string componentType = LuaEmbedder::PullString(1);
    unsigned int entityId = (unsigned int)LuaEmbedder::PullInt(2);
    World::CreateComponentAndAddTo(componentType, entityId);
    return 0;
  }

  int LuaWorld::RemoveComponentFrom()
  {
    std::string componentType = LuaEmbedder::PullString(1);
    unsigned int entityId = (unsigned int)LuaEmbedder::PullInt(2);
    World::RemoveComponentFrom(componentType, entityId);
    return 0;
  }

  int LuaWorld::KillEntity()
  {
    unsigned int entityId = (unsigned int)LuaEmbedder::PullInt(1);
    World::KillEntity(entityId);
    return 0;
  }
}