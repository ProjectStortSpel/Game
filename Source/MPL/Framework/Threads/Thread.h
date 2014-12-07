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

		void Deactivate() { m_active = false; }

		void Initialize(TaskPool* _taskPool);

		virtual bool StartThread(const std::string& _name) = 0;

		virtual void Execute() = 0;

		ThreadState GetState() { return m_state; }

	protected:
		ThreadState m_state;
		bool m_active;
		void* m_data;
		TaskPool* m_taskPool;

	private:

	};
};

#endif