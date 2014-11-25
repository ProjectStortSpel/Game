#include "WorldCreator.h"

#include <assert.h>

using namespace ECSL;

WorldCreator::WorldCreator() : m_worldInitialized(false)
{
	m_systemWorkGroups = new std::vector<SystemWorkGroup*>();
	m_componentTypeIds = new std::vector<int>();
}

WorldCreator::~WorldCreator()
{
	if (!m_worldInitialized)
	{
		for (auto it = m_systemWorkGroups->begin(); it != m_systemWorkGroups->end(); ++it)
		{
			delete m_systemWorkGroups->back();
			m_systemWorkGroups->pop_back();
		}

		delete(m_systemWorkGroups);
	}
}

void WorldCreator::AddComponentType(std::string _componentType)
{
	assert(!m_worldInitialized);

	//ComponentManager::GetTableId(_componentType);

	int a = 2;
}

void WorldCreator::AddSystemGroup()
{
	assert(!m_worldInitialized);

	m_systemWorkGroups->push_back(new SystemWorkGroup());
}

World* WorldCreator::InitializeWorld(unsigned int _entityCount)
{
	assert(!m_worldInitialized);

	World* world = new World(_entityCount, m_systemWorkGroups, m_componentTypeIds);
	m_worldInitialized = true;
	return world;
}