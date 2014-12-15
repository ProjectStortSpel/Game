#include "TaskManager.h"

#include "MPL/Framework/Common/ThreadHelper.h"

using namespace MPL;

TaskManager::TaskManager()
//: m_openList(new std::map<TaskId, Task*>()), m_taskPool(new TaskPool())
{
	m_toBeAddedMutex = SDL_CreateMutex();
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
			printf("Couldn't create slave thread. Id: %i", i);
		else
		{
			printf("Slave thread created. Id: %i", i);
			m_slaves->push_back(slave);
		}
	}
}

TaskId TaskManager::BeginAdd(TaskId _dependency)
{
	SDL_LockMutex(m_toBeAddedMutex);
//	TaskId id = m_taskPool->GenerateId();
//	(*m_openList)[id] = new Task();
	SDL_UnlockMutex(m_toBeAddedMutex);
	return 0;
}

TaskId TaskManager::BeginAdd(TaskId _dependency, WorkItem* _workItem)
{
	return 0;
}

void TaskManager::AddChild(TaskId _id, WorkItem* _workItem)
{

}

void TaskManager::AddChildren(TaskId _id, std::vector<WorkItem*>* _workItems)
{

}

void TaskManager::DependsOn(TaskId _id, TaskId _dependsOnId)
{

}

void TaskManager::FinishAdd(TaskId _id)
{

}