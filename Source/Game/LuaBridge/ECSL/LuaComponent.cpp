#include "LuaComponent.h"
#include "LuaEmbedder/LuaEmbedder.h"

namespace LuaBridge
{
  LuaComponent::LuaComponent()
  {
    m_dataLocation = nullptr;
  }

  LuaComponent::~LuaComponent() { }
  
  void LuaComponent::Embed()
  {
    LuaEmbedder::EmbedClass<LuaComponent>("Component");
    LuaEmbedder::EmbedClassProperty<LuaComponent>("Component", "Float", &LuaComponent::GetFloat, &LuaComponent::SetFloat);
    LuaEmbedder::EmbedClassProperty<LuaComponent>("Component", "Int", &LuaComponent::GetInt, &LuaComponent::SetInt);
    LuaEmbedder::EmbedClassProperty<LuaComponent>("Component", "Bool", &LuaComponent::GetBool, &LuaComponent::SetBool);
    LuaEmbedder::EmbedClassProperty<LuaComponent>("Component", "String", &LuaComponent::GetString, &LuaComponent::SetString);
  }
  
  void LuaComponent::SetDataLocation(ECSL::DataLocation dataLocation)
  {
    m_dataLocation = dataLocation;
  }
  
  int LuaComponent::GetFloat()
  {
    if (m_dataLocation)
      LuaEmbedder::PushFloat(*((float*)m_dataLocation));
    else
      LuaEmbedder::PushNull();
    return 1;
  }
  int LuaComponent::SetFloat()
  {
    *m_dataLocation = LuaEmbedder::PullFloat(1);
    return 0;
  }
  int LuaComponent::GetInt()
  {
    if (m_dataLocation)
      LuaEmbedder::PushInt(*((int*)m_dataLocation));
    else
      LuaEmbedder::PushNull();
    return 1;
  }
  int LuaComponent::SetInt()
  {
    *m_dataLocation = LuaEmbedder::PullInt(1);
    return 0;
  }
  int LuaComponent::GetBool()
  {
    if (m_dataLocation)
      LuaEmbedder::PushBool(*((bool*)m_dataLocation));
    else
      LuaEmbedder::PushNull();
    return 1;
  }
  int LuaComponent::SetBool()
  {
    *m_dataLocation = LuaEmbedder::PullBool(1);
    return 0;
  }
  int LuaComponent::GetString()
  {
    if (m_dataLocation)
      LuaEmbedder::PushString(std::string((const char*)m_dataLocation));
    else
      LuaEmbedder::PushNull();
    return 1;
  }
  int LuaComponent::SetString()
  {
    m_dataLocation = (char*)LuaEmbedder::PullString(1).c_str();
    return 0;
  }
}