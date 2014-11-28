#include "World.h"
#include "../Managers/ComponentTypeManager.h"
using namespace ECSL;

World::World(unsigned int _entityCount, std::vector<SystemWorkGroup*>* _systemWorkGroups, std::vector<unsigned int>* _componentTypeIds)
{
	m_dataManager = new DataManager(_entityCount, _componentTypeIds);
	m_systemManager = new SystemManager(_systemWorkGroups);

	m_dataManager->InitializeTables();
	m_systemManager->InitializeSystems();
}

World::~World()
{
	delete(m_dataManager);
	delete(m_systemManager);
}

unsigned int World::CreateNewEntity()
{
	return m_dataManager->CreateNewEntity();
}

void World::CreateComponentAndAddTo(const std::string& _componentType, unsigned int _id)
{
	m_dataManager->CreateComponentAndAddTo(_componentType, _id);
}

void World::KillEntity(unsigned int _id)
{
	m_dataManager->KillEntity(_id);
}