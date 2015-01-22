#ifndef PROFILER_H
#define PROFILER_H

#include <SDL/SDL.h>

#include "MPL/Framework/Tasks/WorkItem.h"

#define ACTIVE

namespace MPL
{
	class DECLSPEC Profiler
	{
	public:
		~Profiler();
		static Profiler& GetInstance();

		void LogDeadTime(unsigned int _threadId, float _time);
		void LogWorkTime(unsigned int _threadId, float _time);
		void LogWork(const WorkItem* _workItem, float _time);



	private:
		Profiler();
	};
};

#endif