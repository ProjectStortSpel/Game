#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <SDL/SDL.h>
#include <vector>
#include "MPL/Framework/Tasks/TaskPool.h"
#include "MPL/Framework/Threads/SlaveThread.h"
#include "MPL/Interfaces/Scheduler.h"

namespace MPL
{
	class DECLSPEC TaskManager
	{
	public:
		~TaskManager();
		TaskManager& GetInstance();

		void CreateThreads();
		void SafeKillThreads();
		void Execute(const std::vector<Task*>& _tasks);

		unsigned int GetThreadCount() { return m_threadCount; }

	private:
		TaskManager();

		unsigned int m_threadCount;
		std::vector<Thread*>* m_threads;
		TaskPool* m_taskPool;
	};
};

#endif