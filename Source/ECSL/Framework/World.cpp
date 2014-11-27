#include "World.h"
#include "../Managers/ComponentTypeManager.h"
using namespace ECSL;

World::World(unsigned int _entityCount, std::vector<SystemWorkGroup*>* _systemWorkGroups, std::vector<int>* _componentTypeIds)
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

void CreateComponentAndAddTo(std::string& _componentType, unsigned int _id)
{

}