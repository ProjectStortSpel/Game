#include "DataManager.h"

#include <assert.h>
#include "ECSL/Framework/Common/ContainerHelper.h"
#include "ECSL/Managers/ComponentTypeManager.h"
#include "Tables/ComponentTable.h"
#include "Tables/DataArray.h"
#include "Tables/DataMap.h"

using namespace ECSL;

DataManager::DataManager(unsigned int _entityCount, std::vector<unsigned int>* _componentTypeIds)
:	m_entityCount(_entityCount), m_componentTypeIds(_componentTypeIds),
	m_entitiesToBeRemoved(new std::vector<unsigned int>()), m_changedEntities(new std::vector<unsigned int>()),
	m_componentsToBeRemoved(new std::map<unsigned int, std::vector<unsigned int>>())
{
}

DataManager::~DataManager()
{
	for (auto it = m_componentTables->begin(); it != m_componentTables->end(); ++it)
	{
		delete *it;
	}
	m_componentTables->clear();
		
	delete m_componentTables;
	delete m_entityTable;
	delete m_componentTypeIds;
	delete m_entitiesToBeRemoved;
	delete m_changedEntities;
	delete m_componentsToBeRemoved;
}

void DataManager::InitializeTables()
{
	m_componentTables = new std::vector<ComponentTable*>(ComponentTypeManager::GetInstance().GetComponentTypeCount());
	m_entityTable = new EntityTable(m_entityCount, (unsigned int)m_componentTypeIds->size());

	for (unsigned int n = 0; n < m_componentTypeIds->size(); ++n)
	{
		unsigned int componentTypeId = m_componentTypeIds->at(n);
		ComponentType* componentType = ComponentTypeManager::GetInstance().GetComponentType(componentTypeId);

		/* Couldn't find component type. Is the type added and is it spelled correctly? */
		assert(componentType);

		switch (componentType->GetTableType())
		{
		case TableType::Array:
			m_componentTables->at(componentTypeId) = new ComponentTable(new DataArray(m_entityCount, componentType->GetByteSize()), componentTypeId);
			break;
		case TableType::Map:
			m_componentTables->at(componentTypeId) = new ComponentTable(new DataMap(componentType->GetByteSize()), componentTypeId);
			break;
		default:
			printf("ERROR: Invalid Table Type! (ID %d)\n", m_componentTypeIds->at(n));
			break;
		}
	}
}

unsigned int DataManager::CreateNewEntity()
{
	return m_entityTable->GenerateNewEntityId();
}

void DataManager::RemoveEntity(unsigned int _entityId)
{
	/* Add entity to list */
	m_entitiesToBeRemoved->push_back(_entityId);

	std::vector<unsigned int> components;
	m_entityTable->GetEntityComponents(components, _entityId);
	/* Remove every component from the entity */
	for (unsigned int componentTypeId : components)
		RemoveComponentFrom(componentTypeId, _entityId);

	m_entityTable->ClearEntityData(_entityId);
}

void DataManager::CreateComponentAndAddTo(const std::string& _componentType, unsigned int _entityId)
{
	CreateComponentAndAddTo(ComponentTypeManager::GetInstance().GetTableId(_componentType), _entityId);
}

void DataManager::CreateComponentAndAddTo(unsigned int _componentTypeId, unsigned int _entityId)
{
	/* Add entity to lists */
	m_changedEntities->push_back(_entityId);
	m_entityTable->AddComponentTo(_entityId, _componentTypeId);
}

void DataManager::RemoveComponentFrom(const std::string& _componentType, unsigned int _entityId)
{
	RemoveComponentFrom(ComponentTypeManager::GetInstance().GetTableId(_componentType), _entityId);
}

void DataManager::RemoveComponentFrom(unsigned int _componentTypeId, unsigned int _entityId)
{
	/* Add entity and component to lists */
	ContainerHelper::TryAddKey<unsigned int, std::vector<unsigned int>>(_entityId, *m_componentsToBeRemoved);
	(*m_componentsToBeRemoved)[_entityId].push_back(_componentTypeId);
	m_changedEntities->push_back(_entityId);

	m_entityTable->RemoveComponentFrom(_entityId, _componentTypeId);
}

void DataManager::ClearComponentData()
{
	for (auto entity = m_componentsToBeRemoved->begin(); entity != m_componentsToBeRemoved->end(); ++entity)
	{
		for (auto componentTypeId : entity->second)
		{
			m_componentTables->at(componentTypeId)->ClearComponent(entity->first);
		}
	}
}

void DataManager::RecycleEntityIds()
{
	for (auto entityId : *m_entitiesToBeRemoved)
	{
		m_entityTable->AddOldEntityId(entityId);
	}
}

void DataManager::ClearChangeLists()
{
	m_entitiesToBeRemoved->clear();
	m_changedEntities->clear();
	m_componentsToBeRemoved->clear();
}

unsigned int DataManager::GetMemoryAllocated()
{
	unsigned int memoryUsage = 0;

	for (auto dataTable : *m_componentTables)
		memoryUsage += (dataTable->GetMemoryAllocated());

	memoryUsage += sizeof(DataManager);

	float megabytes = ((float)memoryUsage) / (1024.f * 1024.f);
 	return (unsigned int)megabytes;
}