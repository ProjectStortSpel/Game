#ifndef TASKPOOL_H
#define TASKPOOL_H

#include <SDL/SDL.h>
#include <vector>
#include "Task.h"

namespace MPL
{
	class TaskPool
	{
	public:
		TaskPool();
		~TaskPool();

		TaskId GenerateId();
		void AddTask(Task* _task);
		void AddTasks(const std::vector<Task*>& _tasks);
		Task* GetTask();
		unsigned int GetTaskCount();

	private:
		TaskId m_nextId;
		SDL_mutex* m_taskMutex;
		std::vector<TaskId>* m_availableIds;
		std::vector<Task*>* m_tasks;
		
	};
};

#endif