#include "SystemManager.h"

#include <assert.h>
#include "ECSL/Managers/ComponentTypeManager.h"
#include <sstream>

using namespace ECSL;

SystemManager::SystemManager(DataManager* _dataManager, std::vector<SystemWorkGroup*>* _systemWorkGroups) 
:	m_nextSystemId(-1), m_dataManager(_dataManager), m_systemWorkGroups(_systemWorkGroups)
{

}

SystemManager::~SystemManager()
{
	for (int n = (unsigned int)m_systemWorkGroups->size() - 1; n >= 0; --n)
		delete m_systemWorkGroups->at(n);

	delete m_systemWorkGroups;
}

void SystemManager::InitializeSystems()
{
	/*	Initialize all system groups	*/
	for (unsigned int groupId = 0; groupId < m_systemWorkGroups->size(); ++groupId)
	{
		std::vector<System*>* systems = m_systemWorkGroups->at(groupId)->GetSystems();

		/*	Go through all systems in the group and initialize them	*/
		for (unsigned int systemId = 0; systemId < systems->size(); ++systemId)
		{
			System* system = systems->at(systemId);
			system->Initialize();

			GenerateComponentFilter(system, FilterType::Mandatory);
			GenerateComponentFilter(system, FilterType::RequiresOneOf);
			GenerateComponentFilter(system, FilterType::Excluded);

			system->SetDataManager(m_dataManager);
			system->InitializeEntityList();
		}
	}
}

void SystemManager::PostInitializeSystems()
{
	/*	PostInitialize all system groups	*/
	for (unsigned int groupId = 0; groupId < m_systemWorkGroups->size(); ++groupId)
	{
		std::vector<System*>* systems = m_systemWorkGroups->at(groupId)->GetSystems();

		/*	Go through all systems in the group and postinitialize them	*/
		for (unsigned int systemId = 0; systemId < systems->size(); ++systemId)
		{
			System* system = systems->at(systemId);
			system->PostInitialize();
		}
	}
}

void SystemManager::Update(float _dt)
{
	for (auto workGroup : *m_systemWorkGroups)
	{
		workGroup->Update(_dt);
	}
}

void SystemManager::AddEntityToSystem(unsigned int _entityId, System* _system)
{
	if (!_system->HasEntity(_entityId))
	{
		_system->AddEntityToSystem(_entityId);
		_system->OnEntityAdded(_entityId);
	}
}

void SystemManager::RemoveEntityFromSystem(unsigned int _entityId, System* _system)
{
	if (_system->HasEntity(_entityId))
	{
		_system->RemoveEntityFromSystem(_entityId);
		_system->OnEntityRemoved(_entityId);
	}
}

void SystemManager::SystemEntitiesUpdate()
{
	const std::vector<unsigned int>* changedEntities = m_dataManager->GetChangedEntities();
	EntityTable* entityTable = m_dataManager->GetEntityTable();

	/*	Loop through all entities	*/
	for (int n = 0; n < changedEntities->size(); ++n)
	{
		unsigned int entityId = changedEntities->at(n);

		/* Loop through every system see if changed entities passes the filter */
		for (auto workGroup : *m_systemWorkGroups)
		{
			for (auto system : *workGroup->GetSystems())
			{
                //printf("System: %s\n", system->GetSystemName().c_str());
				/* Try add entity to system if it passes filters, else try to remove it */
				if (entityTable->EntityPassFilters(entityId, system->GetMandatoryFilter()->GetBitSet(), system->GetRequiresOneOfFilter()->GetBitSet(), system->GetExcludedFilter()->GetBitSet()))
					AddEntityToSystem(entityId, system);
				else
					RemoveEntityFromSystem(entityId, system);
			}
		}
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
        
        if( ComponentTypeManager::GetInstance().GetComponentType(componentTypeId) == 0 )
        {
            std::stringstream ss;
            ss << "System " << _system->GetSystemName().c_str();
            ss << " is trying to use component \"" << componentTypes->at(i);
            ss << "\" when it doesn't exist!\n";
            
            printf("%s", ss.str().c_str());
            assert(false);
        }
        
		componentIds.push_back(componentTypeId);
	}
	unsigned int componentTypeCount = ComponentTypeManager::GetInstance().GetComponentTypeCount();
	BitSet::DataType* convertedBitSet = BitSet::BitSetConverter::ArrayToBitSet(componentIds, componentTypeCount);
	componentFilter->SetBitSet(convertedBitSet);
}