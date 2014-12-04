#include "LuaComponentType.h"
#include "LuaEmbedder/LuaEmbedder.h"

namespace LuaBridge
{
  LuaComponentType::LuaComponentType() { }

  LuaComponentType::~LuaComponentType()
  {
    m_variables.clear();
  }
  
  void LuaComponentType::Embed()
  {
    LuaEmbedder::EmbedClass<LuaComponentType>("ComponentType");
    LuaEmbedder::EmbedClassFunction<LuaComponentType>("ComponentType", "GetName", &LuaComponentType::GetName);
    LuaEmbedder::EmbedClassFunction<LuaComponentType>("ComponentType", "SetName", &LuaComponentType::SetName);
    LuaEmbedder::EmbedClassFunction<LuaComponentType>("ComponentType", "GetTableType", &LuaComponentType::GetTableType);
    LuaEmbedder::EmbedClassFunction<LuaComponentType>("ComponentType", "SetTableType", &LuaComponentType::SetTableType);
    LuaEmbedder::EmbedClassFunction<LuaComponentType>("ComponentType", "AddVariable", &LuaComponentType::AddVariable);
    LuaEmbedder::EmbedClassProperty<LuaComponentType>("ComponentType", "Name", &LuaComponentType::GetName, &LuaComponentType::SetName);
    LuaEmbedder::EmbedClassProperty<LuaComponentType>("ComponentType", "TableType", &LuaComponentType::GetTableType, &LuaComponentType::SetTableType);
  
    LuaEmbedder::AddInt("None", (int)ECSL::TableType::None, "TableType");
    LuaEmbedder::AddInt("Array", (int)ECSL::TableType::Array, "TableType");
    LuaEmbedder::AddInt("Map", (int)ECSL::TableType::Map, "TableType");
    
    LuaEmbedder::AddInt("Float", sizeof(float), "ByteSize");
    LuaEmbedder::AddInt("Int", sizeof(int), "ByteSize");
    LuaEmbedder::AddInt("Pointer", sizeof(void*), "ByteSize");
  }
  
  int LuaComponentType::GetName()
  {
    LuaEmbedder::PushString(m_name);
    return 1;
  }

  int LuaComponentType::SetName()
  {
    m_name = LuaEmbedder::PullString(1);
    return 0;
  }

  int LuaComponentType::GetTableType()
  {
    LuaEmbedder::PushInt(m_tableType);
    return 1;
  }

  int LuaComponentType::SetTableType()
  {
    m_tableType = (ECSL::TableType)LuaEmbedder::PullInt(1);
    return 0;
  }
  
  int LuaComponentType::AddVariable()
  {
    std::string variableName = LuaEmbedder::PullString(1);
    int byteSize = LuaEmbedder::PullInt(2);
    ECSL::ComponentVariable variable = ECSL::ComponentVariable(variableName, byteSize);
    m_variables.insert(std::pair<std::string, ECSL::ComponentVariable>(variableName, variable));
    return 0;
  }
  
  ECSL::ComponentType* LuaComponentType::CreateComponentType()
  {
    return new ECSL::ComponentType(m_name, m_tableType, m_variables);
  }
}