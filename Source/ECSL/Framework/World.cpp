#include "World.h"
#include "../Managers/ComponentTypeManager.h"
using namespace ECSL;

World::World(unsigned int _entityCount, std::vector<SystemWorkGroup*>* _systemWorkGroups, std::vector<unsigned int>* _componentTypeIds)
{
	m_dataManager = new DataManager(_entityCount, _componentTypeIds);
	m_systemManager = new SystemManager(_systemWorkGroups);

	m_dataManager->InitializeTables();
	m_systemManager->InitializeSystems();

	m_simulation = new Simulation(m_dataManager, m_systemManager);
}

World::~World()
{
	delete(m_dataManager);
	delete(m_systemManager);
	delete(m_simulation);
}

void World::Update(float _dt)
{
	m_simulation->Update(_dt);
}