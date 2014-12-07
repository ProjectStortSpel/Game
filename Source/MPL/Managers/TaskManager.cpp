#include "TaskManager.h"

using namespace MPL;

TaskManager::TaskManager()
: m_threads(new std::vector<Thread*>()), m_taskPool(new TaskPool())
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

void TaskManager::CreateThreads()
{
	m_threadCount = SDL_GetCPUCount();
	/* Create slave threads equal to the number of logical processors - 1 */
	for (unsigned int i = 0; i < m_threadCount - 1; ++i)
	{
		SlaveThread* slave = new SlaveThread();
		slave->Initialize(m_taskPool);
		if (slave->StartThread("TaskManager " + i))
			m_threads->push_back(slave);
		else
			delete(slave);
	}
}

void TaskManager::SafeKillThreads()
{
	/* Signal every thread that it should end its life cycle as soon as possible */
	for (auto thread : *m_threads)
		thread->Deactivate();

	/* Wait until all threads are dead */
	for (unsigned int activeThread = 0; activeThread < m_threads->size();)
		if (m_threads->at(activeThread)->GetState() == ThreadState::Dead)
			++activeThread;
}

void TaskManager::Execute(const std::vector<Task*>& _tasks)
{
	//m_threadsDone = SDL_Sem m_threadCount;
	m_taskPool->AddTasks(_tasks);
	for (auto thread : *m_threads)
		thread->Execute();
}