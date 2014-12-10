#include "LuaEntityTemplateManager.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "LuaEntityTemplate.h"
#include "ECSL/Managers/EntityTemplateManager.h"

namespace LuaBridge
{
  namespace LuaEntityTemplateManager
  {
    int AddTemplate();
    
    void Embed()
    {
      LuaEmbedder::AddFunction("AddTemplate", &AddTemplate, "EntityTemplateManager");
    }
    
    int AddTemplate()
    {
      LuaEntityTemplate* entityTemplate = LuaEmbedder::PullObject<LuaEntityTemplate>("EntityTemplate", 2);
      ECSL::EntityTemplateManager::GetInstance().AddTemplate(*entityTemplate->CreateEntityTemplate());
      return 0;
    }
  }
}