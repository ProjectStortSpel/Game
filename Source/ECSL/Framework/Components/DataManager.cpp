#include "DataManager.h"

#include <assert.h>
#include "Tables/ComponentTable.h"
#include "Tables/DataArray.h"
#include "Tables/DataMap.h"
#include "ECSL/Framework/Common/ContainerHelper.h"
#include "ECSL/Framework/Multithreading/RuntimeInfo.h"
#include "ECSL/Managers/ComponentTypeManager.h"

using namespace ECSL;

DataManager::DataManager(unsigned int _entityCount, std::vector<unsigned int>* _componentTypeIds)
:	m_entityCount(_entityCount), m_componentTypeIds(_componentTypeIds),
	m_entitiesToBeRemoved(new std::vector<unsigned int>()), m_changedEntities(new std::vector<unsigned int>()),
	m_componentsToBeRemoved(new std::map<unsigned int, std::vector<unsigned int>>())
{
	m_changedEntitiesMutex = SDL_CreateMutex();
	m_entitiesToBeRemovedMutex = SDL_CreateMutex();
	m_componentsToBeAddedMutex = SDL_CreateMutex();
	m_componentsToBeRemovedMutex = SDL_CreateMutex();
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
	ToBeRemoved(_entityId);
	Changed(_entityId);

	///* Add entity to removal list */
	//m_entitiesToBeRemoved->push_back(_entityId);

	//std::vector<unsigned int> components;
	//m_entityTable->GetEntityComponents(components, _entityId);
	///* Remove every component from the entity */
	//for (unsigned int componentTypeId : components)
	//	RemoveComponentFrom(componentTypeId, _entityId);

	//m_entityTable->ClearEntityData(_entityId);
}

void DataManager::CreateComponentAndAddTo(const std::string& _componentType, unsigned int _entityId)
{
	CreateComponentAndAddTo(ComponentTypeManager::GetInstance().GetTableId(_componentType), _entityId);
}

void DataManager::CreateComponentAndAddTo(unsigned int _componentTypeId, unsigned int _entityId)
{
	ToBeAdded(_entityId, _componentTypeId);
	Changed(_entityId);

	///* Add entity to lists */
	//m_changedEntities->push_back(_entityId);

	//m_entityTable->AddComponentTo(_entityId, _componentTypeId);
}

void DataManager::RemoveComponentFrom(const std::string& _componentType, unsigned int _entityId)
{
	RemoveComponentFrom(ComponentTypeManager::GetInstance().GetTableId(_componentType), _entityId);
}

void DataManager::RemoveComponentFrom(unsigned int _componentTypeId, unsigned int _entityId)
{
	ToBeRemoved(_entityId, _componentTypeId);
	Changed(_entityId);

	///* Add entity and component to lists */
	//ContainerHelper::TryAddKey<unsigned int, std::vector<unsigned int>>(_entityId, *m_componentsToBeRemoved);
	//(*m_componentsToBeRemoved)[_entityId].push_back(_componentTypeId);
	//m_changedEntities->push_back(_entityId);

	//m_entityTable->RemoveComponentFrom(_entityId, _componentTypeId);
}

void DataManager::UpdateEntityTable(const RuntimeInfo& _runtime)
{
	unsigned int startAt, endAt;
	MPL::MathHelper::SplitIterations(startAt, endAt, m_changedEntities->size(), _runtime.TaskIndex, _runtime.TaskCount);
	for (unsigned int i = startAt; i < endAt; ++i)
	{
		unsigned int entityId = m_changedEntities->at(i);

		auto it = m_componentsToBeAdded->find(entityId);
		if (it != m_componentsToBeAdded->end())
		{
			std::vector<unsigned int>* componentTypeIds = &(it->second);
			for (unsigned int componentTypeId : *componentTypeIds)
			{
				m_entityTable->AddComponentTo(entityId, componentTypeId);
			}
			componentTypeIds->clear();
		}

		it = m_componentsToBeRemoved->find(entityId);
		if (it != m_componentsToBeRemoved->end())
		{
			std::vector<unsigned int>* componentTypeIds = &(it->second);
			for (unsigned int componentTypeId : *componentTypeIds)
			{
				m_entityTable->RemoveComponentFrom(entityId, componentTypeId);
			}
			componentTypeIds->clear();
		}
	}
}

void DataManager::ClearDeadEntities(const RuntimeInfo& _runtime)
{
	unsigned int startAt, endAt;
	MPL::MathHelper::SplitIterations(startAt, endAt, m_entitiesToBeRemoved->size(), _runtime.TaskIndex, _runtime.TaskCount);
	for (unsigned int i = startAt; i < endAt; ++i)
	{
		m_entityTable->ClearEntityData(m_entitiesToBeRemoved->at(i));
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
	m_changedEntities->clear();
	m_entitiesToBeRemoved->clear();
	m_componentsToBeAdded->clear();
	m_componentsToBeRemoved->clear();
}

void DataManager::Changed(unsigned int _entityId)
{
	SDL_LockMutex(m_changedEntitiesMutex);
	ContainerHelper::AddUniqueElement<unsigned int>(_entityId, (*m_changedEntities));
	SDL_UnlockMutex(m_changedEntitiesMutex);
}

void DataManager::ToBeAdded(unsigned int _entityId, unsigned int _componentTypeId)
{
	SDL_LockMutex(m_componentsToBeAddedMutex);
	ContainerHelper::AddUniqueElement<unsigned int>(_componentTypeId, (*m_componentsToBeAdded)[_entityId]);
	SDL_UnlockMutex(m_componentsToBeAddedMutex);
}

void DataManager::ToBeRemoved(unsigned int _entityId)
{
	SDL_LockMutex(m_entitiesToBeRemovedMutex);
	ContainerHelper::AddUniqueElement<unsigned int>(_entityId, (*m_entitiesToBeRemoved));
	SDL_UnlockMutex(m_entitiesToBeRemovedMutex);
}

void DataManager::ToBeRemoved(unsigned int _entityId, unsigned int _componentTypeId)
{
	SDL_LockMutex(m_componentsToBeRemovedMutex);
	ContainerHelper::AddUniqueElement<unsigned int>(_componentTypeId, (*m_componentsToBeRemoved)[_entityId]);
	SDL_UnlockMutex(m_componentsToBeRemovedMutex);
}