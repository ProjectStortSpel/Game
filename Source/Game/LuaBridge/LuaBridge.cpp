#include "LuaBridge.h"
#include "Input/LuaInput.h"
#include "ECSL/LuaWorldCreator.h"
#include "ECSL/LuaWorld.h"
#include "ECSL/LuaSystem.h"
#include "ECSL/LuaComponentType.h"
#include "ECSL/LuaComponent.h"
#include "Renderer/LuaGraphicDevice.h"
#include "Renderer/LuaCamera.h"
#include "Math/LuaMatrix.h"
#include "FileLoader/LuaFileLoader.h"

namespace LuaBridge
{
  void Embed()
  {
    LuaInput::Embed();
    
    LuaWorldCreator::Embed();
    LuaWorld::Embed();
    LuaSystem::Embed();
    LuaComponentType::Embed();
    LuaComponent::Embed();
    
    LuaGraphicDevice::Embed();
    LuaCamera::Embed();
    
    LuaMatrix::Embed();

	LuaEmbedder::AddFunction("LoadMap", &LoadMap, "File");
  }
}