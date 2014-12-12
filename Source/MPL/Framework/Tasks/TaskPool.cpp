#include "TaskPool.h"

#include <SDL/SDL_mutex.h>

using namespace MPL;

TaskPool::TaskPool()
: m_tasks(new std::vector<Task*>())
{
	m_taskMutex = SDL_CreateMutex();
}

TaskPool::~TaskPool()
{
	if (m_taskMutex)
	{
		SDL_DestroyMutex(m_taskMutex);
		delete(m_taskMutex);
		m_taskMutex = 0;
	}
	delete(m_tasks);
}

TaskId TaskPool::GenerateId()
{
	return ++m_nextId;
}

void TaskPool::AddTask(Task* _task)
{
	SDL_LockMutex(m_taskMutex);
	m_tasks->push_back(_task);
	SDL_UnlockMutex(m_taskMutex);
}

void TaskPool::AddTasks(const std::vector<Task*>& _tasks)
{
	SDL_LockMutex(m_taskMutex);
	for (auto task : _tasks)
		m_tasks->push_back(task);
	SDL_UnlockMutex(m_taskMutex);
}

Task* TaskPool::GetTask()
{
	Task* task;
	SDL_LockMutex(m_taskMutex);
	if (m_tasks->size() != 0)
	{
		task = m_tasks->back();
		m_tasks->pop_back();
	}
	else
		task = 0;
	SDL_UnlockMutex(m_taskMutex);
	return task;
}

unsigned int TaskPool::GetTaskCount()
{
	return m_tasks->size();
}