#include "TaskPool.h"

#include <SDL/SDL_mutex.h>

using namespace MPL;

TaskPool::TaskPool()
{
	m_taskMutex = SDL_CreateMutex();
}

TaskPool::~TaskPool()
{

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