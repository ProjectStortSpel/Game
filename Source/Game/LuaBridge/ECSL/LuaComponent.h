#ifndef LUACOMPONENT_H
#define LUACOMPONENT_H

#include "ECSL/Framework/Components/Tables/DataTable.h"

#include <string>
#include <map>

namespace LuaBridge
{
  class LuaComponent
  {
  public:
    LuaComponent();
    ~LuaComponent();
    
    static void Embed();
    
    void SetDataLocation(ECSL::DataLocation dataLocation);
    
  private:
    int GetFloat();
    int SetFloat();
    int GetInt();
    int SetInt();
    int GetBool();
    int SetBool();
    int GetString();
    int SetString();
    
  private:
    ECSL::DataLocation m_dataLocation;
  };
}

#endif