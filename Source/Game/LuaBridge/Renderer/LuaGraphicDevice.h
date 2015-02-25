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
	namespace LuaGraphicDevice
	{
		void Embed(lua_State* L);
		void SetGraphicDevice(Renderer::GraphicDevice* graphicDevice);
		Renderer::GraphicDevice* GetGraphicDevice();
	};
}

#endif