#include "TaskManager.h"

using namespace MPL;

TaskManager::TaskManager()
: m_threads(new std::vector<Thread*>()), m_taskPool(new TaskPool())
{
}

TaskManager::~TaskManager()
{
	SafeKillThreads();
	for (auto thread : *m_threads)
		delete(thread);
	delete(m_threads);
	delete(m_taskPool);
}

TaskManager& TaskManager::GetInstance()
{
	static TaskManager* instance = new TaskManager();
	return *instance;
}

void TaskManager::CreateThreads()
{
	m_threadCount = 4;//SDL_GetCPUCount();

	/* Create slave threads equal to the number of logical processors - 1 */
	for (unsigned int i = 0; i < m_threadCount - 1; ++i)
	{
		SlaveThread* slave = new SlaveThread();
		slave->Initialize(m_taskPool);
		if (slave->StartThread("TaskManager " + i))
		{
			printf("Added slave: %i\n", i);
			m_threads->push_back(slave);
		}
		else
		{
			printf("Failed to start slave thread, index: %i\n", i);
			delete(slave);
		}
	}

	/* Add the master thread last */
	MasterThread* master = new MasterThread();
	master->Initialize(m_taskPool);
	m_threads->push_back(master);
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
	/* Add new tasks to the task pool */
	m_taskPool->AddTasks(_tasks);

	/* Tell the threads its time to work */
	for (auto thread : *m_threads)
		thread->Execute();

	/* Wait for all slave threads to finish their tasks */
	for (unsigned int threadIndex = 0; threadIndex < m_threadCount - 1;)
		if (m_threads->at(threadIndex)->GetState() != ThreadState::Working)
			++threadIndex;
}

void TaskManager::ExecuteSlaves(const std::vector<Task*>& _tasks)
{
	/* Add new tasks to the task pool */
	m_taskPool->AddTasks(_tasks);

	/* Tell the slave threads its time to work */
	for (auto threadIt = m_threads->begin(); threadIt != m_threads->end() - 1; ++threadIt)
		(*threadIt)->Execute();
}

void TaskManager::WakeUp()
{
	for (auto thread : *m_threads)
		thread->WakeUp();
}

void TaskManager::Sleep()
{
	for (auto thread : *m_threads)
		thread->Sleep();
}