#include "LuaEntityTemplateManager.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "LuaEntityTemplate.h"
#include "ECSL/Managers/EntityTemplateManager.h"

namespace LuaBridge
{
  namespace LuaEntityTemplateManager
  {
    lua_State* g_luaState;
      
    int AddTemplate(lua_State* L);
    
    void Embed(lua_State* L)
    {
      LuaEmbedder::AddFunction(L, "AddTemplate", &AddTemplate, "EntityTemplateManager");
    }
    
    int AddTemplate(lua_State* L)
    {
      lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
      if (g_luaState == parent)
      {
          LuaEntityTemplate* entityTemplate = LuaEmbedder::PullObject<LuaEntityTemplate>(L, "EntityTemplate", 2);
          ECSL::EntityTemplateManager::GetInstance().AddTemplate(entityTemplate->CreateEntityTemplate());
      }
      return 0;
    }
      
    void SetLuaState(lua_State* L)
    {
        g_luaState = L;
    }
  }
}