#include "LuaWorld.h"
#include "LuaSystem.h"
#include "LuaComponent.h"

namespace LuaBridge
{
  LuaWorld::LuaWorld(lua_State* L)
    : ECSL::World(0, nullptr, nullptr) { }
  
  LuaWorld::~LuaWorld() { }
  
  void LuaWorld::Embed(lua_State* L)
  {
    LuaEmbedder::EmbedClass<LuaWorld>(L, "World");
    LuaEmbedder::EmbedClassFunction<LuaWorld>(L, "World", "Update", &LuaWorld::Update);
    LuaEmbedder::EmbedClassFunction<LuaWorld>(L, "World", "CreateNewEntity", &LuaWorld::CreateNewEntity);
    LuaEmbedder::EmbedClassFunction<LuaWorld>(L, "World", "CreateComponentAndAddTo", &LuaWorld::CreateComponentAndAddTo);
    LuaEmbedder::EmbedClassFunction<LuaWorld>(L, "World", "RemoveComponentFrom", &LuaWorld::RemoveComponentFrom);
    LuaEmbedder::EmbedClassFunction<LuaWorld>(L, "World", "KillEntity", &LuaWorld::KillEntity);
    LuaEmbedder::EmbedClassFunction<LuaWorld>(L, "World", "SetComponent", &LuaWorld::SetComponent);
    LuaEmbedder::EmbedClassFunction<LuaWorld>(L, "World", "GetComponent", &LuaWorld::GetComponent);
	LuaEmbedder::EmbedClassFunction<LuaWorld>(L, "World", "EntityHasComponent", &LuaWorld::HasComponent);
  }
  
  int LuaWorld::Update(lua_State* L)
  {
    float dt = LuaEmbedder::PullFloat(L, 1);
    World::Update(dt);
    return 0;
  }
  
  int LuaWorld::CreateNewEntity(lua_State* L)
  {
    if (LuaEmbedder::IsString(L, 1))
      LuaEmbedder::PushInt(L, (int)World::CreateNewEntity(LuaEmbedder::PullString(L, 1)));
    else
      LuaEmbedder::PushInt(L, (int)World::CreateNewEntity());
    return 1;
  }

  int LuaWorld::CreateComponentAndAddTo(lua_State* L)
  {
    std::string componentType = LuaEmbedder::PullString(L, 1);
    unsigned int entityId = (unsigned int)LuaEmbedder::PullInt(L, 2);
    World::CreateComponentAndAddTo(componentType, entityId);
    return 0;
  }

  int LuaWorld::RemoveComponentFrom(lua_State* L)
  {
    std::string componentType = LuaEmbedder::PullString(L, 1);
    unsigned int entityId = (unsigned int)LuaEmbedder::PullInt(L, 2);
    World::RemoveComponentFrom(componentType, entityId);
    return 0;
  }

  int LuaWorld::KillEntity(lua_State* L)
  {
    unsigned int entityId = (unsigned int)LuaEmbedder::PullInt(L, 1);
    World::KillEntity(entityId);
    return 0;
  }

  int LuaWorld::SetComponent(lua_State* L)
  {
	  unsigned int id = (unsigned int)LuaEmbedder::PullInt(L, 1);
	  std::string componentType = LuaEmbedder::PullString(L, 2);
	  std::string variableName = LuaEmbedder::PullString(L, 3);
	  void* data = 0;

	  if (LuaEmbedder::IsBool(L, 4))
	  {
		  bool h = LuaEmbedder::PullBool(L, 4);
		  data = &h;
		  World::SetComponent(id, componentType, variableName, data);
	  }
	  else if (LuaEmbedder::IsInt(L, 4))
	  {
		  int h = LuaEmbedder::PullInt(L, 4);
		  data = &h;
		  World::SetComponent(id, componentType, variableName, data);
	  }
	  else if (LuaEmbedder::IsFloat(L, 4))
	  {
		  float h = LuaEmbedder::PullFloat(L, 4);
		  data = &h;
		  World::SetComponent(id, componentType, variableName, data);
	  }
	  else if (LuaEmbedder::IsString(L, 4))
	  {
		  std::string h = LuaEmbedder::PullString(L, 4);
		  char* tmp = new char[h.size() + 1];

		  for (int i = 0; i < h.size(); i++)
			  tmp[i] = h[i];
		  tmp[h.size()] = '\0';

		  World::SetComponent(id, componentType, variableName, tmp);

		  delete tmp;
	  }
	  else
		  return 0;

	  

	  return 0;
  }
  
  int LuaWorld::GetComponent(lua_State* L)
  {
	  ECSL::DataLocation dataLocation;
	  unsigned int entityId = (unsigned int)LuaEmbedder::PullInt(L, 1);
	  std::string componentType = LuaEmbedder::PullString(L, 2);
	  if (LuaEmbedder::IsInt(L, 3))
	  {
		  unsigned int index = (unsigned int)LuaEmbedder::PullInt(L, 3);
		  dataLocation = World::GetComponent(entityId, componentType, index);
	  }
	  else
	  {
		  std::string variableName = LuaEmbedder::PullString(L, 3);
		  dataLocation = World::GetComponent(entityId, componentType, variableName);
	  }
	  LuaComponent* component = new LuaComponent(dataLocation, this, entityId, componentType);
	  LuaEmbedder::PushObject<LuaComponent>(L, "Component", component, true);
	  return 1;
  }

  int LuaWorld::HasComponent(lua_State* L)
  {
	  std::vector<unsigned int> components;
	  LuaEmbedder::PushBool(L, World::HasComponent((unsigned int)LuaEmbedder::PullInt(L, 1), LuaEmbedder::PullString(L, 2)));
	  return 1;
  }
}