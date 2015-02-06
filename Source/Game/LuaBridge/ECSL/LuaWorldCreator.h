#ifndef LUAWORLDCREATOR_H
#define LUAWORLDCREATOR_H

#include "ECSL/Framework/WorldCreator.h"
#include "LuaSystem.h"
#include <LuaEmbedder/LuaEmbedder.h>
#include <vector>

namespace LuaBridge
{
  class LuaWorldCreator : public ECSL::WorldCreator
  {
  public:
    LuaWorldCreator();
    LuaWorldCreator(lua_State* L);
    
    static void Embed(lua_State* L);
    
    std::vector<LuaSystem*>* GetSystemsAdded() { return &m_systemsAdded; }
    
    template<typename ComponentType>
    void AddComponentType() { ECSL::WorldCreator::AddComponentType<ComponentType>(); }
    void AddComponentType(const std::string& _componentType) { ECSL::WorldCreator::AddComponentType(_componentType); }
    void AddComponentType(ECSL::ComponentType& _componentType) { ECSL::WorldCreator::AddComponentType(_componentType); }

    void AddSystemGroup() { ECSL::WorldCreator::AddSystemGroup(); }

	template<typename SystemType, typename... Args>
	void AddSystemToNewGroup(Args&&... args) { ECSL::WorldCreator::AddSystemToNewGroup<SystemType>(std::forward<Args>(args)...); }
	template<typename SystemType, typename... Args>
	void AddSystemToCurrentGroup(Args&&... args) { ECSL::WorldCreator::AddSystemToCurrentGroup<SystemType>(std::forward<Args>(args)...); }
    void AddLuaSystemToCurrentGroup(ECSL::System* _system) { ECSL::WorldCreator::AddLuaSystemToCurrentGroup(_system); }

    ECSL::World* CreateWorld(unsigned int _entityCount) { return ECSL::WorldCreator::CreateWorld(_entityCount); }

    bool IsWorldInitialized() const { return ECSL::WorldCreator::IsWorldInitialized(); }
    
      void SkipComponentTypesAndTemplates(bool _value) { m_skipComponentTypes = _value; }
    
  private:
    int AddComponentType(lua_State* L);
    int AddSystemToCurrentGroup(lua_State* L);
    int AddSystemGroup(lua_State* L);
    int CreateWorld(lua_State* L);
    int IsWorldInitialized(lua_State* L);
    
  private:
    std::vector<LuaSystem*> m_systemsAdded;
      
      bool m_skipComponentTypes;
  };
}

#endif