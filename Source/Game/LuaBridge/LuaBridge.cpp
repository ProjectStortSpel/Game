#include "LuaBridge.h"
#include "Input/LuaInput.h"
#include "ECSL/LuaWorldCreator.h"
#include "ECSL/LuaWorld.h"
#include "ECSL/LuaSystem.h"
#include "ECSL/LuaComponentType.h"

namespace LuaBridge
{
  void Embed()
  {
    LuaInput::Embed();
    
    LuaWorldCreator::Embed();
    LuaWorld::Embed();
    LuaSystem::Embed();
    LuaComponentType::Embed();
  }
}