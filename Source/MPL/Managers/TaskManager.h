#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <SDL/SDL.h>
#include <vector>
#include <map>
#include "MPL/Framework/Tasks/TaskPool.h"
#include "MPL/Framework/Threads/SlaveThread.h"

namespace MPL
{
	class DECLSPEC TaskManager
	{
	public:
		~TaskManager();
		static TaskManager& GetInstance();

		void CreateSlaves();

		TaskId BeginAdd(TaskId _dependency);
		TaskId BeginAdd(TaskId _dependency, WorkItem* _workItem);
		void AddChild(TaskId _id, WorkItem* _workItem);
		void AddChildren(TaskId _id, std::vector<WorkItem*>* _workItems);
		void FinishAdd(TaskId _id);
		void WaitFor(TaskId _id);

		/* Number of created slave threads */
		unsigned int GetSlaveCount() { return (unsigned int)m_slaves->size(); }

		/* Number of created slaves plus main threads */
		unsigned int GetThreadCount() { return (unsigned int)m_slaves->size() + 1; }

	private:
		TaskManager();

		std::vector<SlaveThread*>* m_slaves;
		TaskPool* m_taskPool;
	};
};

#endif