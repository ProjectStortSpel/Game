#include "LuaBridge.h"
#include "Input/LuaInput.h"
#include "ECSL/LuaWorldCreator.h"
#include "ECSL/LuaWorld.h"
#include "ECSL/LuaSystem.h"
#include "ECSL/LuaComponentType.h"
#include "ECSL/LuaComponent.h"
#include "ECSL/LuaEntityTemplate.h"
#include "ECSL/LuaEntityTemplateManager.h"
#include "AI/LuaPotentialFieldHandler.h"
#include "AI/LuaPathfinderHandler.h"
#include "AI/LuaCombinationMath.h"
#include "Renderer/LuaGraphicDevice.h"
#include "Renderer/LuaCamera.h"
#include "Math/LuaMatrix.h"
#include "FileLoader/LuaFileLoader.h"
#include "Console/LuaConsole.h"
#include "Network/LuaNetwork.h"

namespace LuaBridge
{
	lua_State* g_IOLuaState = nullptr;

	void SetIOLuaState(lua_State* L)
	{
		g_IOLuaState = L;
	}

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

	PotentialField::Embed(L);
	PathfinderHandler::Embed(L);
	CombinationMath::Embed(L);

    LuaEmbedder::AddFunction(L, "LoadMap", &LoadMap, "File");
  }
}