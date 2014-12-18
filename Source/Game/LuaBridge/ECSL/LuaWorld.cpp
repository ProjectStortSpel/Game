#include "LuaWorld.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "LuaSystem.h"
#include "LuaComponent.h"

namespace LuaBridge
{
  LuaWorld::LuaWorld()
    : ECSL::World(0, nullptr, nullptr) { }
  
  LuaWorld::~LuaWorld() { }
  
  void LuaWorld::Embed()
  {
    LuaEmbedder::EmbedClass<LuaWorld>("World");
    LuaEmbedder::EmbedClassFunction<LuaWorld>("World", "Update", &LuaWorld::Update);
    LuaEmbedder::EmbedClassFunction<LuaWorld>("World", "CreateNewEntity", &LuaWorld::CreateNewEntity);
    LuaEmbedder::EmbedClassFunction<LuaWorld>("World", "CreateComponentAndAddTo", &LuaWorld::CreateComponentAndAddTo);
    LuaEmbedder::EmbedClassFunction<LuaWorld>("World", "RemoveComponentFrom", &LuaWorld::RemoveComponentFrom);
    LuaEmbedder::EmbedClassFunction<LuaWorld>("World", "KillEntity", &LuaWorld::KillEntity);
    LuaEmbedder::EmbedClassFunction<LuaWorld>("World", "SetComponent", &LuaWorld::SetComponent);
    LuaEmbedder::EmbedClassFunction<LuaWorld>("World", "GetComponent", &LuaWorld::GetComponent);
  }
  
  int LuaWorld::Update()
  {
    float dt = LuaEmbedder::PullFloat(1);
    World::Update(dt);
    return 0;
  }
  
  int LuaWorld::CreateNewEntity()
  {
    if (LuaEmbedder::IsString(1))
      LuaEmbedder::PushInt((int)World::CreateNewEntity(LuaEmbedder::PullString(1)));
    else
      LuaEmbedder::PushInt((int)World::CreateNewEntity());
    return 1;
  }

  int LuaWorld::CreateComponentAndAddTo()
  {
    std::string componentType = LuaEmbedder::PullString(1);
    unsigned int entityId = (unsigned int)LuaEmbedder::PullInt(2);
    World::CreateComponentAndAddTo(componentType, entityId);
    return 0;
  }

  int LuaWorld::RemoveComponentFrom()
  {
    std::string componentType = LuaEmbedder::PullString(1);
    unsigned int entityId = (unsigned int)LuaEmbedder::PullInt(2);
    World::RemoveComponentFrom(componentType, entityId);
    return 0;
  }

  int LuaWorld::KillEntity()
  {
    unsigned int entityId = (unsigned int)LuaEmbedder::PullInt(1);
    World::KillEntity(entityId);
    return 0;
  }

  int LuaWorld::SetComponent()
  {
	  unsigned int id = (unsigned int)LuaEmbedder::PullInt(1);
	  std::string componentType = LuaEmbedder::PullString(2);
	  std::string variableName = LuaEmbedder::PullString(3);
	  void* data = 0;

	  if (LuaEmbedder::IsBool(4))
	  {
		  bool h = LuaEmbedder::PullBool(4);
		  data = &h;
		  World::SetComponent(id, componentType, variableName, data);
	  }
	  else if (LuaEmbedder::IsInt(4))
	  {
		  int h = LuaEmbedder::PullInt(4);
		  data = &h;
		  World::SetComponent(id, componentType, variableName, data);
	  }
	  else if (LuaEmbedder::IsFloat(4))
	  {
		  float h = LuaEmbedder::PullFloat(4);
		  data = &h;
		  World::SetComponent(id, componentType, variableName, data);
	  }
	  else if (LuaEmbedder::IsString(4))
	  {
		  std::string h = LuaEmbedder::PullString(4);
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
  
  int LuaWorld::GetComponent()
  {
	  ECSL::DataLocation dataLocation;
	  unsigned int entityId = (unsigned int)LuaEmbedder::PullInt(1);
	  std::string componentType = LuaEmbedder::PullString(2);
	  if (LuaEmbedder::IsInt(3))
	  {
		  unsigned int index = (unsigned int)LuaEmbedder::PullInt(3);
		  dataLocation = World::GetComponent(entityId, componentType, index);
	  }
	  else
	  {
		  std::string variableName = LuaEmbedder::PullString(3);
		  dataLocation = World::GetComponent(entityId, componentType, variableName);
	  }
	  LuaComponent* component = new LuaComponent(dataLocation, this, entityId, componentType);
	  LuaEmbedder::PushObject<LuaComponent>("Component", component, true);
	  return 1;
  }
}