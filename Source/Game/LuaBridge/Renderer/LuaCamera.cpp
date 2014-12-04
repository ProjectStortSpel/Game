#include "LuaCamera.h"
#include "LuaEmbedder/LuaEmbedder.h"

namespace LuaBridge
{
  LuaCamera::LuaCamera() { }
  
  void LuaCamera::Embed()
  {
    LuaEmbedder::EmbedClass<LuaCamera>("Camera");
    LuaEmbedder::EmbedClassFunction<LuaCamera>("Camera", "MoveForward", &LuaCamera::MoveForward);
    LuaEmbedder::EmbedClassFunction<LuaCamera>("Camera", "MoveBackward", &LuaCamera::MoveBackward);
    LuaEmbedder::EmbedClassFunction<LuaCamera>("Camera", "MoveLeft", &LuaCamera::MoveLeft);
    LuaEmbedder::EmbedClassFunction<LuaCamera>("Camera", "MoveRight", &LuaCamera::MoveRight);
    LuaEmbedder::EmbedClassFunction<LuaCamera>("Camera", "UpdateMouse", &LuaCamera::UpdateMouse);
  }
  
  int LuaCamera::MoveForward()
  {
    float dt = LuaEmbedder::PullFloat(1);
    Camera::MoveForward(dt);
    return 0;
  }

  int LuaCamera::MoveBackward()
  {
    float dt = LuaEmbedder::PullFloat(1);
    Camera::MoveBackward(dt);
    return 0;
  }

  int LuaCamera::MoveLeft()
  {
    float dt = LuaEmbedder::PullFloat(1);
    Camera::MoveLeft(dt);
    return 0;
  }

  int LuaCamera::MoveRight()
  {
    float dt = LuaEmbedder::PullFloat(1);
    Camera::MoveRight(dt);
    return 0;
  }

  int LuaCamera::UpdateMouse()
  {
    float midX = LuaEmbedder::PullFloat(1);
    float midY = LuaEmbedder::PullFloat(2);
    int x = LuaEmbedder::PullInt(3);
    int y = LuaEmbedder::PullInt(4);
    Camera::UpdateMouse(midX, midY, x, y);
    return 0;
  }
}