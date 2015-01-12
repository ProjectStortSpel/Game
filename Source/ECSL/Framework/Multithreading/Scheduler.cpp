#include "Scheduler.h"

#include "MPL/Managers/TaskManager.h"

using namespace ECSL;

Scheduler::Scheduler(DataManager* _dataManager, SystemManager* _systemManager)
:	m_dataManager(_dataManager),
	m_systemManager(_systemManager),
	m_updateWorkItems(new std::vector<std::vector<MPL::WorkItem*>*>()),
	m_updateSystemEntityListsWorkItems(new std::vector<MPL::WorkItem*>()),
	m_updateEntityTableWorkItems(new std::vector<MPL::WorkItem*>()),
	m_clearDeadEntitiesWorkItems(new std::vector<MPL::WorkItem*>())
{
	unsigned int workCount = MPL::TaskManager::GetInstance().GetThreadCount();
	m_entitiesToAddToSystems = new std::vector<std::vector<unsigned int>*>();
	m_entitiesToRemoveFromSystems = new std::vector<std::vector<unsigned int>*>();
	for (unsigned int i = 0; i < m_systemManager->GetSystems()->size(); ++i)
	{
		m_entitiesToAddToSystems->push_back(new std::vector<unsigned int>());
		m_entitiesToRemoveFromSystems->push_back(new std::vector<unsigned int>());
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

MPL::TaskId Scheduler::ScheduleUpdateSystems()
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

MPL::TaskId Scheduler::ScheduleUpdateSystemEntityLists(MPL::TaskId _dependency)
{
	MPL::TaskId updateSystemEntityLists = MPL::TaskManager::GetInstance().BeginAdd(_dependency);
	MPL::TaskManager::GetInstance().AddChildren(updateSystemEntityLists, *m_updateSystemEntityListsWorkItems);
	MPL::TaskManager::GetInstance().FinishAdd(updateSystemEntityLists);
	return updateSystemEntityLists;
}

MPL::TaskId Scheduler::ScheduleUpdateEntityTable(MPL::TaskId _dependency)
{
	MPL::TaskId updateEntityTable = MPL::TaskManager::GetInstance().BeginAdd(_dependency);
	MPL::TaskManager::GetInstance().AddChildren(updateEntityTable, *m_updateEntityTableWorkItems);
	MPL::TaskManager::GetInstance().FinishAdd(updateEntityTable);
	return updateEntityTable;
}

MPL::TaskId Scheduler::ScheduleOnEntityAddedTasks(MPL::TaskId _dependency)
{
	MPL::TaskId currentTaskId = _dependency;
	MPL::TaskId lastTaskId;
	/* Create tasks of each groups' work items. Each new task will have a dependency to the previous task */
	for (auto workItems : *m_onEntityAddedWorkItems)
	{
		/* Send the work items to the TaskManager and create a task out of it */
		lastTaskId = currentTaskId;
		currentTaskId = MPL::TaskManager::GetInstance().BeginAdd(lastTaskId);
		MPL::TaskManager::GetInstance().AddChildren(currentTaskId, *workItems);
		MPL::TaskManager::GetInstance().FinishAdd(currentTaskId);
	}
	return currentTaskId;
}

MPL::TaskId Scheduler::ScheduleOnEntityRemovedTasks(MPL::TaskId _dependency)
{
	MPL::TaskId currentTaskId = _dependency;
	MPL::TaskId lastTaskId;
	/* Create tasks of each groups' work items. Each new task will have a dependency to the previous task */
	for (auto workItems : *m_onEntityRemovedWorkItems)
	{
		/* Send the work items to the TaskManager and create a task out of it */
		lastTaskId = currentTaskId;
		currentTaskId = MPL::TaskManager::GetInstance().BeginAdd(lastTaskId);
		MPL::TaskManager::GetInstance().AddChildren(currentTaskId, *workItems);
		MPL::TaskManager::GetInstance().FinishAdd(currentTaskId);
	}
	return currentTaskId;
}

MPL::TaskId Scheduler::ScheduleClearDeadEntities(MPL::TaskId _dependency)
{
	MPL::TaskId clearDeadEntities = MPL::TaskManager::GetInstance().BeginAdd(_dependency);
	MPL::TaskManager::GetInstance().AddChildren(clearDeadEntities, *m_clearDeadEntitiesWorkItems);
	MPL::TaskManager::GetInstance().FinishAdd(clearDeadEntities);
	return clearDeadEntities;
}

void Scheduler::AddUpdateSystemsTasks()
{
	for (auto systemGroup : *m_systemManager->GetSystemWorkGroups())
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

void Scheduler::AddOnEntityAddedTasks()
{
	for (auto systemGroup : *m_systemManager->GetSystemWorkGroups())
	{
		std::vector<MPL::WorkItem*>* workItems = new std::vector<MPL::WorkItem*>();
		for (auto system : *systemGroup->GetSystems())
		{
			/* Create one work item for each systems' OnEntityAdded task */
			for (unsigned int taskIndex = 0; taskIndex < system->GetOnEntityAddedTaskCount(); ++taskIndex)
			{
				OnEntityAddedData* data = new OnEntityAddedData();
				data->System = system;
				data->RuntimeInfo.TaskIndex = taskIndex;
				data->RuntimeInfo.TaskCount = system->GetOnEntityAddedTaskCount();
				data->EntitiesToAddToSystems = m_entitiesToAddToSystems;
				MPL::WorkItem* workItem = new MPL::WorkItem();
				workItem->Work = &SystemOnEntityAdded;
				workItem->Data = data;
				workItems->push_back(workItem);
			}
		}
		m_onEntityAddedWorkItems->push_back(workItems);
	}
}

void Scheduler::AddOnEntityRemovedTasks()
{
	for (auto systemGroup : *m_systemManager->GetSystemWorkGroups())
	{
		std::vector<MPL::WorkItem*>* workItems = new std::vector<MPL::WorkItem*>();
		for (auto system : *systemGroup->GetSystems())
		{
			/* Create one work item for each systems' OnEntityAdded task */
			for (unsigned int taskIndex = 0; taskIndex < system->GetOnEntityRemovedTaskCount(); ++taskIndex)
			{
				OnEntityRemovedData* data = new OnEntityRemovedData();
				data->System = system;
				data->RuntimeInfo.TaskIndex = taskIndex;
				data->RuntimeInfo.TaskCount = system->GetOnEntityRemovedTaskCount();
				data->EntitiesToRemoveFromSystems = m_entitiesToRemoveFromSystems;
				MPL::WorkItem* workItem = new MPL::WorkItem();
				workItem->Work = &SystemOnEntityRemoved;
				workItem->Data = data;
				workItems->push_back(workItem);
			}
		}
		m_onEntityRemovedWorkItems->push_back(workItems);
	}
}

void Scheduler::AddUpdateSystemEntityListsTasks()
{
	const unsigned int updateSystemEntityListsWorkCount = MPL::TaskManager::GetInstance().GetThreadCount();

	for (unsigned int i = 0; i < updateSystemEntityListsWorkCount; ++i)
	{
		UpdateSystemEntityListsData* data = new UpdateSystemEntityListsData();
		data->SystemManager = m_systemManager;
		data->EntitiesToAddToSystems = m_entitiesToAddToSystems;
		data->EntitiesToRemoveFromSystems = m_entitiesToRemoveFromSystems;
		data->RuntimeInfo.TaskIndex = i;
		data->RuntimeInfo.TaskCount = updateSystemEntityListsWorkCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &SystemManagerUpdateSystemEntityLists;
		workItem->Data = data;
		m_updateSystemEntityListsWorkItems->push_back(workItem);
	}
}

void Scheduler::AddUpdateEntityTableTask()
{
	const unsigned int updateEntityTableWorkCount = 1;
	
	/* Add update entity component tasks*/
	for (unsigned int i = 0; i < updateEntityTableWorkCount; ++i)
	{
		DataManagerData* data = new DataManagerData();
		data->DataManager = m_dataManager;
		data->RuntimeInfo.TaskIndex = i;
		data->RuntimeInfo.TaskCount = updateEntityTableWorkCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &DataManagerUpdateEntityTable;
		workItem->Data = data;
		m_updateEntityTableWorkItems->push_back(workItem);
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
		m_clearDeadEntitiesWorkItems->push_back(workItem);
	}
}

void ECSL::SystemUpdate(void* _data)
{
	UpdateData* data = (UpdateData*)_data;
	data->System->Update(data->RuntimeInfo);
}

void ECSL::SystemOnEntityAdded(void* _data)
{
	OnEntityAddedData* data = (OnEntityAddedData*)_data;
	data->System->OnEntityAdded(onEntity->TaskIndex, onEntity->Dt, onEntity->EntityId);
}

void ECSL::SystemOnEntityRemoved(void* _data)
{
	OnEntityRemovedData* data = (OnEntityRemovedData*)_data;
	data->System->OnEntityRemoved(onEntity->TaskIndex, onEntity->Dt, onEntity->EntityId);
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
	data->SystemManager->UpdateSystemEntityLists(data->RuntimeInfo, *data->EntitiesToAddToSystems, *data->EntitiesToRemoveFromSystems);
}