#include "DataManager.h"

#include <assert.h>
#include "Tables/ComponentTable.h"
#include "Tables/DataArray.h"
#include "Tables/DataMap.h"
#include "ECSL/Framework/Common/ContainerHelper.h"
#include "ECSL/Framework/Multithreading/RuntimeInfo.h"
#include "ECSL/Managers/ComponentTypeManager.h"
#include "ECSL/Framework/Logger/DataLogger.h"

using namespace ECSL;

DataManager::DataManager(unsigned int _entityCount, std::vector<unsigned int>* _componentTypeIds)
:	m_entityCount(_entityCount), m_componentTypeIds(_componentTypeIds),
	m_entityChanges(new std::map<unsigned int, EntityChange*>())
{
	m_entityChangesMutex = SDL_CreateMutex();
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
	delete m_entityChanges;

	SDL_DestroyMutex(m_entityChangesMutex);
}

void DataManager::InitializeTables()
{
	m_componentTypeCount = ComponentTypeManager::GetInstance().GetComponentTypeCount();
	m_componentTables = new std::vector<ComponentTable*>(m_componentTypeCount, nullptr);
	m_entityTable = new EntityTable(m_entityCount, (unsigned int)m_componentTypeIds->size());

	for (unsigned int n = 0; n < m_componentTypeIds->size(); ++n)
	{
		unsigned int componentTypeId = m_componentTypeIds->at(n);
		ComponentType* componentType = ComponentTypeManager::GetInstance().GetComponentType(componentTypeId);

		/* Couldn't find component type. Is the type added and is it spelled correctly? */
		assert(componentType || DataLogger::GetInstance().LogWorldDataAssert());

		switch (componentType->GetTableType())
		{
		case TableType::Array:
			m_componentTables->at(componentTypeId) = new ComponentTable(new DataArray(m_entityCount, componentType->GetByteSize()), componentTypeId);
			break;
		case TableType::Map:
			m_componentTables->at(componentTypeId) = new ComponentTable(new DataMap(componentType->GetByteSize()), componentTypeId);
			break;
		default:
			printf("ERROR: Invalid Table Type! (ID %i)\n", m_componentTypeIds->at(n));
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
}

void DataManager::CreateComponentAndAddTo(const std::string& _componentType, unsigned int _entityId)
{
	CreateComponentAndAddTo(ComponentTypeManager::GetInstance().GetTableId(_componentType), _entityId);
}

void DataManager::CreateComponentAndAddTo(unsigned int _componentTypeId, unsigned int _entityId)
{
	ToBeAdded(_entityId, _componentTypeId);
}

void DataManager::RemoveComponentFrom(const std::string& _componentType, unsigned int _entityId)
{
	RemoveComponentFrom(ComponentTypeManager::GetInstance().GetTableId(_componentType), _entityId);
}

void DataManager::RemoveComponentFrom(unsigned int _componentTypeId, unsigned int _entityId)
{
	ToBeRemoved(_entityId, _componentTypeId);
}

void DataManager::UpdateEntityTable(const RuntimeInfo& _runtime)
{
	for (auto entity : *m_entityChangesCopy)
	{
		unsigned int entityId = entity.first;
		EntityChange* entityChange = entity.second;
		if (entityChange->Dead)
			m_entityTable->ClearEntityData(entityId);
		else
		{
			for (auto componentChange : entityChange->ComponentChanges)
			{
				switch (componentChange.second)
				{
					case ComponentChange::TO_BE_ADDED:
						m_entityTable->AddComponentTo(entityId, componentChange.first);
						break;
					case ComponentChange::TO_BE_REMOVED:
						m_entityTable->RemoveComponentFrom(entityId, componentChange.first);
						break;
				}
			}
		}
	}
}

void DataManager::RecycleEntityIds(const RuntimeInfo& _runtime)
{
	for (auto entity : *m_entityChangesCopy)
	{
		if (entity.second->Dead)
			m_entityTable->AddOldEntityId(entity.first);
	}
}

void DataManager::CopyCurrentLists(const RuntimeInfo& _runtime)
{
	m_entityChangesCopy = m_entityChanges;
	m_entityChanges = new std::map<unsigned int, EntityChange*>();
}

void DataManager::ClearCopiedLists(const RuntimeInfo& _runtime)
{
	for (auto keyValuePair : *m_entityChangesCopy)
		delete(keyValuePair.second);
	delete(m_entityChangesCopy);
}

void DataManager::DeleteComponentData(const RuntimeInfo& _runtime)
{
	for (auto entityChange : *m_entityChangesCopy)
	{
		/* If entity is dead, remove all components*/
		if (entityChange.second->Dead)
		{
			for (auto componentTypeId : *m_componentTypeIds)
			{
				if (m_entityTable->HasComponent(0, componentTypeId))
					(*m_componentTables)[componentTypeId]->ClearComponent(entityChange.first);
			}
		}
		/* Else remove the specified components */
		else
		{
			for (auto componentChanges : entityChange.second->ComponentChanges)
			{
				if (componentChanges.second == ComponentChange::TO_BE_REMOVED)
					(*m_componentTables)[componentChanges.first]->ClearComponent(componentChanges.first);
			}
		}
	}
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

void DataManager::ToBeAdded(unsigned int _entityId, unsigned int _componentTypeId)
{
	SDL_LockMutex(m_entityChangesMutex);
	auto it = m_entityChanges->find(_entityId);
	if (it == m_entityChanges->end())
	{
		EntityChange* entityChange = new EntityChange();
		entityChange->ComponentChanges[_componentTypeId] = TO_BE_ADDED;
		(*m_entityChanges)[_entityId] = entityChange;
	}
	else
	{
		it->second->ComponentChanges[_componentTypeId] = TO_BE_ADDED;
	}
	SDL_UnlockMutex(m_entityChangesMutex);
}

void DataManager::ToBeRemoved(unsigned int _entityId)
{
	SDL_LockMutex(m_entityChangesMutex);
	auto it = m_entityChanges->find(_entityId);
	if (it == m_entityChanges->end())
	{
		EntityChange* entityChange = new EntityChange();
		entityChange->Dead = true;
		(*m_entityChanges)[_entityId] = entityChange;
	}
	else
	{
		it->second->Dead = true;
	}
	SDL_UnlockMutex(m_entityChangesMutex);
}

void DataManager::ToBeRemoved(unsigned int _entityId, unsigned int _componentTypeId)
{
	SDL_LockMutex(m_entityChangesMutex);
	auto it = m_entityChanges->find(_entityId);
	if (it == m_entityChanges->end())
	{
		EntityChange* entityChange = new EntityChange();
		entityChange->ComponentChanges[_componentTypeId] = TO_BE_REMOVED;
		(*m_entityChanges)[_entityId] = entityChange;
	}
	else
	{
		it->second->ComponentChanges[_componentTypeId] = TO_BE_REMOVED;
	}
	SDL_UnlockMutex(m_entityChangesMutex);
}