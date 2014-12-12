#include "LuaEntityTemplate.h"
#include "LuaEmbedder/LuaEmbedder.h"

namespace LuaBridge
{
  LuaEntityTemplate::LuaEntityTemplate() { }

  LuaEntityTemplate::~LuaEntityTemplate()
  {
    m_components.clear();
  }
  
  void LuaEntityTemplate::Embed()
  {
    LuaEmbedder::EmbedClass<LuaEntityTemplate>("EntityTemplate");
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>("EntityTemplate", "GetName", &LuaEntityTemplate::GetName);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>("EntityTemplate", "SetName", &LuaEntityTemplate::SetName);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>("EntityTemplate", "SetFloat", &LuaEntityTemplate::SetFloat);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>("EntityTemplate", "SetFloat2", &LuaEntityTemplate::SetFloat2);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>("EntityTemplate", "SetFloat3", &LuaEntityTemplate::SetFloat3);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>("EntityTemplate", "SetFloat4", &LuaEntityTemplate::SetFloat4);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>("EntityTemplate", "SetInt", &LuaEntityTemplate::SetInt);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>("EntityTemplate", "SetInt2", &LuaEntityTemplate::SetInt2);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>("EntityTemplate", "SetInt3", &LuaEntityTemplate::SetInt3);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>("EntityTemplate", "SetInt4", &LuaEntityTemplate::SetInt4);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>("EntityTemplate", "SetString", &LuaEntityTemplate::SetString);
	LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>("EntityTemplate", "SetBool", &LuaEntityTemplate::SetBool);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>("EntityTemplate", "SetModel", &LuaEntityTemplate::SetModel);
    LuaEmbedder::EmbedClassProperty<LuaEntityTemplate>("EntityTemplate", "Name", &LuaEntityTemplate::GetName, &LuaEntityTemplate::SetName);
  }
  
  int LuaEntityTemplate::GetName()
  {
    LuaEmbedder::PushString(m_name);
    return 1;
  }

  int LuaEntityTemplate::SetName()
  {
    m_name = LuaEmbedder::PullString(1);
    return 0;
  }
  
  int LuaEntityTemplate::SetFloat()
  {
    std::string componentName = LuaEmbedder::PullString(1);
    m_components[componentName].push_back(ECSL::TemplateEntry(LuaEmbedder::PullFloat(2)));
    return 0;
  }
  int LuaEntityTemplate::SetFloat2()
  {
    std::string componentName = LuaEmbedder::PullString(1);
    m_components[componentName].push_back(ECSL::TemplateEntry(LuaEmbedder::PullFloat(2)));
    m_components[componentName].push_back(ECSL::TemplateEntry(LuaEmbedder::PullFloat(3)));
    return 0;
  }
  int LuaEntityTemplate::SetFloat3()
  {
    std::string componentName = LuaEmbedder::PullString(1);
    m_components[componentName].push_back(ECSL::TemplateEntry(LuaEmbedder::PullFloat(2)));
    m_components[componentName].push_back(ECSL::TemplateEntry(LuaEmbedder::PullFloat(3)));
    m_components[componentName].push_back(ECSL::TemplateEntry(LuaEmbedder::PullFloat(4)));
    return 0;
  }
  int LuaEntityTemplate::SetFloat4()
  {
    std::string componentName = LuaEmbedder::PullString(1);
    m_components[componentName].push_back(ECSL::TemplateEntry(LuaEmbedder::PullFloat(2)));
    m_components[componentName].push_back(ECSL::TemplateEntry(LuaEmbedder::PullFloat(3)));
    m_components[componentName].push_back(ECSL::TemplateEntry(LuaEmbedder::PullFloat(4)));
    m_components[componentName].push_back(ECSL::TemplateEntry(LuaEmbedder::PullFloat(5)));
    return 0;
  }
  
  int LuaEntityTemplate::SetInt()
  {
    std::string componentName = LuaEmbedder::PullString(1);
    m_components[componentName].push_back(ECSL::TemplateEntry(LuaEmbedder::PullInt(2)));
    return 0;
  }
  int LuaEntityTemplate::SetInt2()
  {
    std::string componentName = LuaEmbedder::PullString(1);
    m_components[componentName].push_back(ECSL::TemplateEntry(LuaEmbedder::PullInt(2)));
    m_components[componentName].push_back(ECSL::TemplateEntry(LuaEmbedder::PullInt(3)));
    return 0;
  }
  int LuaEntityTemplate::SetInt3()
  {
    std::string componentName = LuaEmbedder::PullString(1);
    m_components[componentName].push_back(ECSL::TemplateEntry(LuaEmbedder::PullInt(2)));
    m_components[componentName].push_back(ECSL::TemplateEntry(LuaEmbedder::PullInt(3)));
    m_components[componentName].push_back(ECSL::TemplateEntry(LuaEmbedder::PullInt(4)));
    return 0;
  }
  int LuaEntityTemplate::SetInt4()
  {
    std::string componentName = LuaEmbedder::PullString(1);
    m_components[componentName].push_back(ECSL::TemplateEntry(LuaEmbedder::PullInt(2)));
    m_components[componentName].push_back(ECSL::TemplateEntry(LuaEmbedder::PullInt(3)));
    m_components[componentName].push_back(ECSL::TemplateEntry(LuaEmbedder::PullInt(4)));
    m_components[componentName].push_back(ECSL::TemplateEntry(LuaEmbedder::PullInt(5)));
    return 0;
  }

  int LuaEntityTemplate::SetString()
  {
    std::string componentName = LuaEmbedder::PullString(1);
    m_components[componentName].push_back(ECSL::TemplateEntry(LuaEmbedder::PullString(2)));
    return 0;
  }
  
  int LuaEntityTemplate::SetBool()
  {
	  bool active = LuaEmbedder::PullBool(2);

	  std::string componentName = LuaEmbedder::PullString(1);
	  m_components[componentName].push_back(ECSL::TemplateEntry(active));

	  return 0;
  };

  int LuaEntityTemplate::SetModel()
  {
    std::string componentName = LuaEmbedder::PullString(1);
    std::string modelName = LuaEmbedder::PullString(2);
    std::string folderName = LuaEmbedder::PullString(3);
    m_components[componentName].push_back(ECSL::TemplateEntry(modelName));
    m_components[componentName].push_back(ECSL::TemplateEntry(folderName));
    return 0;
  }
  
  ECSL::EntityTemplate* LuaEntityTemplate::CreateEntityTemplate()
  {
    return new ECSL::EntityTemplate(m_name, m_components);
  }
}