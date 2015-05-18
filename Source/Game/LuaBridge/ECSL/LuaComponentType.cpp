#include "LuaComponentType.h"
#include "LuaEmbedder/LuaEmbedder.h"

namespace LuaBridge
{
	LuaComponentType::LuaComponentType(lua_State* L) { m_byteOffset = 0; m_syncWithNetwork = false; }

  LuaComponentType::~LuaComponentType()
  {
    m_variables.clear();
	m_offsetToType.clear();
  }
  
  void LuaComponentType::Embed(lua_State* L)
  {
    LuaEmbedder::EmbedClass<LuaComponentType>(L, "ComponentType");
    LuaEmbedder::EmbedClassFunction<LuaComponentType>(L, "ComponentType", "GetName", &LuaComponentType::GetName);
    LuaEmbedder::EmbedClassFunction<LuaComponentType>(L, "ComponentType", "SetName", &LuaComponentType::SetName);
    LuaEmbedder::EmbedClassFunction<LuaComponentType>(L, "ComponentType", "GetTableType", &LuaComponentType::GetTableType);
    LuaEmbedder::EmbedClassFunction<LuaComponentType>(L, "ComponentType", "SetTableType", &LuaComponentType::SetTableType);
    LuaEmbedder::EmbedClassFunction<LuaComponentType>(L, "ComponentType", "AddVariable", &LuaComponentType::AddVariable);
	LuaEmbedder::EmbedClassProperty<LuaComponentType>(L, "ComponentType", "Name", &LuaComponentType::GetName, &LuaComponentType::SetName);
	LuaEmbedder::EmbedClassProperty<LuaComponentType>(L, "ComponentType", "SyncNetwork", &LuaComponentType::GetSyncWithNetwork, &LuaComponentType::SetSyncWithNetwork);
    LuaEmbedder::EmbedClassProperty<LuaComponentType>(L, "ComponentType", "TableType", &LuaComponentType::GetTableType, &LuaComponentType::SetTableType);
  
    LuaEmbedder::AddInt(L, "None", (int)ECSL::TableType::None, "TableType");
    LuaEmbedder::AddInt(L, "Array", (int)ECSL::TableType::Array, "TableType");
    LuaEmbedder::AddInt(L, "Map", (int)ECSL::TableType::Map, "TableType");
    
	LuaEmbedder::AddInt(L, "Float", ECSL::ComponentDataType::FLOAT, "ByteSize");
	LuaEmbedder::AddInt(L, "Int", ECSL::ComponentDataType::INT, "ByteSize");
	LuaEmbedder::AddInt(L, "Matrix", ECSL::ComponentDataType::MATRIX, "ByteSize");
	LuaEmbedder::AddInt(L, "Reference", ECSL::ComponentDataType::REFERENCE, "ByteSize");
	LuaEmbedder::AddInt(L, "Text", ECSL::ComponentDataType::TEXT, "ByteSize");
	LuaEmbedder::AddInt(L, "Bool", ECSL::ComponentDataType::BOOL, "ByteSize");
	LuaEmbedder::AddInt(L, "String", ECSL::ComponentDataType::STRING, "ByteSize");
  }
  
  int LuaComponentType::GetName(lua_State* L)
  {
	  LuaEmbedder::PushString(L, m_name);
	  return 1;
  }

  int LuaComponentType::SetName(lua_State* L)
  {
	  m_name = LuaEmbedder::PullString(L, 1);
	  return 0;
  }

  int LuaComponentType::GetSyncWithNetwork(lua_State* L)
  {
	  LuaEmbedder::PushBool(L, m_syncWithNetwork);
	  return 1;
  }

  int LuaComponentType::SetSyncWithNetwork(lua_State* L)
  {
	  m_syncWithNetwork = LuaEmbedder::PullBool(L, 1);
	  return 0;
  }

  int LuaComponentType::GetTableType(lua_State* L)
  {
    LuaEmbedder::PushInt(L, m_tableType);
    return 1;
  }

  int LuaComponentType::SetTableType(lua_State* L)
  {
    m_tableType = (ECSL::TableType)LuaEmbedder::PullInt(L, 1);
    return 0;
  }
  
  int LuaComponentType::AddVariable(lua_State* L)
  {
    std::string variableName = LuaEmbedder::PullString(L, 1);
	int variableDataType = LuaEmbedder::PullInt(L, 2);
	int byteSize = ECSL::GetByteSizeFromType(ECSL::ComponentDataType(variableDataType));

	ECSL::ComponentVariable variable = ECSL::ComponentVariable(variableName, byteSize);
	variable.SetOffset(m_byteOffset);
    m_variables.insert(std::pair<std::string, ECSL::ComponentVariable>(variableName, variable));

	m_offsetToType.insert(std::pair<unsigned int, ECSL::ComponentDataType>(m_byteOffset, ECSL::ComponentDataType(variableDataType)));
	m_byteOffset += byteSize;
    return 0;
  }
  
  ECSL::ComponentType* LuaComponentType::CreateComponentType()
  {
	  return new ECSL::ComponentType(m_name, m_tableType, m_variables, m_offsetToType, m_syncWithNetwork);
  }
}