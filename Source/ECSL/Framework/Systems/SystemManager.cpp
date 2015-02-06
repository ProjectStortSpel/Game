#include "SystemManager.h"

#include <assert.h>
#include "ECSL/Framework/Common/ContainerHelper.h"
#include "ECSL/Managers/ComponentTypeManager.h"

using namespace ECSL;

SystemManager::SystemManager(DataManager* _dataManager, std::vector<SystemWorkGroup*>* _systemWorkGroups) 
:	m_systemActivationManager(new SystemActivationManager()),
	m_systemIdManager(new SystemIdManager()),
	m_dataManager(_dataManager), 
	m_systems(new std::vector<System*>()),
	m_systemWorkGroups(_systemWorkGroups)
{

}

SystemManager::~SystemManager()
{
	for (int n = (unsigned int)m_systemWorkGroups->size() - 1; n >= 0; --n)
		delete m_systemWorkGroups->at(n);
	delete m_systemWorkGroups;
	delete m_systems;
	delete m_systemActivationManager;
	delete m_systemIdManager;
}

void SystemManager::InitializeSystems()
{
	/*	Initialize all system groups */
	for (unsigned int groupId = 0; groupId < m_systemWorkGroups->size(); ++groupId)
	{
		std::vector<System*>* systems = m_systemWorkGroups->at(groupId)->GetSystems();

		/*	Go through all systems in the group and initialize them	*/
		for (unsigned int systemId = 0; systemId < systems->size(); ++systemId)
		{
			System* system = systems->at(systemId);
			system->SetSystemActivationManager(m_systemActivationManager);
			system->SetSystemIdManager(m_systemIdManager);
			system->SetDataManager(m_dataManager);
			system->SetGroupId(groupId);

			system->Initialize();
			GenerateComponentFilter(system, FilterType::Mandatory);
			GenerateComponentFilter(system, FilterType::RequiresOneOf);
			GenerateComponentFilter(system, FilterType::Excluded);
			system->InitializeBackEnd();

			m_systems->push_back(system);
		}
	}
}

void SystemManager::PostInitializeSystems()
{
	/* PostInitialize all system groups */
	for (unsigned int groupId = 0; groupId < m_systemWorkGroups->size(); ++groupId)
	{
		std::vector<System*>* systems = m_systemWorkGroups->at(groupId)->GetSystems();
		/* Go through all systems in the group and postinitialize them */
		for (unsigned int systemId = 0; systemId < systems->size(); ++systemId)
		{
			System* system = systems->at(systemId);
			system->PostInitialize();
		}
	}
}

void SystemManager::UpdateSystemEntityLists(
	const RuntimeInfo& _runtime,
	std::vector<std::vector<unsigned int>*>& _entitiesToAddToSystems,
	std::vector<std::vector<unsigned int>*>& _entitiesToRemoveFromSystems)
{
	const std::vector<DataManager::EntityChange*>* entityChanges = m_dataManager->GetEntityChanges();
	const std::vector<unsigned int>* changedEntities = m_dataManager->GetChangedEntities();
	EntityTable* entityTable = m_dataManager->GetEntityTable();
	unsigned int dataTypeCount = BitSet::GetDataTypeCount(m_dataManager->GetComponentTypeCount());

	unsigned int startAt, endAt;
	MPL::MathHelper::SplitIterations(startAt, endAt, (unsigned int)m_systems->size(), _runtime.TaskIndex, _runtime.TaskCount);
	for (unsigned int i = startAt; i < endAt; ++i)
	{
		System* system = m_systems->at(i);
		const BitSet::DataType* mandatoryFilter = system->GetMandatoryFilter()->GetBitSet();
		const BitSet::DataType* requiresOneOfFilter = system->GetRequiresOneOfFilter()->GetBitSet();
		const BitSet::DataType* excludedFilter = system->GetExcludedFilter()->GetBitSet();
		unsigned int entitiesAddedTaskCount = system->GetEntitiesAddedTaskCount();
		unsigned int entitiesRemovedTaskCount = system->GetEntitiesRemovedTaskCount();
		for (unsigned int entityId : *changedEntities)
		{
			DataManager::EntityChange* entityChange = (*entityChanges)[entityId];

			/* If the entity is dead and the system has the entity, then remove it from the system */
			if (entityChange->Dead)
			{
				if (system->HasEntity(entityId))
				{
					system->RemoveEntityFromSystem(entityId);
					if (entitiesRemovedTaskCount > 0)
						_entitiesToRemoveFromSystems[i]->push_back(entityId);
				}
			}
			/* Else do a bit set check to see if the entity should be added or removed from the system */
			else
			{
				bool entityMatchesSystem = BitSet::BitSetPassFilters(dataTypeCount, entityTable->GetEntityComponents(entityId), mandatoryFilter, requiresOneOfFilter, excludedFilter);
				/* If system matches entity and entity isn't added to the system, add entity to system */
				if (entityMatchesSystem)
				{
					if (!system->HasEntity(entityId))
					{
						system->AddEntityToSystem(entityId);
						if (system->GetEntitiesAddedTaskCount() > 0)
							_entitiesToAddToSystems[i]->push_back(entityId);
					}
				}
				/* Else if entity doesn't match system and system has entity, remove entity from system */
				else if (system->HasEntity(entityId))
				{
					system->RemoveEntityFromSystem(entityId);
					if (system->GetEntitiesRemovedTaskCount() > 0)
						_entitiesToRemoveFromSystems[i]->push_back(entityId);
				}
			}
		}
	}
}

void SystemManager::GenerateComponentFilter(System* _system, FilterType _filterType)
{
	ComponentFilter* componentFilter = 0;
	switch (_filterType)
	{
	case FilterType::Mandatory:
		componentFilter = _system->GetMandatoryFilter();
		break;
	case FilterType::RequiresOneOf:
		componentFilter = _system->GetRequiresOneOfFilter();
		break;
	case FilterType::Excluded:
		componentFilter = _system->GetExcludedFilter();
		break;
	default:
		printf("Unknown ComponentFilter passed into AddComponentToFilter [System.cpp]\n");
		break;
	}
	std::vector<std::string>* componentTypes = componentFilter->GetComponentTypes();
	std::vector<unsigned int> componentIds = std::vector<unsigned int>();
	for (unsigned int i = 0; i < componentTypes->size(); ++i)
	{
		unsigned int componentTypeId = ComponentTypeManager::GetInstance().GetTableId(componentTypes->at(i));
		componentIds.push_back(componentTypeId);
	}
	unsigned int componentTypeCount = ComponentTypeManager::GetInstance().GetComponentTypeCount();
	BitSet::DataType* convertedBitSet = BitSet::BitSetConverter::ArrayToBitSet(componentIds, componentTypeCount);
	componentFilter->SetBitSet(convertedBitSet);
}