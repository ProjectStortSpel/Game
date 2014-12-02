#include "DataManager.h"

#include <assert.h>
#include "ECSL/Framework/Common/ContainerHelper.h"
#include "ECSL/Managers/ComponentTypeManager.h"
#include "Tables/DataArray.h"
#include "Tables/DataMap.h"

using namespace ECSL;

DataManager::DataManager(unsigned int _entityCount, std::vector<unsigned int>* _componentTypeIds)
:	m_entityCount(_entityCount), m_componentTypeIds(_componentTypeIds),
	m_entitiesToBeAdded(new std::vector<unsigned int>()), m_entitiesToBeRemoved(new std::vector<unsigned int>()),
	m_componentsToBeRemoved(new std::map<unsigned int, std::vector<unsigned int>>()), m_changedEntities(new std::vector<unsigned int>())
{
}

DataManager::~DataManager()
{
	for (auto it = m_componentTables->begin(); it != m_componentTables->end(); ++it)
	{
		delete it->second;
	}
	m_componentTables->clear();
		
	delete m_componentTables;
	delete m_entityTable;
	delete m_componentTypeIds;
	delete m_entitiesToBeAdded;
	delete m_entitiesToBeRemoved;
	delete m_changedEntities;
	delete m_componentsToBeRemoved;
}

void DataManager::InitializeTables()
{
	m_componentTables = new std::map<unsigned int, DataTable*>();
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
			m_componentTables->insert(std::pair<unsigned int, DataTable*>(componentTypeId, new DataArray(m_entityCount, componentType->GetByteSize())));
			break;
		case TableType::Map:
			m_componentTables->insert(std::pair<unsigned int, DataTable*>(componentTypeId, new DataMap(componentType->GetByteSize())));
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
	ContainerHelper::AddUniqueElement(_entityId, *m_entitiesToBeRemoved);

	std::vector<unsigned int> components;
	BitSet::BitSetConverter::BitSetToArray(components, m_entityTable->GetEntityComponents(_entityId), ComponentTypeManager::GetInstance().GetComponentTypeCount());
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
	ContainerHelper::AddUniqueElement(_entityId, *m_entitiesToBeAdded);
	ContainerHelper::AddUniqueElement(_entityId, *m_changedEntities);

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
	ContainerHelper::AddUniqueElement(_entityId, *m_changedEntities);

	m_entityTable->RemoveComponentFrom(_entityId, _componentTypeId);
}

void DataManager::RemoveComponents()
{
	for (auto entity = m_componentsToBeRemoved->begin(); entity != m_componentsToBeRemoved->end(); ++entity)
	{
		for (auto componentTypeId : entity->second)
		{
			m_componentTables->at(componentTypeId)->ClearRow(entity->first);
		}
	}
}

void DataManager::RemoveEntities()
{
	for (auto entityId : *m_entitiesToBeRemoved)
	{
		std::vector<unsigned int> components;
		m_entityTable->GetEntityComponents(components, entityId);
		/* Clear the memory used by the entity. In other words, components */
		for (unsigned int i = 0; i < components.size(); ++i)
		{
			m_componentTables->at(components[i])->ClearRow(entityId);
		}
		m_entityTable->AddOldEntityId(entityId);
	}
}

void DataManager::ClearChangeLists()
{
	m_entitiesToBeAdded->clear();
	m_entitiesToBeRemoved->clear();
	m_changedEntities->clear();
	m_componentsToBeRemoved->clear();
}