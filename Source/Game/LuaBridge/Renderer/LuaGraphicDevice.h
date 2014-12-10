#ifndef LUAGRAPHICDEVICE_H
#define LUAGRAPHICDEVICE_H

#include "Renderer/GraphicDevice.h"

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