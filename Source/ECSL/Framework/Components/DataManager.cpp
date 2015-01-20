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
	m_componentsToBeAdded(new std::map<unsigned int, std::vector<unsigned int>*>()),
	m_componentsToBeRemoved(new std::map<unsigned int, std::vector<unsigned int>*>()),
	m_componentDataToBeCleared(new std::map<unsigned int, std::vector<unsigned int>*>())
{
	m_changedEntitiesMutex = SDL_CreateMutex();
	m_entitiesToBeRemovedMutex = SDL_CreateMutex();
	m_componentsToBeAddedMutex = SDL_CreateMutex();
	m_componentsToBeRemovedMutex = SDL_CreateMutex();
	m_componentDataToBeClearedMutex = SDL_CreateMutex();
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

	SDL_DestroyMutex(m_changedEntitiesMutex);
	SDL_DestroyMutex(m_entitiesToBeRemovedMutex);
	SDL_DestroyMutex(m_componentsToBeAddedMutex);
	SDL_DestroyMutex(m_componentsToBeRemovedMutex);
	SDL_DestroyMutex(m_componentDataToBeClearedMutex);
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
}

void DataManager::CreateComponentAndAddTo(const std::string& _componentType, unsigned int _entityId)
{
	CreateComponentAndAddTo(ComponentTypeManager::GetInstance().GetTableId(_componentType), _entityId);
}

void DataManager::CreateComponentAndAddTo(unsigned int _componentTypeId, unsigned int _entityId)
{
	ToBeAdded(_entityId, _componentTypeId);
	Changed(_entityId);
}

void DataManager::RemoveComponentFrom(const std::string& _componentType, unsigned int _entityId)
{
	RemoveComponentFrom(ComponentTypeManager::GetInstance().GetTableId(_componentType), _entityId);
}

void DataManager::RemoveComponentFrom(unsigned int _componentTypeId, unsigned int _entityId)
{
	ToBeRemoved(_entityId, _componentTypeId);
	Changed(_entityId);
}

void DataManager::UpdateEntityTable(const RuntimeInfo& _runtime)
{
	unsigned int startAt, endAt;
	MPL::MathHelper::SplitIterations(startAt, endAt, m_changedEntities->size(), _runtime.TaskIndex, _runtime.TaskCount);
	for (unsigned int i = startAt; i < endAt; ++i)
	{
		unsigned int entityId = m_changedEntities->at(i);

		auto it = m_componentsToBeRemoved->find(entityId);
		if (it != m_componentsToBeRemoved->end())
		{
			std::vector<unsigned int>* componentTypeIds = it->second;
			for (unsigned int componentTypeId : *componentTypeIds)
			{
				m_entityTable->RemoveComponentFrom(entityId, componentTypeId);
				DataToBeCleared(entityId, componentTypeId);
			}
			componentTypeIds->clear();
		}

		it = m_componentsToBeAdded->find(entityId);
		if (it != m_componentsToBeAdded->end())
		{
			std::vector<unsigned int>* componentTypeIds = it->second;
			for (unsigned int componentTypeId : *componentTypeIds)
			{
				m_entityTable->AddComponentTo(entityId, componentTypeId);
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

void DataManager::RecycleEntityIds(const RuntimeInfo& _runtime)
{
	for (auto entityId : *m_entitiesToBeRemoved)
	{
		m_entityTable->AddOldEntityId(entityId);
	}
}

void DataManager::ClearComponentData(const RuntimeInfo& _runtime)
{
	unsigned int startAt, endAt;
	MPL::MathHelper::SplitIterations(startAt, endAt, m_componentDataToBeCleared->size(), _runtime.TaskIndex, _runtime.TaskCount);
	for (unsigned int i = startAt; i < endAt; ++i)
	{
		auto it = m_componentDataToBeCleared->at(i);
		m_componentTables[m_componentDataToBeCleared->at(i)];
	}
}

void DataManager::ClearComponentChangeLists(const RuntimeInfo& _runtime)
{
	m_changedEntities->clear();
}

void DataManager::ClearDeadEntitiesChangeLists(const RuntimeInfo& _runtime)
{
	m_entitiesToBeRemoved->clear();
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

void DataManager::Changed(unsigned int _entityId)
{
	SDL_LockMutex(m_changedEntitiesMutex);
	ContainerHelper::AddUniqueElement<unsigned int>(_entityId, (*m_changedEntities));
	SDL_UnlockMutex(m_changedEntitiesMutex);
}

void DataManager::ToBeAdded(unsigned int _entityId, unsigned int _componentTypeId)
{
	SDL_LockMutex(m_componentsToBeAddedMutex);
	ContainerHelper::AddUniqueElement<unsigned int>(_componentTypeId, *(*m_componentsToBeAdded)[_entityId]);
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
	ContainerHelper::AddUniqueElement<unsigned int>(_componentTypeId, *(*m_componentsToBeRemoved)[_entityId]);
	SDL_UnlockMutex(m_componentsToBeRemovedMutex);
}

void DataManager::DataToBeCleared(unsigned int _entityId, unsigned int _componentTypeId)
{
	SDL_LockMutex(m_componentDataToBeClearedMutex);
	ContainerHelper::AddUniqueElement<unsigned int>(_componentTypeId, *(*m_componentDataToBeCleared)[_entityId]);
	SDL_UnlockMutex(m_componentDataToBeClearedMutex);
}