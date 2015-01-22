#include "LuaCamera.h"
#include "LuaEmbedder/LuaEmbedder.h"

namespace LuaBridge
{
  LuaCamera::LuaCamera(lua_State* L) { }
  
  void LuaCamera::Embed(lua_State* L)
  {
    LuaEmbedder::EmbedClass<LuaCamera>(L, "Camera");
    LuaEmbedder::EmbedClassFunction<LuaCamera>(L, "Camera", "MoveForward", &LuaCamera::MoveForward);
    LuaEmbedder::EmbedClassFunction<LuaCamera>(L, "Camera", "MoveBackward", &LuaCamera::MoveBackward);
    LuaEmbedder::EmbedClassFunction<LuaCamera>(L, "Camera", "MoveLeft", &LuaCamera::MoveLeft);
    LuaEmbedder::EmbedClassFunction<LuaCamera>(L, "Camera", "MoveRight", &LuaCamera::MoveRight);
    LuaEmbedder::EmbedClassFunction<LuaCamera>(L, "Camera", "UpdateMouse", &LuaCamera::UpdateMouse);
    LuaEmbedder::EmbedClassFunction<LuaCamera>(L, "Camera", "GetRight", &LuaCamera::GetRight);
    LuaEmbedder::EmbedClassFunction<LuaCamera>(L, "Camera", "GetUp", &LuaCamera::GetUp);
    LuaEmbedder::EmbedClassFunction<LuaCamera>(L, "Camera", "GetLook", &LuaCamera::GetLook);
    LuaEmbedder::EmbedClassFunction<LuaCamera>(L, "Camera", "GetPosition", &LuaCamera::GetPosition);
  }
  
  int LuaCamera::MoveForward(lua_State* L)
  {
    float dt = LuaEmbedder::PullFloat(L, 1);
    Camera::MoveForward(dt);
    return 0;
  }

  int LuaCamera::MoveBackward(lua_State* L)
  {
    float dt = LuaEmbedder::PullFloat(L, 1);
    Camera::MoveBackward(dt);
    return 0;
  }

  int LuaCamera::MoveLeft(lua_State* L)
  {
    float dt = LuaEmbedder::PullFloat(L, 1);
    Camera::MoveLeft(dt);
    return 0;
  }

  int LuaCamera::MoveRight(lua_State* L)
  {
    float dt = LuaEmbedder::PullFloat(L, 1);
    Camera::MoveRight(dt);
    return 0;
  }

  int LuaCamera::UpdateMouse(lua_State* L)
  {
    float midX = LuaEmbedder::PullFloat(L, 1);
    float midY = LuaEmbedder::PullFloat(L, 2);
    int x = LuaEmbedder::PullInt(L, 3);
    int y = LuaEmbedder::PullInt(L, 4);
    Camera::UpdateMouse(midX, midY, x, y);
    return 0;
  }
  
  int LuaCamera::GetRight(lua_State* L)
  {
    glm::vec3* right = Camera::GetRight();
    LuaEmbedder::PushFloat(L, right->x);
    LuaEmbedder::PushFloat(L, right->y);
    LuaEmbedder::PushFloat(L, right->z);
    return 3;
  }

  int LuaCamera::GetUp(lua_State* L)
  {
    glm::vec3* up = Camera::GetUp();
    LuaEmbedder::PushFloat(L, up->x);
    LuaEmbedder::PushFloat(L, up->y);
    LuaEmbedder::PushFloat(L, up->z);
    return 3;
  }

  int LuaCamera::GetLook(lua_State* L)
  {
    glm::vec3* look = Camera::GetLook();
    LuaEmbedder::PushFloat(L, look->x);
    LuaEmbedder::PushFloat(L, look->y);
    LuaEmbedder::PushFloat(L, look->z);
    return 3;
  }

  int LuaCamera::GetPosition(lua_State* L)
  {
    glm::vec3* position = Camera::GetPos();
    LuaEmbedder::PushFloat(L, position->x);
    LuaEmbedder::PushFloat(L, position->y);
    LuaEmbedder::PushFloat(L, position->z);
    return 3;
  }
}