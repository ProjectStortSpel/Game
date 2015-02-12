#include "DataManager.h"

#include <assert.h>
#include "Tables/ComponentTable.h"
#include "Tables/DataArray.h"
#include "Tables/DataSlimArray.h"
#include "ECSL/Framework/Common/ContainerHelper.h"
#include "ECSL/Framework/Multithreading/RuntimeInfo.h"
#include "ECSL/Managers/ComponentTypeManager.h"
#include "ECSL/Framework/Logger/DataLogger.h"

using namespace ECSL;

DataManager::DataManager(unsigned int _entityCount, std::vector<unsigned int>* _componentTypeIds)
:	m_entityCount(_entityCount), m_componentTypeIds(_componentTypeIds),
	m_entityChanges(new std::vector<EntityChange*>(_entityCount, nullptr)),
	m_entityChangesCopy(new std::vector<EntityChange*>(_entityCount, nullptr)),
	m_changedEntities(new std::vector<unsigned int>()),
	m_changedEntitiesCopy(new std::vector<unsigned int>())
{
	m_entityChangesMutex = SDL_CreateMutex();
	//String?
	m_stringMutex = SDL_CreateMutex();
}

DataManager::~DataManager()
{
	for (auto it = m_componentTables->begin(); it != m_componentTables->end(); ++it)
	{
		delete *it;
	}
	m_componentTables->clear();
	
	delete(m_componentTables);
	delete(m_entityTable);
	delete(m_componentTypeIds);
	delete(m_entityChanges);
	delete(m_entityChangesCopy);
	delete(m_changedEntities);
	delete(m_changedEntitiesCopy);

	SDL_DestroyMutex(m_entityChangesMutex);
	//String?
	SDL_DestroyMutex(m_stringMutex);
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
			m_componentTables->at(componentTypeId) = new ComponentTable(new DataSlimArray(m_entityCount, componentType->GetByteSize()), componentTypeId);
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
	unsigned startAt, endAt;
	MPL::MathHelper::SplitIterations(startAt, endAt, (unsigned int)m_changedEntitiesCopy->size(), _runtime.TaskIndex, _runtime.TaskCount);
	for (unsigned int i = startAt; i < endAt; ++i)
	{
		unsigned int entityId = (*m_changedEntitiesCopy)[i];
		EntityChange* entityChange = (*m_entityChangesCopy)[entityId];
		if (entityChange->Dead)
		{
			SDL_LockMutex(m_stringMutex);
			if (m_stringMap.find(entityId) != m_stringMap.end())
			{
				m_stringMap.erase(entityId);
			}
			SDL_UnlockMutex(m_stringMutex);

			m_entityTable->ClearEntityData(entityId);
		}
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
					{
						SDL_LockMutex(m_stringMutex);
						if (m_stringMap.find(entityId) != m_stringMap.end())
						{
							std::map<unsigned int, std::map<unsigned int, std::string>>* compToVarMap = &m_stringMap[entityId];
							if (compToVarMap->find(componentChange.first) != compToVarMap->end())
							{
								compToVarMap->erase(componentChange.first);
							}
						}
						SDL_UnlockMutex(m_stringMutex);

						m_entityTable->RemoveComponentFrom(entityId, componentChange.first);
						break;
					}						
				}
			}
		}
	}
}

void DataManager::RecycleEntityIds(const RuntimeInfo& _runtime)
{
	for (auto entityId : *m_changedEntitiesCopy)
	{
		if ((*m_entityChangesCopy)[entityId]->Dead)
			m_entityTable->AddOldEntityId(entityId);
	}
}

void DataManager::CopyCurrentLists(const RuntimeInfo& _runtime)
{
	/* Swap the lists */
	std::vector<EntityChange*>* swapPointer = m_entityChangesCopy;
	m_entityChangesCopy = m_entityChanges;
	m_entityChanges = swapPointer;

	/* Clear the data in entityChanges list */
	for (auto entityId : *m_changedEntitiesCopy)
	{
		delete((*m_entityChanges)[entityId]);
		(*m_entityChanges)[entityId] = nullptr;
	}

	delete(m_changedEntitiesCopy);
	m_changedEntitiesCopy = m_changedEntities;
	m_changedEntities = new std::vector<unsigned int>();
}

void DataManager::DeleteComponentData(const RuntimeInfo& _runtime)
{
	unsigned startAt, endAt;
	MPL::MathHelper::SplitIterations(startAt, endAt, (unsigned int)m_changedEntitiesCopy->size(), _runtime.TaskIndex, _runtime.TaskCount);
	for (unsigned int i = startAt; i < endAt; ++i)
	{
		unsigned int entityId = (*m_changedEntitiesCopy)[i];
		EntityChange* entityChange = (*m_entityChangesCopy)[entityId];
		/* If entity is dead, remove all components */
		if (entityChange->Dead)
		{
			for (auto componentTypeId : *m_componentTypeIds)
			{
				if (m_entityTable->HasComponent(entityId, componentTypeId))
					(*m_componentTables)[componentTypeId]->ClearComponent(entityId);
			}
		}
		/* Else remove the specified components */
		else
		{
			for (auto componentChanges : entityChange->ComponentChanges)
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
	EntityChange* entityChange = (*m_entityChanges)[_entityId];
	if (!entityChange)
	{
		entityChange = new EntityChange();
		entityChange->ComponentChanges[_componentTypeId] = TO_BE_ADDED;
		(*m_entityChanges)[_entityId] = entityChange;
		m_changedEntities->push_back(_entityId);
	}
	else
		entityChange->ComponentChanges[_componentTypeId] = TO_BE_ADDED;
	SDL_UnlockMutex(m_entityChangesMutex);
}

void DataManager::ToBeRemoved(unsigned int _entityId)
{
	SDL_LockMutex(m_entityChangesMutex);
	EntityChange* entityChange = (*m_entityChanges)[_entityId];
	if (!entityChange)
	{
		entityChange = new EntityChange();
		entityChange->Dead = true;
		(*m_entityChanges)[_entityId] = entityChange;
		m_changedEntities->push_back(_entityId);
	}
	else
		entityChange->Dead = true;
	SDL_UnlockMutex(m_entityChangesMutex);
}

void DataManager::ToBeRemoved(unsigned int _entityId, unsigned int _componentTypeId)
{
	SDL_LockMutex(m_entityChangesMutex);
	EntityChange* entityChange = (*m_entityChanges)[_entityId];
	if (!entityChange)
	{
		entityChange = new EntityChange();
		entityChange->ComponentChanges[_componentTypeId] = TO_BE_REMOVED;
		(*m_entityChanges)[_entityId] = entityChange;
		m_changedEntities->push_back(_entityId);
	}
	else
		entityChange->ComponentChanges[_componentTypeId] = TO_BE_REMOVED;
	SDL_UnlockMutex(m_entityChangesMutex);
}



//String?
void DataManager::SetString(unsigned int _eId, unsigned int _componentTypeId, const unsigned int _index, const char* _value)
{
	SDL_LockMutex(m_stringMutex);

	m_stringMap[_eId][_componentTypeId][_index] = _value;

	SDL_UnlockMutex(m_stringMutex);
}

std::string DataManager::GetString(unsigned int _eId, unsigned int _componentTypeId, const unsigned int _index)
{
	std::string result = "";
	SDL_LockMutex(m_stringMutex);

	if (m_stringMap.find(_eId) != m_stringMap.end())
	{
		std::map<unsigned int, std::map<unsigned int, std::string>>* compToVarMap = &m_stringMap[_eId];
		if (compToVarMap->find(_componentTypeId) != compToVarMap->end())
		{
			std::map<unsigned int, std::string>* VarToStrMap = &compToVarMap->at(_componentTypeId);

			if (VarToStrMap->find(_index) != VarToStrMap->end())
			{
				result = VarToStrMap->at(_index);
			}
		}
	}

	SDL_UnlockMutex(m_stringMutex);

	return result;
}