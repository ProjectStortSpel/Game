#ifndef SLAVETHREAD_H
#define SLAVETHREAD_H

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <vector>
#include <string>
#include "MPL/Framework/Tasks/Task.h"
#include "MPL/Framework/Tasks/TaskPool.h"
#include "MPL/Framework/Threads/Thread.h"

namespace MPL
{
	class SlaveThread : public Thread
	{
	public:
		SlaveThread();
		~SlaveThread();

		void Deactivate() { m_active = false; }

		bool StartThread(const std::string& _name);
		void Execute();

		ThreadState GetState() { return m_state; }

	private:
		SDL_Thread* m_thread;

		void Release();

		static int BeginThreadLoop(void* _thread);
		int ThreadLoop();
	};
};

#endif