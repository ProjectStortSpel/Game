#include "Scheduler.h"

#include "MPL/Managers/TaskManager.h"

using namespace ECSL;

Scheduler::Scheduler(DataManager* _dataManager)
:	m_dataManager(_dataManager),
	m_updateWorkItems(new std::vector<std::vector<MPL::WorkItem*>*>())
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

void Scheduler::ScheduleUpdate(float _dt)
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
	/* Wait for the last task to finish before proceeding */
	MPL::TaskManager::GetInstance().WaitFor(currentTaskId);
}

void Scheduler::AddUpdateGroup(const std::vector<System*>& _systems)
{
	std::vector<MPL::WorkItem*>* workItems = new std::vector<MPL::WorkItem*>();
	for (auto system : _systems)
	{
		/* Create one work item for each system update task */
		for (unsigned int taskIndex = 0; taskIndex < system->GetUpdateTaskCount(); ++taskIndex)
		{
			MPL::WorkItem* workItem = new MPL::WorkItem();
			workItem->Work = &SystemUpdate;
			UpdateData* update = new UpdateData();
			update->System = system;
			update->RuntimeInfo.TaskIndex = taskIndex;
			update->RuntimeInfo.TaskCount = system->GetUpdateTaskCount();
			workItem->Data = update;
			workItems->push_back(workItem);
		}
	}
	m_updateWorkItems->push_back(workItems);
}

void Scheduler::AddOnEntityAddedGroup(const std::vector<System*>& _systemWorkGroup)
{
	
}

void Scheduler::AddOnEntityRemovedGroup(const std::vector<System*>& _systemWorkGroup)
{

}

void Scheduler::AddDataManagerTasks()
{

}

void SystemUpdate(void* _data)
{
	UpdateData* update = (UpdateData*)_data;
	update->System->Update(update->RuntimeInfo);
}

void SystemOnEntityAdded(void* _data)
{
	OnEntityData* onEntity = (OnEntityData*)_data;
	//onEntity->System->OnEntityAdded(onEntity->TaskIndex, onEntity->Dt, onEntity->EntityId);
}

void SystemOnEntityRemoved(void* _data)
{
	OnEntityData* onEntity = (OnEntityData*)_data;
	//onEntity->System->OnEntityRemoved(onEntity->TaskIndex, onEntity->Dt, onEntity->EntityId);
}