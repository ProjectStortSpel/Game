#include "LuaBridge.h"
#include "Input/LuaInput.h"
#include "ECSL/LuaWorldCreator.h"
#include "ECSL/LuaWorld.h"
#include "ECSL/LuaSystem.h"
#include "ECSL/LuaComponentType.h"
#include "ECSL/LuaComponent.h"
#include "ECSL/LuaEntityTemplate.h"
#include "ECSL/LuaEntityTemplateManager.h"
#include "Renderer/LuaGraphicDevice.h"
#include "Renderer/LuaCamera.h"
#include "Math/LuaMatrix.h"
#include "FileLoader/LuaFileLoader.h"
#include "Console/LuaConsole.h"
#include "Network/LuaNetwork.h"
#include "File/LuaFile.h"

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
    LuaEntityTemplate::Embed();
    LuaEntityTemplateManager::Embed();
    
    LuaGraphicDevice::Embed();
    LuaCamera::Embed();
    
    LuaMatrix::Embed();
    
    LuaConsole::Embed();

	LuaNetwork::Embed();

	LuaFile::Embed();

	//LuaClientNetwork::Embed();
	//LuaServerNetwork::Embed();

	LuaEmbedder::AddFunction("LoadMap", &LoadMap, "File");
  }
}