#include "LuaGraphicDevice.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "LuaCamera.h"
#include "../Math/LuaMatrix.h"

namespace LuaBridge
{
  LuaGraphicDevice::LuaGraphicDevice() { }
  
  void LuaGraphicDevice::Embed()
  {
    LuaEmbedder::EmbedClass<LuaGraphicDevice>("GraphicDevice");
    
    LuaEmbedder::EmbedClassFunction<LuaGraphicDevice>("GraphicDevice", "ResizeWindow", &LuaGraphicDevice::ResizeWindow);
    LuaEmbedder::EmbedClassFunction<LuaGraphicDevice>("GraphicDevice", "SetTitle", &LuaGraphicDevice::SetTitle);
    LuaEmbedder::EmbedClassFunction<LuaGraphicDevice>("GraphicDevice", "RenderSimpleText", &LuaGraphicDevice::RenderSimpleText);
    LuaEmbedder::EmbedClassFunction<LuaGraphicDevice>("GraphicDevice", "SetSimpleTextColor", &LuaGraphicDevice::SetSimpleTextColor);
    LuaEmbedder::EmbedClassFunction<LuaGraphicDevice>("GraphicDevice", "GetCamera", &LuaGraphicDevice::GetCamera);
    LuaEmbedder::EmbedClassFunction<LuaGraphicDevice>("GraphicDevice", "GetWindowSize", &LuaGraphicDevice::GetWindowSize);
    LuaEmbedder::EmbedClassFunction<LuaGraphicDevice>("GraphicDevice", "LoadModel", &LuaGraphicDevice::LoadModel);
    LuaEmbedder::EmbedClassFunction<LuaGraphicDevice>("GraphicDevice", "ChangeModelTexture", &LuaGraphicDevice::ChangeModelTexture);
    LuaEmbedder::EmbedClassFunction<LuaGraphicDevice>("GraphicDevice", "ChangeModelNormalMap", &LuaGraphicDevice::ChangeModelNormalMap);
    LuaEmbedder::EmbedClassFunction<LuaGraphicDevice>("GraphicDevice", "ChangeModelSpecularMap", &LuaGraphicDevice::ChangeModelSpecularMap);
    
    LuaEmbedder::EmbedClassProperty<LuaGraphicDevice>("GraphicDevice", "Camera", &LuaGraphicDevice::GetCamera, &LuaGraphicDevice::SetCamera);
  }
  
  int LuaGraphicDevice::ResizeWindow()
  {
    int width = LuaEmbedder::PullInt(1);
    int height = LuaEmbedder::PullInt(2);
    GraphicDevice::ResizeWindow(width, height);
    return 0;
  }

  int LuaGraphicDevice::SetTitle()
  {
    std::string title = LuaEmbedder::PullString(1);
    //GraphicDevice::SetTitle(title);
    return 0;
  }

  int LuaGraphicDevice::RenderSimpleText()
  {
    std::string text = LuaEmbedder::PullString(1);
    int x = LuaEmbedder::PullInt(2);
    int y = LuaEmbedder::PullInt(3);
    GraphicDevice::RenderSimpleText(text, x, y);
    return 0;
  }

  int LuaGraphicDevice::SetSimpleTextColor()
  {
    float r = LuaEmbedder::PullFloat(1);
    float g = LuaEmbedder::PullFloat(2);
    float b = LuaEmbedder::PullFloat(3);
    float a = LuaEmbedder::IsFloat(4) ? LuaEmbedder::PullFloat(4) : 1.0f;
    GraphicDevice::SetSimpleTextColor(glm::vec4(r, g, b, a));
    return 0;
  }

  int LuaGraphicDevice::GetCamera()
  {
    Camera* camera = GraphicDevice::GetCamera();
    LuaEmbedder::PushObject<Camera>("Camera", camera);
    return 1;
  }

  int LuaGraphicDevice::SetCamera()
  {
    return 0;
  }

  int LuaGraphicDevice::GetWindowSize()
  {
    int x, y;
    GraphicDevice::GetWindowSize(x, y);
    LuaEmbedder::PushInt(x);
    LuaEmbedder::PushInt(y);
    return 2;
  }

  int LuaGraphicDevice::LoadModel()
  {
    std::string dir = LuaEmbedder::PullString(1);
    std::string file = LuaEmbedder::PullString(2);
    LuaMatrix* matrix = LuaEmbedder::PullObject<LuaMatrix>("Matrix", 3);
    int model = GraphicDevice::LoadModel(dir, file, matrix->GetGlmMatrix());
    LuaEmbedder::PushInt(model);
    return 1;
  }

  int LuaGraphicDevice::ChangeModelTexture()
  {
    int id = LuaEmbedder::PullInt(1);
    std::string fileDir = LuaEmbedder::PullString(2);
    bool result = GraphicDevice::ChangeModelTexture(id, fileDir);
    LuaEmbedder::PushBool(result);
    return 1;
  }

  int LuaGraphicDevice::ChangeModelNormalMap()
  {
    int id = LuaEmbedder::PullInt(1);
    std::string fileDir = LuaEmbedder::PullString(2);
    bool result = GraphicDevice::ChangeModelNormalMap(id, fileDir);
    LuaEmbedder::PushBool(result);
    return 1;
  }

  int LuaGraphicDevice::ChangeModelSpecularMap()
  {
    int id = LuaEmbedder::PullInt(1);
    std::string fileDir = LuaEmbedder::PullString(2);
    bool result = GraphicDevice::ChangeModelSpecularMap(id, fileDir);
    LuaEmbedder::PushBool(result);
    return 1;
  }
}