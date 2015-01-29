#ifndef LUAGRAPHICDEVICE_H
#define LUAGRAPHICDEVICE_H

#if defined(__ANDROID__) || defined(__IOS__)
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif

#include "Input/InputWrapper.h"
#include <LuaEmbedder/LuaEmbedder.h>

namespace LuaBridge
{
  class LuaGraphicDevice : public Renderer::GraphicDevice
  {
  public:
    LuaGraphicDevice(lua_State* L);
    
    static void Embed(lua_State* L);
    
  private:
	int GetAspectRatio(lua_State* L);
	int GetTouchPosition(lua_State* L);

    int ResizeWindow(lua_State* L);
    int SetTitle(lua_State* L);
    
    int RenderSimpleText(lua_State* L);
    int SetSimpleTextColor(lua_State* L);
    
    int GetCamera(lua_State* L);
    int SetCamera(lua_State* L);
    
    int GetWindowSize(lua_State* L);
    
    int LoadModel(lua_State* L);
    int ChangeModelTexture(lua_State* L);
    int ChangeModelNormalMap(lua_State* L);
    int ChangeModelSpecularMap(lua_State* L);

	int LuaGraphicDevice::AddFont(lua_State* L);
	int LuaGraphicDevice::CreateTextTexture(lua_State* L);
	int LuaGraphicDevice::CreateWrappedTextTexture(lua_State* L);
  };
}

#endif