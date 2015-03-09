#include "LuaEntityTemplate.h"
#include "LuaEmbedder/LuaEmbedder.h"

namespace LuaBridge
{
  LuaEntityTemplate::LuaEntityTemplate(lua_State* L) { }

  LuaEntityTemplate::~LuaEntityTemplate()
  {
	  for (auto keyValuePair : m_components)
	  {
		  for (auto templateEntry : keyValuePair.second)
			  delete(templateEntry);
	  }
    m_components.clear();
  }
  
  void LuaEntityTemplate::Embed(lua_State* L)
  {
    LuaEmbedder::EmbedClass<LuaEntityTemplate>(L, "EntityTemplate");
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>(L, "EntityTemplate", "GetName", &LuaEntityTemplate::GetName);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>(L, "EntityTemplate", "SetName", &LuaEntityTemplate::SetName);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>(L, "EntityTemplate", "AddComponent", &LuaEntityTemplate::AddComponent);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>(L, "EntityTemplate", "SetFloat", &LuaEntityTemplate::SetFloat);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>(L, "EntityTemplate", "SetFloat2", &LuaEntityTemplate::SetFloat2);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>(L, "EntityTemplate", "SetFloat3", &LuaEntityTemplate::SetFloat3);
	LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>(L, "EntityTemplate", "SetFloat4", &LuaEntityTemplate::SetFloat4);
	LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>(L, "EntityTemplate", "SetFloat5", &LuaEntityTemplate::SetFloat5);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>(L, "EntityTemplate", "SetInt", &LuaEntityTemplate::SetInt);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>(L, "EntityTemplate", "SetInt2", &LuaEntityTemplate::SetInt2);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>(L, "EntityTemplate", "SetInt3", &LuaEntityTemplate::SetInt3);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>(L, "EntityTemplate", "SetInt4", &LuaEntityTemplate::SetInt4);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>(L, "EntityTemplate", "SetString", &LuaEntityTemplate::SetString);
	LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>(L, "EntityTemplate", "SetBool", &LuaEntityTemplate::SetBool);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>(L, "EntityTemplate", "SetModel", &LuaEntityTemplate::SetModel);
    LuaEmbedder::EmbedClassFunction<LuaEntityTemplate>(L, "EntityTemplate", "SetPointlight", &LuaEntityTemplate::SetPointlight);
    LuaEmbedder::EmbedClassProperty<LuaEntityTemplate>(L, "EntityTemplate", "Name", &LuaEntityTemplate::GetName, &LuaEntityTemplate::SetName);
  }
  
  int LuaEntityTemplate::GetName(lua_State* L)
  {
    LuaEmbedder::PushString(L, m_name);
    return 1;
  }

  int LuaEntityTemplate::SetName(lua_State* L)
  {
    m_name = LuaEmbedder::PullString(L, 1);
    return 0;
  }
  
  int LuaEntityTemplate::AddComponent(lua_State* L)
  {
    std::string componentName = LuaEmbedder::PullString(L, 1);
    m_components[componentName] = std::vector<ECSL::TemplateEntry*>();
    return 0;
  }
  
  int LuaEntityTemplate::SetFloat(lua_State* L)
  {
    std::string componentName = LuaEmbedder::PullString(L, 1);
    m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullFloat(L, 2)));
    return 0;
  }
  int LuaEntityTemplate::SetFloat2(lua_State* L)
  {
    std::string componentName = LuaEmbedder::PullString(L, 1);
    m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullFloat(L, 2)));
    m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullFloat(L, 3)));
    return 0;
  }
  int LuaEntityTemplate::SetFloat3(lua_State* L)
  {
    std::string componentName = LuaEmbedder::PullString(L, 1);
    m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullFloat(L, 2)));
    m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullFloat(L, 3)));
    m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullFloat(L, 4)));
    return 0;
  }
  int LuaEntityTemplate::SetFloat4(lua_State* L)
  {
	  std::string componentName = LuaEmbedder::PullString(L, 1);
	  m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullFloat(L, 2)));
	  m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullFloat(L, 3)));
	  m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullFloat(L, 4)));
	  m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullFloat(L, 5)));
	  return 0;
  }
  int LuaEntityTemplate::SetFloat5(lua_State* L)
  {
	  std::string componentName = LuaEmbedder::PullString(L, 1);
	  m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullFloat(L, 2)));
	  m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullFloat(L, 3)));
	  m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullFloat(L, 4)));
	  m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullFloat(L, 5)));
	  m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullFloat(L, 6)));
	  return 0;
  }
  
  int LuaEntityTemplate::SetInt(lua_State* L)
  {
    std::string componentName = LuaEmbedder::PullString(L, 1);
	m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullInt(L, 2)));
    return 0;
  }
  int LuaEntityTemplate::SetInt2(lua_State* L)
  {
    std::string componentName = LuaEmbedder::PullString(L, 1);
	m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullInt(L, 2)));
	m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullInt(L, 3)));
    return 0;
  }
  int LuaEntityTemplate::SetInt3(lua_State* L)
  {
    std::string componentName = LuaEmbedder::PullString(L, 1);
	m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullInt(L, 2)));
	m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullInt(L, 3)));
	m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullInt(L, 4)));
    return 0;
  }
  int LuaEntityTemplate::SetInt4(lua_State* L)
  {
    std::string componentName = LuaEmbedder::PullString(L, 1);
	m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullInt(L, 2)));
	m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullInt(L, 3)));
	m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullInt(L, 4)));
	m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullInt(L, 5)));
    return 0;
  }

  int LuaEntityTemplate::SetString(lua_State* L)
  {
    std::string componentName = LuaEmbedder::PullString(L, 1);
	m_components[componentName].push_back(new ECSL::TemplateEntry(LuaEmbedder::PullString(L, 2)));
    return 0;
  }
  
  int LuaEntityTemplate::SetBool(lua_State* L)
  {
	  bool active = LuaEmbedder::PullBool(L, 2);

	  std::string componentName = LuaEmbedder::PullString(L, 1);
	  m_components[componentName].push_back(new ECSL::TemplateEntry(active));

	  return 0;
  };

  int LuaEntityTemplate::SetModel(lua_State* L)
  {
    std::string componentName = LuaEmbedder::PullString(L, 1);
    std::string modelName = LuaEmbedder::PullString(L, 2);
	std::string folderName = LuaEmbedder::PullString(L, 3);
	int renderType = LuaEmbedder::PullInt(L, 4);
	int renderShadow = LuaEmbedder::IsBool(L, 5) ? (int)LuaEmbedder::PullBool(L, 5) : 1;
	m_components[componentName].push_back(new ECSL::TemplateEntry(modelName));
	m_components[componentName].push_back(new ECSL::TemplateEntry(folderName));
	m_components[componentName].push_back(new ECSL::TemplateEntry(renderType));
	m_components[componentName].push_back(new ECSL::TemplateEntry(renderShadow));
    return 0;
  }
  
  int LuaEntityTemplate::SetPointlight(lua_State* L)
  {
    std::string componentName = LuaEmbedder::PullString(L, 1);
    float posX = LuaEmbedder::PullFloat(L, 2);
    float posY = LuaEmbedder::PullFloat(L, 3);
    float posZ = LuaEmbedder::PullFloat(L, 4);
    float ambientInt = LuaEmbedder::PullFloat(L, 5);
    float diffuseInt = LuaEmbedder::PullFloat(L, 6);
    float specularInt = LuaEmbedder::PullFloat(L, 7);
    float colorRed = LuaEmbedder::PullFloat(L, 8);
    float colorGreen = LuaEmbedder::PullFloat(L, 9);
    float colorBlue = LuaEmbedder::PullFloat(L, 10);
    float range = LuaEmbedder::PullFloat(L, 11);
	m_components[componentName].push_back(new ECSL::TemplateEntry(posX));
	m_components[componentName].push_back(new ECSL::TemplateEntry(posY));
	m_components[componentName].push_back(new ECSL::TemplateEntry(posZ));
	m_components[componentName].push_back(new ECSL::TemplateEntry(ambientInt));
	m_components[componentName].push_back(new ECSL::TemplateEntry(diffuseInt));
	m_components[componentName].push_back(new ECSL::TemplateEntry(specularInt));
	m_components[componentName].push_back(new ECSL::TemplateEntry(colorRed));
	m_components[componentName].push_back(new ECSL::TemplateEntry(colorGreen));
	m_components[componentName].push_back(new ECSL::TemplateEntry(colorBlue));
	m_components[componentName].push_back(new ECSL::TemplateEntry(range));
    return 0;
  }
  
  ECSL::EntityTemplate* LuaEntityTemplate::CreateEntityTemplate()
  {
	  std::string newName = m_name;
	  std::map<std::string, std::vector<ECSL::TemplateEntry*>> newComponents = std::map<std::string, std::vector<ECSL::TemplateEntry*>>();
	  for (auto component : m_components)
	  {
		  newComponents[component.first] = std::vector<ECSL::TemplateEntry*>();
		  for (auto componentData : component.second)
		  {
			  switch (componentData->GetDataType())
			  {
			  case ECSL::ComponentDataType::BOOL:
				  newComponents[component.first].push_back(new ECSL::TemplateEntry(componentData->GetBoolData()));
				  break;

			  case ECSL::ComponentDataType::INT:
					newComponents[component.first].push_back(new ECSL::TemplateEntry(componentData->GetIntData()));
					break;

			  case ECSL::ComponentDataType::TEXT:
				  newComponents[component.first].push_back(new ECSL::TemplateEntry(componentData->GetTextData()));
				  break;

			  case ECSL::ComponentDataType::FLOAT:
				  newComponents[component.first].push_back(new ECSL::TemplateEntry(componentData->GetFloatData()));
				  break;

			  default:
				  assert(false);
				  break;
			  }
		  }

	  }

	  ECSL::EntityTemplate* newTemplate = new ECSL::EntityTemplate(newName, newComponents);

	  return newTemplate;
  }
}