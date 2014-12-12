#include "TaskManager.h"

#include "MPL/Framework/Common/ThreadHelper.h"

using namespace MPL;

TaskManager::TaskManager()
: m_toBeAdded(new std::map<TaskId, Task*>()), m_taskPool(new TaskPool())
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
			m_slaves->push_back(slave);
	}
}

TaskId TaskManager::BeginAdd()
{
	
}

TaskId TaskManager::BeginAdd(WorkItem* _workItem)
{

}

void TaskManager::AddChild(TaskId _id, WorkItem* _workItem)
{

}

void TaskManager::DependsOn(TaskId _id, TaskId _dependsOnId)
{

}

void TaskManager::FinishAdd(TaskId _id)
{

}