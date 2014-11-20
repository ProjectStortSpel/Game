#include "World.h"

using namespace ECSL;

World::World(unsigned int _entityCount, std::vector<SystemWorkGroup*>* _systemWorkGroups, std::vector<int>* _componentTypeIds)
{
	m_dataManager = new DataManager(_entityCount);
	m_systemManager = new SystemManager(_systemWorkGroups);

	m_dataManager->InitializeTables();
}

World::~World()
{
	delete(m_dataManager);
	delete(m_systemManager);
}