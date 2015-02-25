#include "TaskManager.h"

#include "MPL/Framework/Common/ThreadHelper.h"

using namespace MPL;

TaskManager::TaskManager()
: m_slaves(new std::vector<SlaveThread*>()), m_taskPool(new TaskPool())
{

}

TaskManager::~TaskManager()
{
	for (auto slave : *m_slaves)
	{
		slave->Kill();
		slave->WakeUp();
	}
	for (auto slave : *m_slaves)
	{
		int status;
		SDL_WaitThread(slave->GetThread(), &status);
		if (status != 0)
			printf("Thread unsuccessfully destroyed\n");
		delete(slave);
	}
	delete(m_slaves);
	delete(m_taskPool);
	
	delete &ThreadLogger::GetInstance();
}

TaskManager& TaskManager::GetInstance()
{
	static TaskManager* instance = new TaskManager();
	return *instance;
}

void TaskManager::CreateSlaves()
{
	m_slaveCount = (unsigned int)(SDL_GetCPUCount() - 4) > SDL_GetCPUCount() ? 0 : SDL_GetCPUCount() - 4;
	/* Create slaves */
	for (unsigned int i = 0; i < m_slaveCount; ++i)
	{
		SlaveThread* slave = new SlaveThread(m_taskPool, m_slaves);
		/* Try create a slave and force it into a loop */
		if (!slave->StartThread("Slave " + i, i))
		{
			printf("Creation of slave thread was unsuccessful. Id: %i\n", i);
			delete(slave);
		}
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
	ThreadLogger* logger = &ThreadLogger::GetInstance();
	unsigned int threadId = GetThreadCount() - 1;

	while (!m_taskPool->IsTaskDone(_id))
	{
		FetchWorkStatus fetchWorkStatus;
		WorkItem* workItem = m_taskPool->FetchWork(fetchWorkStatus);
		if (fetchWorkStatus == OK)
		{
			logger->LogBeginWork(threadId);
			workItem->Work(workItem->Data);
			logger->LogWorkDone(threadId, workItem);
			WorkDoneStatus workDoneStatus = m_taskPool->WorkDone(workItem);
			if (!workDoneStatus.OpenListEmpty && workDoneStatus.TaskCompleted)
				WakeThreads();
		}
	}
}

void TaskManager::WakeThreads()
{
	for (auto slaveThread : *m_slaves)
		if (slaveThread->IsSleeping())
			slaveThread->WakeUp();
}