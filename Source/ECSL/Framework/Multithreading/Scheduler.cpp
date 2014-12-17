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
		MPL::TaskManager::GetInstance().AddChildren(currentTaskId, workItems);
		MPL::TaskManager::GetInstance().FinishAdd(currentTaskId);
	}

	return currentTaskId;
}

MPL::TaskId Scheduler::ScheduleUpdateEntityTable(MPL::TaskId _dependency)
{
	MPL::TaskId updateEntityComponents = MPL::TaskManager::GetInstance().BeginAdd(_dependency);
	MPL::TaskManager::GetInstance().AddChildren(updateEntityComponents, m_updateEntityComponentsWorkItems);
	MPL::TaskManager::GetInstance().FinishAdd(updateEntityComponents);
	return updateEntityComponents;
}

MPL::TaskId Scheduler::ScheduleClearDeadEntities(MPL::TaskId _dependency)
{
	MPL::TaskId clearDeadEntities = MPL::TaskManager::GetInstance().BeginAdd(_dependency);
	MPL::TaskManager::GetInstance().AddChildren(clearDeadEntities, m_clearDeadEntitiesWorkItems);
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

void Scheduler::AddCopyChangedEntitiesTask()
{
	CopyChangedEntitiesData* data = new CopyChangedEntitiesData();
	data->SystemManager = m_systemManager;
	data->RuntimeInfo.TaskIndex = 0;
	data->RuntimeInfo.TaskCount = 1;
	MPL::WorkItem* workItem = new MPL::WorkItem();
	workItem->Work = &SystemManagerCopyChangedEntities;
	workItem->Data = data;
	m_updateEntityComponentsWorkItems->push_back(workItem);
}

void Scheduler::AddUpdateGroupEntityListsTasks()
{
	const unsigned int taskCount = m_updateWorkItems->size();
	const unsigned int workCountPerTask = MPL::TaskManager::GetInstance().GetThreadCount();

	for (unsigned int taskIndex = 0; taskIndex < taskCount; ++taskIndex)
	{
		std::vector<MPL::WorkItem*>* workItems = new std::vector<MPL::WorkItem*>();
		for (unsigned int workIndex = 0; workIndex < workCountPerTask; ++workIndex)
		{
			UpdateGroupEntitiesListData* data = new UpdateGroupEntitiesListData();
			data->SystemManager = m_systemManager;
			data->GroupIndex = taskIndex;
			data->RuntimeInfo.TaskIndex = workIndex;
			data->RuntimeInfo.TaskCount = workCountPerTask;
			MPL::WorkItem* workItem = new MPL::WorkItem();
			workItem->Work = &SystemManagerCopyChangedEntities;
			workItem->Data = data;
			m_updateEntityComponentsWorkItems->push_back(workItem);
		}
		m_updateGroupEntityListsWorkItems->push_back(workItems);
	}
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

void ECSL::SystemManagerCopyChangedEntities(void* _data)
{
	CopyChangedEntitiesData* data = (CopyChangedEntitiesData*)_data;
	data->SystemManager->CopyChangedEntities(data->RuntimeInfo);
}

void ECSL::SystemManagerUpdateGroupEntitiesList(void* _data)
{
	UpdateGroupEntitiesListData* data = (UpdateGroupEntitiesListData*)_data;
	data->SystemManager->UpdateGroupEntityLists(data->RuntimeInfo, data->GroupIndex);
}