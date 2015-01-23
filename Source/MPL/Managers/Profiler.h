#ifndef PROFILER_H
#define PROFILER_H

#include <SDL/SDL.h>
#include <vector>
#include <chrono>

#include "MPL/Framework/Tasks/WorkItem.h"

typedef std::chrono::high_resolution_clock Clock;
typedef Clock::time_point Time;
typedef std::chrono::duration<int, std::nano> Nanoseconds;

#define ACTIVE
#define LOG_MAX_SIZE 1024

namespace MPL
{
	enum ActionType
	{
		OVERHEAD,
		WORK,
		HIBERNATE
	};

	struct DECLSPEC LoggedAction
	{
		ActionType Type;
		Time CreationTime;
		int Duration;
		const WorkItem* WorkItem;
	};

	struct DECLSPEC LoggedSession
	{
		Time SessionStartTime;
		Time SessionEndTime;
		int SessionDuration;
		std::vector<std::vector<LoggedAction*>*>* ThreadLogs;

		~LoggedSession()
		{
			for (auto threadLog : *ThreadLogs)
			{
				for (auto action : *threadLog)
					delete(action);
				delete(threadLog);
			}
			delete(ThreadLogs);
		}
	};

	class DECLSPEC Profiler
	{
	public:
		~Profiler();
		static Profiler& GetInstance();

		void LogBeginWork(unsigned int _threadId);
		void LogWorkDone(unsigned int _threadId, const WorkItem* _workItem);
		void LogBeginHibernate(unsigned int _threadId);
		void CreateNewSession();
		void PullSession();
		void DeleteSession();

	private:
		Profiler();

		std::vector<bool>* m_hibernating;
		std::vector<unsigned int>* m_logIndices;
		LoggedSession* m_currentSession;
		void Initialize();
		void AddNewAction(unsigned int _threadId, LoggedAction* _loggedAction);

		inline int Duration(Time _end, Time _start) { return std::chrono::duration_cast<Nanoseconds>(_end - _start).count(); }
	};
};

#endif