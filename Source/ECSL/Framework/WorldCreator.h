#ifndef WORLDCREATOR_H
#define WORLDCREATOR_H

#include <SDL/SDL.h>
#include <assert.h>
#include <vector>

#include "World.h"
#include "Components/ComponentType.h"
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

		// Adds a new system group. Every system in the same group will be executed simultaneously
		void AddSystemGroup();

		/// Adds a system to a new group. Every system in the same group will be executed simultaneously
		template<typename SystemType, typename... Args>
		void AddSystemToNewGroup(Args&&... args);
		// Adds a system to the most recently added group. Every system in the same group will be executed simultaneously
		template<typename SystemType, typename... Args>
		void AddSystemToCurrentGroup(Args&&... args);
		// Adds a Lua system to the most recently added group. Every system in the same group will be executed simultaneously
		void AddLuaSystemToCurrentGroup(System* _system);

		World* CreateWorld(unsigned int _entityCount);

		bool IsWorldInitialized() const { return m_worldInitialized; }

	private:

		bool m_worldInitialized;
		std::vector<SystemWorkGroup*>* m_systemWorkGroups;
		std::vector<unsigned int>* m_componentTypeIds;

		inline bool IsIdAdded(unsigned int _id);
	};
	//template<typename ComponentType>
	//void WorldCreator::AddComponentType()
	//{
	//	ComponentTypeManager::GetInstance().AddComponentType<ComponentType>();
	//	int newId = ComponentManager::GetTableId<ComponentType>();

	//	assert(IsIdAdded(newId));

	//	m_componentTypeIds->push_back(newId);
	//}

	template<typename SystemType, typename... Args>
	void WorldCreator::AddSystemToNewGroup(Args&&... args)
	{
		m_systemWorkGroups->push_back(new SystemWorkGroup(new SystemType{ std::forward<Args>(args)... }));
	}

	template<typename SystemType, typename... Args>
	void WorldCreator::AddSystemToCurrentGroup(Args&&... args)
	{
		m_systemWorkGroups->at(m_systemWorkGroups->size() - 1)->AddSystem(new SystemType{ std::forward<Args>(args)... });
	}
}
#endif