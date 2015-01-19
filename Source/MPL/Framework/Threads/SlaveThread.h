#ifndef SLAVETHREAD_H
#define SLAVETHREAD_H

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <chrono>
#include <vector>
#include <string>
#include "MPL/Framework/Tasks/Task.h"
#include "MPL/Framework/Tasks/TaskPool.h"

namespace MPL
{
	class SlaveThread
	{
	public:
		SlaveThread(TaskPool* _taskPool, std::vector<SlaveThread*>* _slaves);
		~SlaveThread();

		bool StartThread(const std::string& _name);
		void WakeUp();
		void Kill() { m_alive = false; }

		bool IsSleeping() { return m_sleeping; }
		SDL_Thread* GetThread() { return m_thread; }
		

	private:
		SDL_Thread* m_thread;
		TaskPool* m_taskPool;
		SDL_sem* m_sleepSem;
		bool m_sleeping;
		bool m_alive;
		std::vector<SlaveThread*>* m_slaves;

		static int BeginThreadLoop(void* _thread);
		int ThreadLoop();
		void Sleep();
		void WakeThreads();


	};
};

#endif