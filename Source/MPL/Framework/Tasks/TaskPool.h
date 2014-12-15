#ifndef TASKPOOL_H
#define TASKPOOL_H

#include <SDL/SDL.h>
#include <map>
#include <deque>
#include "Task.h"

namespace MPL
{
	class TaskPool
	{
	public:
		TaskPool();
		~TaskPool();

		TaskId CreateTask(TaskId _dependency, WorkItem* _workItem);
		void CreateChild(TaskId _parent, WorkItem* _workItem);
		TaskId GenerateTaskId();
		void AddToOpenList(Task* _task);
		void RemoveFromOpenList(Task* _task);
		void AddToQueue(WorkItem* _workItem, Task* _task);

	private:
		TaskId m_nextId;
		SDL_mutex* m_idMutex;
		SDL_mutex* m_workItemCountMutex;
		SDL_mutex* m_openListMutex;
		SDL_mutex* m_queueMutex;
		std::map<TaskId, Task*>* m_openList;
		std::deque<WorkItem*>* m_queue;
		std::map<WorkItem*, TaskId>* m_workTaskConnection;
		
	};
};

#endif