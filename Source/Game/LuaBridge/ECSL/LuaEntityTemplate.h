#ifndef LUAENTITYTEMPLATE_H
#define LUAENTITYTEMPLATE_H

#include "ECSL/Framework/Templates/EntityTemplate.h"

#include <string>
#include <map>
#include <vector>

namespace LuaBridge
{
  class LuaEntityTemplate
  {
  public:
    LuaEntityTemplate();
    ~LuaEntityTemplate();
    
    static void Embed();
    
    ECSL::EntityTemplate* CreateEntityTemplate();
    
  private:
    int GetName();
    int SetName();
    
    int AddComponent();
    
    int SetFloat();
    int SetFloat2();
    int SetFloat3();
	int SetFloat4();
	int SetFloat5();
    
    int SetInt();
    int SetInt2();
    int SetInt3();
    int SetInt4();
    
    int SetString();
    
    int SetModel();
    
    int SetPointlight();
    
  private:
    std::string m_name;
    std::map<std::string, std::vector<ECSL::TemplateEntry>> m_components;
  };
}

#endif