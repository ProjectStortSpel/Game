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

namespace LuaBridge
{
  void Embed(lua_State* L)
  {
    LuaInput::Embed(L);
    
    LuaWorldCreator::Embed(L);
    LuaWorld::Embed(L);
    LuaSystem::Embed(L);
    LuaComponentType::Embed(L);
    LuaComponent::Embed(L);
    LuaEntityTemplate::Embed(L);
    LuaEntityTemplateManager::Embed(L);
    
    LuaGraphicDevice::Embed(L);
    LuaCamera::Embed(L);
    
    LuaMatrix::Embed(L);
    
    LuaConsole::Embed(L);

    LuaNetwork::Embed(L);
    //LuaClientNetwork::Embed();
    //LuaServerNetwork::Embed();

    LuaEmbedder::AddFunction(L, "LoadMap", &LoadMap, "File");
  }
}