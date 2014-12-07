#ifndef TASKPOOL_H
#define TASKPOOL_H

#include <SDL/SDL.h>
#include <vector>
#include "Task.h"

namespace MPL
{
	class DECLSPEC TaskPool
	{
	public:
		TaskPool();
		~TaskPool();

		void AddTasks(const std::vector<Task*>& _tasks) { m_tasks = new std::vector<Task*>(_tasks); }
		Task* GetTask();

	private:
		SDL_mutex* m_taskMutex;
		std::vector<Task*>* m_tasks;
	};
};

#endif