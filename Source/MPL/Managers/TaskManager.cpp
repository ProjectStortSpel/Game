#include "TaskManager.h"

#include "MPL/Framework/Common/ThreadHelper.h"

using namespace MPL;

TaskManager::TaskManager()
: m_slaves(new std::vector<SlaveThread*>()), m_taskPool(new TaskPool())
{

}

TaskManager::~TaskManager()
{

}

TaskManager& TaskManager::GetInstance()
{
	static TaskManager* instance = new TaskManager();
	return *instance;
}

void TaskManager::CreateSlaves()
{
	int availableThreadCount = SDL_GetCPUCount() - 3;//ThreadHelper::GetAvailableThreadCount();
	for (unsigned int i = 0; i < availableThreadCount; ++i)
	{
		SlaveThread* slave = new SlaveThread(m_taskPool, m_slaves);
		if (!slave->StartThread("Slave " + i))
			printf("Creation of slave thread was unsuccessful. Id: %i\n", i);
		else
		{
			printf("Slave thread successfully created. Id: %i\n", i);
			m_slaves->push_back(slave);
		}
	}
}

TaskId TaskManager::Add(TaskId _dependency, const std::vector<WorkItem*>& _workItems)
{
	TaskId id = m_taskPool->CreateTask(_dependency, _workItems);
	WakeThreads();
	return id;
}

TaskId TaskManager::BeginAdd(TaskId _dependency)
{
	return m_taskPool->BeginCreateTask(_dependency, 0);
}

TaskId TaskManager::BeginAdd(TaskId _dependency, WorkItem* _workItem)
{
	return m_taskPool->BeginCreateTask(_dependency, _workItem);
}

void TaskManager::AddChild(TaskId _id, WorkItem* _workItem)
{
	m_taskPool->CreateChild(_id, _workItem);
	WakeThreads();
}

void TaskManager::AddChildren(TaskId _id, const std::vector<WorkItem*>& _workItems)
{
	m_taskPool->CreateChildren(_id, _workItems);
	WakeThreads();
}

void TaskManager::FinishAdd(TaskId _id)
{
	m_taskPool->FinishCreateTask(_id);
}

void TaskManager::WaitFor(TaskId _id)
{
	while (!m_taskPool->IsTaskDone(_id))
	{
		FetchWorkStatus fetchWorkStatus;
		WorkItem* workItem = m_taskPool->FetchWork(fetchWorkStatus);
		if (fetchWorkStatus == OK)
		{
			workItem->Work(workItem->Data);
			m_taskPool->WorkDone(workItem);
		}
	}
}

void TaskManager::WakeThreads()
{
	for (auto slaveThread : *m_slaves)
		if (slaveThread->IsSleeping())
			slaveThread->WakeUp();
}