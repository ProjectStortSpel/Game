#ifndef LUACOMPONENT_H
#define LUACOMPONENT_H

#include "ECSL/Framework/Components/ComponentType.h"

#include <string>
#include <map>

namespace LuaBridge
{
  class LuaComponent
  {
  public:
    LuaComponent();
    
    static void Embed();
    
    int GetName();
    int SetName();
    
    int GetTableType();
    int SetTableType();
    
    int AddVariable();
    
    ECSL::ComponentType GetComponentType();
    
  private:
    std::string m_name;
    ECSL::TableType m_tableType;
    std::map<std::string, ECSL::ComponentVariable> m_variables;
  };
}

#endif