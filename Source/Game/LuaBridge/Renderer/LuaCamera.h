#ifndef LUACAMERA_H
#define LUACAMERA_H

#include "Renderer/Camera.h"

namespace LuaBridge
{
  class LuaCamera : public Camera
  {
  public:
    LuaCamera();
    
    static void Embed();
    
  private:
    int MoveForward();
    int MoveBackward();
    int MoveLeft();
    int MoveRight();
    
    int UpdateMouse();
    
    int GetRight();
    int GetUp();
    int GetLook();
    int GetPosition();
  };
}

#endif