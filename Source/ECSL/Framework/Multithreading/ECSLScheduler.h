#ifndef ECSLSCHEDULER_H
#define ECSLSCHEDULER_H

#include <vector>
#include "MPL/Framework/Tasks/Task.h"
#include "MPL/Interfaces/Scheduler.h"

namespace ECSL
{
	class DECLSPEC ECSLScheduler : public MPL::Scheduler
	{
	public:
		ECSLScheduler();
		~ECSLScheduler();

		void Execute();

	private:

	};
};

#endif