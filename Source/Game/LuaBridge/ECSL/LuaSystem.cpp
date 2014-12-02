#include "LuaSystem.h"
#include "LuaEmbedder/LuaEmbedder.h"

namespace LuaBridge
{
  LuaSystem::LuaSystem() { }
  
  void LuaSystem::Embed()
  {
    LuaEmbedder::EmbedClass<LuaSystem>("System", false);
  }
  
  void LuaSystem::Run(float _dt)
  {
    LuaEmbedder::PushDouble((double)_dt);
    LuaEmbedder::CallMethod<LuaSystem>("System", "Run", this, 1);
  }

  void LuaSystem::Initialize()
  {
    LuaEmbedder::CallMethod<LuaSystem>("System", "Initialize", this);
  }

  void LuaSystem::OnEntityAdded(unsigned int _entityId)
  {
    LuaEmbedder::PushUnsignedInt(_entityId);
    LuaEmbedder::CallMethod<LuaSystem>("System", "OnEntityAdded", this, 1);
  }

  void LuaSystem::OnEntityRemoved(unsigned int _entityId)
  {
    LuaEmbedder::PushUnsignedInt(_entityId);
    LuaEmbedder::CallMethod<LuaSystem>("System", "OnEntityRemoved", this, 1);
  }
}