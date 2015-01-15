#ifndef LUAGRAPHICDEVICE_H
#define LUAGRAPHICDEVICE_H

#ifdef __ANDROID__
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif

namespace LuaBridge
{
  class LuaGraphicDevice : public Renderer::GraphicDevice
  {
  public:
    LuaGraphicDevice();
    
    static void Embed();
    
  private:
    int ResizeWindow();
    int SetTitle();
    
    int RenderSimpleText();
    int SetSimpleTextColor();
    
    int GetCamera();
    int SetCamera();
    
    int GetWindowSize();
    
    int LoadModel();
    int ChangeModelTexture();
    int ChangeModelNormalMap();
    int ChangeModelSpecularMap();
  };
}

#endif