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
	unsigned int availableThreadCount = ThreadHelper::GetAvailableThreadCount();
	for (unsigned int i = 0; i < availableThreadCount; ++i)
	{
		SlaveThread* slave = new SlaveThread(m_taskPool);
		if (!slave->StartThread("Slave " + i))
			printf("Couldn't create slave thread. Id: %i\n", i);
		else
		{
			printf("Slave thread successfully created. Id: %i\n", i);
			m_slaves->push_back(slave);
		}
	}
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
}

void TaskManager::AddChildren(TaskId _id, std::vector<WorkItem*>* _workItems)
{
	for (auto workItem : *_workItems)
		m_taskPool->CreateChild(_id, workItem);
}

void TaskManager::FinishAdd(TaskId _id)
{
	m_taskPool->FinishCreateTask(_id);
}

void TaskManager::WaitFor(TaskId _id)
{
	while (!m_taskPool->IsTaskDone(_id))
	{
		WorkItem* workItem = m_taskPool->FetchWork();
		if (workItem != 0)
		{
			workItem->Work(TaskInfo(0, 0), (void*)0);
			m_taskPool->WorkDone(workItem);
		}
	}
}