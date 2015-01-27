#include "LuaEntityTemplateManager.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "LuaEntityTemplate.h"
#include "ECSL/Managers/EntityTemplateManager.h"

namespace LuaBridge
{
  namespace LuaEntityTemplateManager
  {
    int AddTemplate(lua_State* L);
    
    void Embed(lua_State* L)
    {
      LuaEmbedder::AddFunction(L, "AddTemplate", &AddTemplate, "EntityTemplateManager");
    }
    
    int AddTemplate(lua_State* L)
    {
      LuaEntityTemplate* entityTemplate = LuaEmbedder::PullObject<LuaEntityTemplate>(L, "EntityTemplate", 2);
      ECSL::EntityTemplateManager::GetInstance().AddTemplate(entityTemplate->CreateEntityTemplate());
      return 0;
    }
  }
}