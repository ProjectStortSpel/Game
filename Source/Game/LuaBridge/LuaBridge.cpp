#include "LuaBridge.h"
#include "Input/LuaInput.h"
#include "ECSL/LuaWorldCreator.h"
#include "ECSL/LuaSystem.h"
#include "ECSL/LuaComponent.h"

namespace LuaBridge
{
  void Init()
  {
    LuaEmbedder::Init();
    
    LuaInput::Embed();
    
    LuaWorldCreator::Embed();
    LuaSystem::Embed();
    LuaComponent::Embed();
    
    LuaEmbedder::Load("test.lua");
  }
  
  void Quit()
  {
    LuaEmbedder::Quit();
  }
}