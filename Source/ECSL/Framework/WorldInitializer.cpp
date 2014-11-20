#include "WorldInitializer.h"

#include <assert.h>

using namespace ECSL;

WorldInitializer::WorldInitializer() : m_worldInitialized(false)
{
	m_systemWorkGroups = new std::vector<SystemWorkGroup*>();
	m_componentTypeIds = new std::vector<int>();
}

WorldInitializer::~WorldInitializer()
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

void WorldInitializer::AddComponentType(std::string _componentType)
{

}

void WorldInitializer::AddSystemGroup()
{
	m_systemWorkGroups->push_back(new SystemWorkGroup());
}

World* WorldInitializer::InitializeWorld(unsigned int _entityCount)
{
	assert(!m_worldInitialized);

	World* world = new World(_entityCount, m_systemWorkGroups, m_componentTypeIds);

	m_worldInitialized = true;

	return world;
}