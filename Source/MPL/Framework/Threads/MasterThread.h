#ifndef MASTERTHREAD_H
#define MASTERTHREAD_H

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <vector>
#include <string>
#include "MPL/Framework/Tasks/Task.h"
#include "MPL/Framework/Tasks/TaskPool.h"
#include "MPL/Framework/Threads/Thread.h"

namespace MPL
{
	class MasterThread : public Thread
	{
	public:
		MasterThread();
		~MasterThread();

		void Deactivate();
		bool StartThread(const std::string& _name);
		void Execute();

	private:
		void ThreadLoop();
	};
};

#endif