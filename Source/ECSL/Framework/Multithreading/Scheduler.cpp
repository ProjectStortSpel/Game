#include "Scheduler.h"

#include "MPL/Managers/TaskManager.h"

using namespace ECSL;

Scheduler::Scheduler(DataManager* _dataManager, SystemManager* _systemManager)
:	m_dataManager(_dataManager),
	m_systemManager(_systemManager),
	m_updateWorkItems(new std::vector<std::vector<MPL::WorkItem*>*>()),
	m_updateEntityComponentsWorkItems(new std::vector<MPL::WorkItem*>()),
	m_clearDeadEntitiesWorkItems(new std::vector<MPL::WorkItem*>())
{
	const std::vector<SystemWorkGroup*>* workGroups = m_systemManager->GetSystemWorkGroups();
	unsigned int workCount = MPL::TaskManager::GetInstance().GetThreadCount();
	m_entityAddedRequests = new std::vector<std::vector<System*>*>[workCount];
	m_entityRemovedRequests = new std::vector<std::vector<System*>*>[workCount];
	for (unsigned int threadIndex = 0; threadIndex < workCount; ++threadIndex)
	{
		m_entityAddedRequests[threadIndex] = std::vector<std::vector<System*>*>(workGroups->size());
		m_entityRemovedRequests[threadIndex] = std::vector<std::vector<System*>*>(workGroups->size());
		for (unsigned int i = 0; i < workGroups->size(); ++i)
		{
			m_entityAddedRequests[threadIndex][i] = new std::vector<System*>();
			m_entityAddedRequests[threadIndex][i] = new std::vector<System*>();
		}
	}
}

Scheduler::~Scheduler()
{

}

void Scheduler::UpdateDt(float _dt)
{
	for (auto workItems : *m_updateWorkItems)
	{
		for (auto workItem : *workItems)
			((UpdateData*)workItem->Data)->RuntimeInfo.Dt = _dt;
	}
}

MPL::TaskId Scheduler::ScheduleUpdate()
{
	MPL::TaskId currentTaskId = MPL::NoDependency;
	MPL::TaskId lastTaskId;
	/* Create tasks of each groups' work items. Each new task will have a dependency to the previous task */
	for (auto workItems : *m_updateWorkItems)
	{
		/* Send the work items to the TaskManager and create a task out of it */
		lastTaskId = currentTaskId;
		currentTaskId = MPL::TaskManager::GetInstance().BeginAdd(lastTaskId);
		MPL::TaskManager::GetInstance().AddChildren(currentTaskId, *workItems);
		MPL::TaskManager::GetInstance().FinishAdd(currentTaskId);
	}

	return currentTaskId;
}

MPL::TaskId Scheduler::ScheduleUpdateEntityTable(MPL::TaskId _dependency)
{
	MPL::TaskId updateEntityComponents = MPL::TaskManager::GetInstance().BeginAdd(_dependency);
	MPL::TaskManager::GetInstance().AddChildren(updateEntityComponents, *m_updateEntityComponentsWorkItems);
	MPL::TaskManager::GetInstance().FinishAdd(updateEntityComponents);
	return updateEntityComponents;
}

MPL::TaskId Scheduler::ScheduleClearDeadEntities(MPL::TaskId _dependency)
{
	MPL::TaskId clearDeadEntities = MPL::TaskManager::GetInstance().BeginAdd(_dependency);
	MPL::TaskManager::GetInstance().AddChildren(clearDeadEntities, *m_clearDeadEntitiesWorkItems);
	MPL::TaskManager::GetInstance().FinishAdd(clearDeadEntities);
	return clearDeadEntities;
}

void Scheduler::AddUpdateTasks(const std::vector<SystemWorkGroup*>& _systemGroups)
{
	for (auto systemGroup : _systemGroups)
	{
		std::vector<MPL::WorkItem*>* workItems = new std::vector<MPL::WorkItem*>();
		for (auto system : *systemGroup->GetSystems())
		{
			/* Create one work item for each system update task */
			for (unsigned int taskIndex = 0; taskIndex < system->GetUpdateTaskCount(); ++taskIndex)
			{
				UpdateData* data = new UpdateData();
				data->System = system;
				data->RuntimeInfo.TaskIndex = taskIndex;
				data->RuntimeInfo.TaskCount = system->GetUpdateTaskCount();
				MPL::WorkItem* workItem = new MPL::WorkItem();
				workItem->Work = &SystemUpdate;
				workItem->Data = data;
				workItems->push_back(workItem);
			}
		}
		m_updateWorkItems->push_back(workItems);
	}
}

void Scheduler::AddOnEntityAddedTask(const std::vector<System*>& _systemWorkGroup)
{

}

void Scheduler::AddOnEntityRemovedTask(const std::vector<System*>& _systemWorkGroup)
{

}

void Scheduler::AddUpdateEntityTableTask()
{
	const unsigned int updateEntityComponentsWorkCount = 1;
	
	/* Add update entity component tasks*/
	for (unsigned int i = 0; i < updateEntityComponentsWorkCount; ++i)
	{
		DataManagerData* data = new DataManagerData();
		data->DataManager = m_dataManager;
		data->RuntimeInfo.TaskIndex = i;
		data->RuntimeInfo.TaskCount = updateEntityComponentsWorkCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &DataManagerUpdateEntityTable;
		workItem->Data = data;
		m_updateEntityComponentsWorkItems->push_back(workItem);
	}
}

void Scheduler::AddClearDeadEntitiesTask()
{
	const unsigned int clearDeadEntitiesWorkCount = 1;

	for (unsigned int i = 0; i < clearDeadEntitiesWorkCount; ++i)
	{
		DataManagerData* data = new DataManagerData();
		data->DataManager = m_dataManager;
		data->RuntimeInfo.TaskIndex = i;
		data->RuntimeInfo.TaskCount = clearDeadEntitiesWorkCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &DataManagerClearDeadEntities;
		workItem->Data = data;
		m_updateEntityComponentsWorkItems->push_back(workItem);
	}
}

void Scheduler::AddUpdateSystemEntityListsTasks()
{
	const unsigned int workCount = MPL::TaskManager::GetInstance().GetThreadCount();

	std::vector<MPL::WorkItem*>* workItems = new std::vector<MPL::WorkItem*>();
	for (unsigned int workIndex = 0; workIndex < workCount; ++workIndex)
	{
		UpdateSystemEntityListsData* data = new UpdateSystemEntityListsData();
		data->SystemManager = m_systemManager;
		data->EntityAddedRequests = &m_entityAddedRequests[workIndex];
		data->EntityRemovedRequests = &m_entityRemovedRequests[workIndex];
		data->RuntimeInfo.TaskIndex = workIndex;
		data->RuntimeInfo.TaskCount = workCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &SystemManagerUpdateSystemEntityLists;
		workItem->Data = data;
		m_updateEntityComponentsWorkItems->push_back(workItem);
	}
	m_updateGroupEntityListsWorkItems->push_back(workItems);
}

void ECSL::SystemUpdate(void* _data)
{
	UpdateData* data = (UpdateData*)_data;
	data->System->Update(data->RuntimeInfo);
}

void ECSL::SystemOnEntityAdded(void* _data)
{
	OnEntityData* data = (OnEntityData*)_data;
	//onEntity->System->OnEntityAdded(onEntity->TaskIndex, onEntity->Dt, onEntity->EntityId);
}

void ECSL::SystemOnEntityRemoved(void* _data)
{
	OnEntityData* data = (OnEntityData*)_data;
	//onEntity->System->OnEntityRemoved(onEntity->TaskIndex, onEntity->Dt, onEntity->EntityId);
}

void ECSL::DataManagerUpdateEntityTable(void* _data)
{
	DataManagerData* data = (DataManagerData*)_data;
	data->DataManager->UpdateEntityTable(data->RuntimeInfo);
}

void ECSL::DataManagerClearDeadEntities(void* _data)
{
	DataManagerData* data = (DataManagerData*)_data;
	data->DataManager->ClearDeadEntities(data->RuntimeInfo);
}

void ECSL::SystemManagerUpdateSystemEntityLists(void* _data)
{
	UpdateSystemEntityListsData* data = (UpdateSystemEntityListsData*)_data;
	//data->SystemManager->UpdateSystemEntityLists(data->RuntimeInfo, data->GroupIndex);
}