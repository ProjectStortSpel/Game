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
}

void WorldCreator::AddComponentType(std::string _componentType)
{
	assert(!m_worldInitialized);
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