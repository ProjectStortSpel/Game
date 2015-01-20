#include "LuaCamera.h"
#include "LuaEmbedder/LuaEmbedder.h"

namespace LuaBridge
{
  LuaCamera::LuaCamera() { }
  
  void LuaCamera::Embed()
  {
    LuaEmbedder::EmbedClass<LuaCamera>("Camera");
	LuaEmbedder::EmbedClassFunction<LuaCamera>("Camera", "MoveToAndLookAt", &LuaCamera::MoveToAndLookAt);
    LuaEmbedder::EmbedClassFunction<LuaCamera>("Camera", "MoveForward", &LuaCamera::MoveForward);
    LuaEmbedder::EmbedClassFunction<LuaCamera>("Camera", "MoveBackward", &LuaCamera::MoveBackward);
    LuaEmbedder::EmbedClassFunction<LuaCamera>("Camera", "MoveLeft", &LuaCamera::MoveLeft);
    LuaEmbedder::EmbedClassFunction<LuaCamera>("Camera", "MoveRight", &LuaCamera::MoveRight);
    LuaEmbedder::EmbedClassFunction<LuaCamera>("Camera", "UpdateMouse", &LuaCamera::UpdateMouse);
    LuaEmbedder::EmbedClassFunction<LuaCamera>("Camera", "GetRight", &LuaCamera::GetRight);
    LuaEmbedder::EmbedClassFunction<LuaCamera>("Camera", "GetUp", &LuaCamera::GetUp);
    LuaEmbedder::EmbedClassFunction<LuaCamera>("Camera", "GetLook", &LuaCamera::GetLook);
    LuaEmbedder::EmbedClassFunction<LuaCamera>("Camera", "GetPosition", &LuaCamera::GetPosition);
	LuaEmbedder::EmbedClassFunction<LuaCamera>("Camera", "SetRight", &LuaCamera::SetRight);
	LuaEmbedder::EmbedClassFunction<LuaCamera>("Camera", "SetUp", &LuaCamera::SetUp);
	LuaEmbedder::EmbedClassFunction<LuaCamera>("Camera", "SetLook", &LuaCamera::SetLook);
	LuaEmbedder::EmbedClassFunction<LuaCamera>("Camera", "SetPosition", &LuaCamera::SetPosition);
  }
  
  int LuaCamera::MoveToAndLookAt()
  {
	  float px = LuaEmbedder::PullFloat(1);
	  float py = LuaEmbedder::PullFloat(2);
	  float pz = LuaEmbedder::PullFloat(3);

	  float ux = LuaEmbedder::PullFloat(4);
	  float uy = LuaEmbedder::PullFloat(5);
	  float uz = LuaEmbedder::PullFloat(6);

	  float tx = LuaEmbedder::PullFloat(7);
	  float ty = LuaEmbedder::PullFloat(8);
	  float tz = LuaEmbedder::PullFloat(9);

	  float time = LuaEmbedder::PullFloat(10);

	  Camera::MoveToAndLookAt(vec3(px, py, pz), vec3(ux, uy, uz), vec3(tx, ty, tz), time);
	  return 0;
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
  
  int LuaCamera::GetRight()
  {
    glm::vec3* right = Camera::GetRight();
    LuaEmbedder::PushFloat(right->x);
    LuaEmbedder::PushFloat(right->y);
    LuaEmbedder::PushFloat(right->z);
    return 3;
  }

  int LuaCamera::GetUp()
  {
    glm::vec3* up = Camera::GetUp();
    LuaEmbedder::PushFloat(up->x);
    LuaEmbedder::PushFloat(up->y);
    LuaEmbedder::PushFloat(up->z);
    return 3;
  }

  int LuaCamera::GetLook()
  {
    glm::vec3* look = Camera::GetLook();
    LuaEmbedder::PushFloat(look->x);
    LuaEmbedder::PushFloat(look->y);
    LuaEmbedder::PushFloat(look->z);
    return 3;
  }

  int LuaCamera::GetPosition()
  {
    glm::vec3* position = Camera::GetPos();
    LuaEmbedder::PushFloat(position->x);
    LuaEmbedder::PushFloat(position->y);
    LuaEmbedder::PushFloat(position->z);
    return 3;
  }

  int LuaCamera::SetRight()
  {
	  float x = LuaEmbedder::PullFloat(1);
	  float y = LuaEmbedder::PullFloat(2);
	  float z = LuaEmbedder::PullFloat(3);
	  glm::vec3* right = Camera::GetRight();
	  right->x = x;
	  right->y = y;
	  right->z = z;
	  return 0;
  }

  int LuaCamera::SetUp()
  {
	  float x = LuaEmbedder::PullFloat(1);
	  float y = LuaEmbedder::PullFloat(2);
	  float z = LuaEmbedder::PullFloat(3);
	  glm::vec3* up = Camera::GetUp();
	  up->x = x;
	  up->y = y;
	  up->z = z;
	  return 0;
  }

  int LuaCamera::SetLook()
  {
	  float x = LuaEmbedder::PullFloat(1);
	  float y = LuaEmbedder::PullFloat(2);
	  float z = LuaEmbedder::PullFloat(3);
	  glm::vec3* look = Camera::GetLook();
	  look->x = x;
	  look->y = y;
	  look->z = z;
	  return 0;
  }

  int LuaCamera::SetPosition()
  {
	  float x = LuaEmbedder::PullFloat(1);
	  float y = LuaEmbedder::PullFloat(2);
	  float z = LuaEmbedder::PullFloat(3);
	  glm::vec3* position = Camera::GetPos();
	  position->x = x;
	  position->y = y;
	  position->z = z;
	  return 0;
  }

}