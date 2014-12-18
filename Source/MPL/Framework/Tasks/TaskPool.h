#ifndef TASKPOOL_H
#define TASKPOOL_H

#include <SDL/SDL.h>
#include <map>
#include <list>
#include <vector>
#include "Task.h"

namespace MPL
{
	class TaskPool
	{
	public:
		TaskPool();
		~TaskPool();

		TaskId BeginCreateTask(TaskId _dependency, WorkItem* _workItem);
		void FinishCreateTask(TaskId _id);
		void CreateChild(TaskId _parent, WorkItem* _workItem);
		void CreateChildren(TaskId _parent, const std::vector<WorkItem*>& _workItems);
		WorkItem* FetchWork();
		void WorkDone(WorkItem* _workItem);
		bool IsTaskDone(TaskId _id);

	private:
		TaskId m_nextId;
		SDL_mutex* m_idMutex;
		SDL_mutex* m_taskMutex;
		std::map<TaskId, Task*>* m_openList;
		std::list<WorkItem*>* m_queue;
		std::map<WorkItem*, Task*>* m_workTaskConnection;

		TaskId GenerateTaskId();
		void AddToOpenList(Task* _task);
		void AddToQueue(WorkItem* _workItem, Task* _task);
		void IncreaseWorkCount(Task* _task);
		void DecreaseWorkCount(Task* _task);
		void MoveFrontToBack(unsigned int _count);
	};
};

#endif