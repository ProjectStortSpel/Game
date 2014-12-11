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
    int GetFloat2();
    int GetFloat3();
    int GetFloat4();
    int SetFloat();
    int SetFloat2();
    int SetFloat3();
    int SetFloat4();
    
    int GetInt();
    int GetInt2();
    int GetInt3();
    int GetInt4();
    int SetInt();
    int SetInt2();
    int SetInt3();
    int SetInt4();
    
    int GetBool();
    int SetBool();
    
    int GetString();
    int SetString();
    
    int SetModel();
    
  private:
    ECSL::DataLocation m_dataLocation;
  };
}

#endif