#include "World.h"
#include "../Managers/ComponentManager.h"
using namespace ECSL;

World::World(unsigned int _entityCount, std::vector<SystemWorkGroup*>* _systemWorkGroups, std::vector<int>* _componentTypeIds)
{
	m_dataManager = new DataManager(_entityCount);
	m_systemManager = new SystemManager(_systemWorkGroups);

	for (int n = 0; n < _componentTypeIds->size(); ++n)
		m_dataManager->AddComponentType(_componentTypeIds->at(n));

	m_dataManager->InitializeTables();
}

World::~World()
{
	delete(m_dataManager);
	delete(m_systemManager);
}