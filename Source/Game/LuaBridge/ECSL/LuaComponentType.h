#ifndef LUACOMPONENTTYPE_H
#define LUACOMPONENTTYPE_H

#include "ECSL/Framework/Components/ComponentType.h"

#include <string>
#include <map>

namespace LuaBridge
{
  class LuaComponentType
  {
  public:
    LuaComponentType();
    ~LuaComponentType();
    
    static void Embed();
    
    ECSL::ComponentType* CreateComponentType();
    
  private:
    int GetName();
    int SetName();
    
    int GetTableType();
    int SetTableType();
    
    int AddVariable();
    
  private:
    std::string m_name;
    ECSL::TableType m_tableType;
    std::map<std::string, ECSL::ComponentVariable> m_variables;
  };
}

#endif