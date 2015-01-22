#include "SystemManager.h"

#include <assert.h>
#include "ECSL/Framework/Common/ContainerHelper.h"
#include "ECSL/Managers/ComponentTypeManager.h"

using namespace ECSL;

SystemManager::SystemManager(DataManager* _dataManager, std::vector<SystemWorkGroup*>* _systemWorkGroups) 
:	m_systemIdManager(new SystemIdManager()),
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
			system->SetSystemIdManager(m_systemIdManager);
			system->SetDataManager(m_dataManager);
			system->SetGroupId(groupId);

			system->Initialize();
			GenerateComponentFilter(system, FilterType::Mandatory);
			GenerateComponentFilter(system, FilterType::RequiresOneOf);
			GenerateComponentFilter(system, FilterType::Excluded);

			system->InitializeEntityList();

			m_systems->push_back(system);
		}
	}
}

void SystemManager::UpdateSystemEntityLists(
	const RuntimeInfo& _runtime,
	std::vector<std::vector<unsigned int>*>& _entitiesToAddToSystems,
	std::vector<std::vector<unsigned int>*>& _entitiesToRemoveFromSystems)
{
	const std::vector<unsigned int>* changedEntities = m_dataManager->GetChangedEntities();
	EntityTable* entityTable = m_dataManager->GetEntityTable();

	unsigned int startAt, endAt;
	MPL::MathHelper::SplitIterations(startAt, endAt, (unsigned int)m_systems->size(), _runtime.TaskIndex, _runtime.TaskCount);
	/* Loop through every system see if changed entities passes the filter */
	for (unsigned int i = startAt; i < endAt; ++i)
	{
		System* system = m_systems->at(i);
		for (unsigned int j = 0; j < changedEntities->size(); ++j)
		{
			unsigned int entityId = changedEntities->at(j);
			/* Try add entity to system if it passes filters, else try to remove it */
			if (entityTable->EntityPassFilters(entityId, system->GetMandatoryFilter()->GetBitSet(), system->GetRequiresOneOfFilter()->GetBitSet(), system->GetExcludedFilter()->GetBitSet()))
			{
				if (!system->HasEntity(entityId))
				{
					system->AddEntityToSystem(entityId);
					if (system->GetEntitiesAddedTaskCount() > 0)
						_entitiesToAddToSystems[i]->push_back(entityId);
				}
			}
			else if (system->HasEntity(entityId))
			{
				system->RemoveEntityFromSystem(entityId);
				if (system->GetEntitiesRemovedTaskCount() > 0)
					_entitiesToRemoveFromSystems[i]->push_back(entityId);
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