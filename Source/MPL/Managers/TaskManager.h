#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <SDL/SDL.h>
#include <vector>
#include "MPL/Framework/Tasks/TaskPool.h"
#include "MPL/Framework/Threads/SlaveThread.h"
#include "MPL/Framework/Threads/MasterThread.h"
#include "MPL/Interfaces/Scheduler.h"

namespace MPL
{
	class DECLSPEC TaskManager
	{
	public:
		~TaskManager();
		static TaskManager& GetInstance();

		void CreateThreads();
		void SafeKillThreads();
		// All the threads including the main thread will perform the given tasks
		void Execute(const std::vector<Task*>& _tasks);
		// The slave threads will perform the given tasks. The main thread will continue as normal after adding the new tasks.
		void ExecuteSlaves(const std::vector<Task*>& _tasks);

		unsigned int GetThreadCount() { return m_threadCount; }
		unsigned int GetTaskCount() { return m_taskPool->GetTaskCount(); }

	private:
		TaskManager();

		unsigned int m_threadCount;
		std::vector<Thread*>* m_threads;
		TaskPool* m_taskPool;
	};
};

#endif