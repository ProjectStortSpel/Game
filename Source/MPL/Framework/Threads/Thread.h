#ifndef THREAD_H
#define THREAD_H

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

	class Thread
	{
	public:
		Thread();
		virtual ~Thread() = 0;

		void Initialize(TaskPool* _taskPool);

		virtual void Deactivate() = 0;

		virtual bool StartThread(const std::string& _name) = 0;

		virtual void Execute() = 0;

		virtual void WakeUp();

		virtual void Sleep();

		ThreadState GetState() { return m_state; }

	protected:
		ThreadState m_state;
		void* m_data;
		TaskPool* m_taskPool;

	private:

	};
};

#endif