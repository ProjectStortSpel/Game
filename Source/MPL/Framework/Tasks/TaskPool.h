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

		void AddTask(Task* _task);
		void AddTasks(const std::vector<Task*>& _tasks);
		Task* GetTask();
		unsigned int GetTaskCount();

	private:
		SDL_mutex* m_taskMutex;
		std::vector<Task*>* m_tasks;
	};
};

#endif