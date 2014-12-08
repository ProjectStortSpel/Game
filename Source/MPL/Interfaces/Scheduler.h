#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <SDL/SDL.h>
#include <vector>
#include "MPL/Framework/Tasks/Task.h"

namespace MPL
{
	class DECLSPEC Scheduler
	{
	public:
		Scheduler();
		virtual ~Scheduler() = 0;

		virtual void Execute() = 0;

		// Send tasks in list to the Task Manager and execute.
		void SendTasks();

	protected:
		std::vector<Task*>* m_tasks;

	private:
		
	};
};

#endif