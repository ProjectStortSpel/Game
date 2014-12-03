#include "LuaWorld.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "LuaComponent.h"
#include "LuaSystem.h"

namespace LuaBridge
{
  LuaWorld::LuaWorld() { }
  
  void LuaWorld::Embed()
  {
    LuaEmbedder::EmbedClass<LuaWorld>("World");     
  }
  
  int LuaWorld::Update()
  {
    float dt = (float)LuaEmbedder::PullFloat(1);
    m_world->Update(dt);
    return 0;
  }

  int LuaWorld::CreateComponentAndAddTo()
  {
    
  }

  int LuaWorld::RemoveComponentFrom()
  {

  }

  int LuaWorld::KillEntity()
  {

  }
}