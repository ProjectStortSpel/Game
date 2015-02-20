#ifndef LUAENTITYTEMPLATE_H
#define LUAENTITYTEMPLATE_H

#include "ECSL/Framework/Templates/EntityTemplate.h"
#include <LuaEmbedder/LuaEmbedder.h>

#include <string>
#include <map>
#include <vector>

namespace LuaBridge
{
  class LuaEntityTemplate
  {
  public:
    LuaEntityTemplate(lua_State* L);
    ~LuaEntityTemplate();
    
    static void Embed(lua_State* L);
    
    ECSL::EntityTemplate* CreateEntityTemplate();
    
  private:
    int GetName(lua_State* L);
    int SetName(lua_State* L);
    
    int AddComponent(lua_State* L);
    
    int SetFloat(lua_State* L);
    int SetFloat2(lua_State* L);
    int SetFloat3(lua_State* L);
	int SetFloat4(lua_State* L);
	int SetFloat5(lua_State* L);
    
    int SetInt(lua_State* L);
    int SetInt2(lua_State* L);
    int SetInt3(lua_State* L);
    int SetInt4(lua_State* L);
    
    int SetString(lua_State* L);

	int SetBool(lua_State* L);
    
    int SetModel(lua_State* L);
    
    int SetPointlight(lua_State* L);
    
  private:
    std::string m_name;
    std::map<std::string, std::vector<ECSL::TemplateEntry*>> m_components;
  };
}

#endif