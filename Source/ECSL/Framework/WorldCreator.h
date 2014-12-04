#ifndef WORLDCREATOR_H
#define WORLDCREATOR_H

#include <SDL/SDL.h>
#include <assert.h>
#include <vector>

#include "World.h"
#include "Components/ComponentType.h"
#include "Systems/SystemIdManager.h"
#include "Systems/SystemWorkGroup.h"
#include "ECSL/Managers/ComponentTypeManager.h"

namespace ECSL
{
	// Used for easier creation of a World object
	class DECLSPEC WorldCreator
	{
	public:
		WorldCreator();
		~WorldCreator();

		template<typename ComponentType>
		void AddComponentType();
		void AddComponentType(const std::string& _componentType);
		void AddComponentType(ComponentType& _componentType);

		// Adds a new system group
		void AddSystemGroup();

		/// Adds a system to a new group
		template<typename SystemType>
		void AddSystemToNewGroup();
		// Adds a system to the most recently added group
		template<typename SystemType>
		void AddSystemToCurrentGroup();
		// Adds a Lua system to the most recently added group
		void AddLuaSystemToCurrentGroup(System* _system);

		World* CreateWorld(unsigned int _entityCount);

		bool IsWorldInitialized() const { return m_worldInitialized; }

	private:
		static int m_nextSystemId;
		bool m_worldInitialized;
		std::vector<SystemWorkGroup*>* m_systemWorkGroups;
		std::vector<unsigned int>* m_componentTypeIds;

		inline bool IsIdAdded(unsigned int _id);
	};
	int WorldCreator::m_nextSystemId = -1;
	//template<typename ComponentType>
	//void WorldCreator::AddComponentType()
	//{
	//	ComponentTypeManager::GetInstance().AddComponentType<ComponentType>();
	//	int newId = ComponentManager::GetTableId<ComponentType>();

	//	assert(IsIdAdded(newId));

	//	m_componentTypeIds->push_back(newId);
	//}

	template<typename SystemType>
	void WorldCreator::AddSystemToNewGroup()
	{
		System* system = new SystemType();
		system->SetId(SystemIdManager::GetInstance().GetSystemId<SystemType>());
		m_systemWorkGroups->push_back(new SystemWorkGroup(system));
	}

	template<typename SystemType>
	void WorldCreator::AddSystemToCurrentGroup()
	{
		System* system = new SystemType();
		system->SetId(SystemIdManager::GetInstance().GetSystemId<SystemType>());
		m_systemWorkGroups->at(m_systemWorkGroups->size() - 1)->AddSystem(system);
	}
}
#endif