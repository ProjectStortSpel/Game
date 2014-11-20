#ifndef WORLDINITIALIZER_H
#define WORLDINITIALIZER_H

#include <SDL/SDL.h>
#include <vector>

#include "World.h"
#include "Systems/SystemWorkGroup.h"

namespace ECSL
{
	// Used for easier creation of a World object
	class DECLSPEC WorldInitializer
	{
	private:
		bool m_worldInitialized;
		std::vector<SystemWorkGroup*>* m_systemWorkGroups;
		std::vector<int>* m_componentTypeIds;

	public:
		WorldInitializer();
		~WorldInitializer();

		template<typename ComponentType>
		void AddComponentType();
		void AddComponentType(std::string _componentType);

		/// Adds a system to a new group
		template<typename SystemType>
		void AddSystem();
		// Adds a system to the most recently added group
		template<typename SystemType>
		void AddSystemToGroup();
		// Adds a new system group
		void AddSystemGroup();

		World* InitializeWorld(unsigned int _entityCount);

		bool IsWorldInitialized() const { return m_worldInitialized; }
	};
	
	template<typename ComponentType>
	void WorldInitializer::AddComponentType()
	{
		
	}

	template<typename SystemType>
	void WorldInitializer::AddSystem()
	{
		m_systemWorkGroups->push_back(new SystemGroup(new SystemType()));
	}

	template<typename SystemType>
	void WorldInitializer::AddSystemToGroup()
	{
		m_systemWorkGroups->at(m_systemWorkGroups->size() - 1)->AddSystem<SystemType>();
	}
}
#endif