#include "World.h"
#include "../Managers/ComponentTypeManager.h"
using namespace ECSL;

World::World(unsigned int _entityCount, std::vector<SystemWorkGroup*>* _systemWorkGroups, std::vector<unsigned int>* _componentTypeIds)
{
	m_dataManager = new DataManager(_entityCount, _componentTypeIds);
	m_systemManager = new SystemManager(m_dataManager, _systemWorkGroups);
	m_simulation = new Simulation(m_dataManager, m_systemManager);

	m_dataManager->InitializeTables();
	m_systemManager->InitializeSystems();
}

World::~World()
{
	delete(m_simulation);
	delete(m_dataManager);
	delete(m_systemManager);
}

void World::Update(float _dt)
{
	m_simulation->Update(_dt);
}

unsigned int World::CreateNewEntity()
{
	++m_activeEntities;
	return m_dataManager->CreateNewEntity();
}

void World::CreateComponentAndAddTo(const std::string& _componentType, unsigned int _entityId)
{
	m_dataManager->CreateComponentAndAddTo(_componentType, _entityId);
}

void World::RemoveComponentFrom(const std::string& _componentType, unsigned int _entityId)
{
	m_dataManager->RemoveComponentFrom(_componentType, _entityId);
}

DataLocation World::GetComponent(unsigned int _entityId, const std::string& _componentType, const std::string& _variableName)
{
	return m_dataManager->GetComponentTable(_componentType)->GetComponent(_entityId, _variableName);
}

void World::SetComponent(unsigned int _entityId, const std::string& _componentType, const std::string& _variableName, void* _data)
{
	ComponentTable* componentTable = m_dataManager->GetComponentTable(ComponentTypeManager::GetInstance().GetTableId(_componentType));
	componentTable->SetComponent(_entityId, _variableName, _data);
}

void World::KillEntity(unsigned int _entityId)
{
	--m_activeEntities;
	m_dataManager->RemoveEntity(_entityId);
}

DataLocation World::GetComponent(unsigned int _entityId, const std::string& _componentType, const std::string& _variableName)
{
	return m_dataManager->GetComponentTable(_componentType)->GetComponent(_entityId, _variableName);
}