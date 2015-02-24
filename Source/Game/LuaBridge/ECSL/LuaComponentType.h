#ifndef LUACOMPONENTTYPE_H
#define LUACOMPONENTTYPE_H

#include "ECSL/Framework/Components/ComponentType.h"
#include <LuaEmbedder/LuaEmbedder.h>

#include <string>
#include <map>

namespace LuaBridge
{
  class LuaComponentType
  {
  public:
    LuaComponentType(lua_State* L);
    ~LuaComponentType();
    
    static void Embed(lua_State* L);
    
    ECSL::ComponentType* CreateComponentType();
    
  private:
    int GetName(lua_State* L);
    int SetName(lua_State* L);

	int GetSyncWithNetwork(lua_State* L);
	int SetSyncWithNetwork(lua_State* L);
    
    int GetTableType(lua_State* L);
    int SetTableType(lua_State* L);
    
    int AddVariable(lua_State* L);
    
  private:
    std::string m_name;
	bool m_syncWithNetwork;
    ECSL::TableType m_tableType;
	std::map<std::string, ECSL::ComponentVariable> m_variables;
	std::map<unsigned int, ECSL::ComponentDataType> m_offsetToType;
	int m_byteOffset;
  };
}

#endif