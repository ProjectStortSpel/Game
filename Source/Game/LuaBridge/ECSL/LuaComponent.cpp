#include "LuaComponent.h"
#include "LuaEmbedder/LuaEmbedder.h"

namespace LuaBridge
{
  LuaComponent::LuaComponent() { }
  
  void LuaComponent::Embed()
  {
    LuaEmbedder::EmbedClass<LuaComponent>("Component", false);
    LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "GetName", &LuaComponent::GetName);
    LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "SetName", &LuaComponent::SetName);
    LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "GetTableType", &LuaComponent::GetTableType);
    LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "SetTableType", &LuaComponent::SetTableType);
    LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "AddVariable", &LuaComponent::AddVariable);
    LuaEmbedder::EmbedClassProperty<LuaComponent>("Component", "Name", &LuaComponent::GetName, &LuaComponent::SetName);
    LuaEmbedder::EmbedClassProperty<LuaComponent>("Component", "TableType", &LuaComponent::GetTableType, &LuaComponent::SetTableType);
  
    LuaEmbedder::AddInt("None", (int)ECSL::TableType::None, "TableType");
    LuaEmbedder::AddInt("Array", (int)ECSL::TableType::Array, "TableType");
    LuaEmbedder::AddInt("Map", (int)ECSL::TableType::Map, "TableType");
    
    LuaEmbedder::AddInt("Float", sizeof(float), "ByteSize");
    LuaEmbedder::AddInt("Int", sizeof(int), "ByteSize");
    LuaEmbedder::AddInt("Pointer", sizeof(void*), "ByteSize");
  }
  
  int LuaComponent::GetName()
  {
    LuaEmbedder::PushString(m_name);
    return 1;
  }

  int LuaComponent::SetName()
  {
    m_name = LuaEmbedder::PullString(1);
    return 0;
  }

  int LuaComponent::GetTableType()
  {
    LuaEmbedder::PushInt(m_tableType);
    return 1;
  }

  int LuaComponent::SetTableType()
  {
    m_tableType = (ECSL::TableType)LuaEmbedder::PullInt(1);
    return 0;
  }
  
  int LuaComponent::AddVariable()
  {
    std::string variableName = LuaEmbedder::PullString(1);
    int byteSize = LuaEmbedder::PullInt(2);
    ECSL::ComponentVariable variable = ECSL::ComponentVariable(variableName, byteSize);
    m_variables.insert(std::pair<std::string, ECSL::ComponentVariable>(variableName, variable));
    return 0;
  }
  
  ECSL::ComponentType LuaComponent::GetComponentType()
  {
    return ECSL::ComponentType(m_name, m_tableType, m_variables);
  }
}