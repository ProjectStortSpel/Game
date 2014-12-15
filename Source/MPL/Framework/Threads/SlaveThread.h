#ifndef SLAVETHREAD_H
#define SLAVETHREAD_H

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <vector>
#include <string>
#include "MPL/Framework/Tasks/Task.h"
#include "MPL/Framework/Tasks/TaskPool.h"

namespace MPL
{
	enum ThreadState
	{
		Waiting,
		Working,
		Dead
	};

	class SlaveThread
	{
	public:
		SlaveThread(TaskPool* _taskPool);
		~SlaveThread();

		bool StartThread(const std::string& _name);

		ThreadState GetState() { return m_state; }

	private:
		ThreadState m_state;
		SDL_Thread* m_thread;
		TaskPool* m_taskPool;

		static int BeginThreadLoop(void* _thread);
		int ThreadLoop();


	};
};

#endif