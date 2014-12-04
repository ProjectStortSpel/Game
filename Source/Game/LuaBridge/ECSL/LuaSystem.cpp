#include "LuaSystem.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "LuaComponent.h"

namespace LuaBridge
{
  LuaSystem::LuaSystem() { }
  
  void LuaSystem::Embed()
  {
    LuaEmbedder::EmbedClass<LuaSystem>("System", false);
    LuaEmbedder::EmbedClassFunction<LuaSystem>("System", "AddComponentTypeToFilter", &LuaSystem::AddComponentTypeToFilter);
    LuaEmbedder::EmbedClassFunction<LuaSystem>("System", "GetEntities", &LuaSystem::GetEntities);
    LuaEmbedder::EmbedClassFunction<LuaSystem>("System", "GetComponent", &LuaSystem::GetComponent);
    
    LuaEmbedder::AddInt("Mandatory", (int)ECSL::FilterType::Mandatory, "FilterType");
    LuaEmbedder::AddInt("RequiresOneOf", (int)ECSL::FilterType::RequiresOneOf, "FilterType");
    LuaEmbedder::AddInt("Excluded", (int)ECSL::FilterType::Excluded, "FilterType");
  }
  
  void LuaSystem::Update(float _dt)
  {
    LuaEmbedder::PushFloat(_dt);
    LuaEmbedder::CallMethod<LuaSystem>("System", "Update", this, 1);
  }

  void LuaSystem::Initialize()
  {
    LuaEmbedder::CallMethod<LuaSystem>("System", "Initialize", this);
  }

  void LuaSystem::OnEntityAdded(unsigned int _entityId)
  {
    LuaEmbedder::PushInt((int)_entityId);
    LuaEmbedder::CallMethod<LuaSystem>("System", "OnEntityAdded", this, 1);
  }

  void LuaSystem::OnEntityRemoved(unsigned int _entityId)
  {
    LuaEmbedder::PushInt((int)_entityId);
    LuaEmbedder::CallMethod<LuaSystem>("System", "OnEntityRemoved", this, 1);
  }
  
  int LuaSystem::GetComponent()
  {
    ECSL::DataLocation dataLocation;
    unsigned int entityId = (unsigned int)LuaEmbedder::PullInt(1);
    if (LuaEmbedder::IsInt(2))
    {
      unsigned int componentTypeId = (unsigned int)LuaEmbedder::PullInt(2);
      unsigned int index = (unsigned int)LuaEmbedder::PullInt(3);
      dataLocation = System::GetComponent(entityId, componentTypeId, index);
    }
    else
    {
      std::string componentType = LuaEmbedder::PullString(2);
      std::string variableName = LuaEmbedder::PullString(3);
      dataLocation = System::GetComponent(entityId, componentType, variableName);
    }
    LuaComponent* component = new LuaComponent();
    component->SetDataLocation(dataLocation);
    LuaEmbedder::PushObject<LuaComponent>("Component", component, true);
    return 1;
  }

  
  int LuaSystem::AddComponentTypeToFilter()
  {
    std::string componentType = LuaEmbedder::PullString(1);
    ECSL::FilterType filterType = (ECSL::FilterType)LuaEmbedder::PullInt(2);
    System::AddComponentTypeToFilter(componentType, filterType);
    return 0;
  }
  
  int LuaSystem::GetEntities()
  {
    LuaEmbedder::PushUnsignedIntArray(System::GetEntities()->data(), System::GetEntities()->size());
    return 1;
  }
}