#include "SystemManager.h"

#include <assert.h>
#include "ECSL/Framework/Common/ContainerHelper.h"
#include "ECSL/Managers/ComponentTypeManager.h"

using namespace ECSL;

SystemManager::SystemManager(DataManager* _dataManager, std::vector<SystemWorkGroup*>* _systemWorkGroups) 
:	m_systemIdManager(new SystemIdManager()), 
	m_dataManager(_dataManager), m_systemWorkGroups(_systemWorkGroups)
{

}

SystemManager::~SystemManager()
{
	for (int n = (unsigned int)m_systemWorkGroups->size() - 1; n >= 0; --n)
		delete m_systemWorkGroups->at(n);
	delete m_systemWorkGroups;
	delete m_systemIdManager;
	delete m_changedEntitiesCopy;
}

void SystemManager::InitializeSystems()
{
	/*	Initialize all system groups */
	for (unsigned int groupId = 0; groupId < m_systemWorkGroups->size(); ++groupId)
	{
		std::vector<System*>* systems = m_systemWorkGroups->at(groupId)->GetSystems();
		std::vector<System*> systemsToUpdate;

		/*	Go through all systems in the group and initialize them	*/
		for (unsigned int systemId = 0; systemId < systems->size(); ++systemId)
		{
			System* system = systems->at(systemId);
			system->Initialize();
			system->SetId(m_systemIdManager->CreateSystemId(system->GetSystemName()));
			system->SetGroupId(groupId);
			system->SetDataManager(m_dataManager);
			system->SetSystemIdManager(m_systemIdManager);

			GenerateComponentFilter(system, FilterType::Mandatory);
			GenerateComponentFilter(system, FilterType::RequiresOneOf);
			GenerateComponentFilter(system, FilterType::Excluded);

			system->InitializeEntityList();

			/* Add system to the update group if the system has atleast one update task */
			if (system->GetUpdateTaskCount() > 0)
				systemsToUpdate.push_back(system);
		}
	}
}

void SystemManager::CopyChangedEntities(const RuntimeInfo& _runtime)
{
	m_changedEntitiesCopy = new std::vector<unsigned int>(*m_dataManager->GetChangedEntities());
}

void SystemManager::UpdateGroupEntityLists(
	const RuntimeInfo& _runtime,
	std::vector<bool>& _changedGroups,
	std::vector<std::vector<System*>*>& _entityAddedRequests,
	std::vector<std::vector<System*>*>& _entityRemovedRequests)
{
	std::vector<unsigned int> changedEntities = std::vector<unsigned int>(*m_dataManager->GetChangedEntities());
	EntityTable* entityTable = m_dataManager->GetEntityTable();
	std::vector<System*>* systems = systemWorkGroup->GetSystems();

	unsigned int startAt, endAt;
	MPL::MathHelper::SplitIterations(startAt, endAt, systems->size(), _runtime.TaskIndex, _runtime.TaskCount);
	/* Loop through every system see if changed entities passes the filter */
	for (unsigned int i = startAt; i < endAt; ++i)
	{
		System* system = systems->at(i);
		for (auto entityId : *m_changedEntitiesCopy)
		{
			/* Try add entity to system if it passes filters, else try to remove it */
			if (entityTable->EntityPassFilters(entityId, system->GetMandatoryFilter()->GetBitSet(), system->GetRequiresOneOfFilter()->GetBitSet(), system->GetExcludedFilter()->GetBitSet()))
				AddEntityToSystem(entityId, system);
			else
				RemoveEntityFromSystem(entityId, system);
		}
	}
}

void SystemManager::AddEntityToSystem(unsigned int _entityId, System* _system)
{
	if (!_system->HasEntity(_entityId))
	{
		_system->AddEntityToSystem(_entityId);
		//_system->OnEntityAdded(_entityId);
	}
}

void SystemManager::RemoveEntityFromSystem(unsigned int _entityId, System* _system)
{
	if (_system->HasEntity(_entityId))
	{
		_system->RemoveEntityFromSystem(_entityId);
		//_system->OnEntityRemoved(_entityId);
	}
}

// Calculate the bitset component filter for a system
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