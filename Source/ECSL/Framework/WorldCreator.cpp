#include "WorldCreator.h"

using namespace ECSL;

WorldCreator::WorldCreator() : m_worldInitialized(false)
{
	m_systemWorkGroups = new std::vector<SystemWorkGroup*>();
	m_componentTypeIds = new std::vector<unsigned int>();
}

WorldCreator::~WorldCreator()
{
}

void WorldCreator::AddComponentType(const std::string& _componentType)
{
	assert(!m_worldInitialized);

	unsigned int tableId = ComponentTypeManager::GetInstance().GetTableId(_componentType);

	assert(!IsIdAdded(tableId));

	m_componentTypeIds->push_back(tableId);
}

void WorldCreator::AddSystemGroup()
{
	assert(!m_worldInitialized);

	m_systemWorkGroups->push_back(new SystemWorkGroup());
}

World* WorldCreator::CreateWorld(unsigned int _entityCount)
{
	assert(!m_worldInitialized);

	World* world = new World(_entityCount, m_systemWorkGroups, m_componentTypeIds);
	m_worldInitialized = true;
	return world;
}

bool WorldCreator::IsIdAdded(unsigned int _id)
{
	//	Check if the component type is already added
	for (unsigned int i = 0; i < m_componentTypeIds->size(); ++i)
		if (_id == m_componentTypeIds->at(i))
			return true;
	return false;
}