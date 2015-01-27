#ifndef THREADLOGGER_H
#define THREADLOGGER_H

#include <SDL/SDL.h>
#include <vector>

#include "MPL/Framework/Tasks/WorkItem.h"

//typedef std::chrono::high_resolution_clock Clock;
//typedef Clock::time_point Time;
//typedef std::chrono::duration<int, std::micro> Nanoseconds;

#define ACTIVE
#define LOG_MAX_SIZE 1024

namespace MPL
{
	enum ActionType
	{
		OVERHEAD,
		WORK
	};

	struct DECLSPEC LoggedAction
	{
		ActionType type;
		Uint64 creationTime;
		float duration;
		const WorkItem* workItem;
	};

	struct DECLSPEC LoggedSession
	{
		unsigned int threadCount;
		Uint64 sessionStartTime;
		Uint64 sessionEndTime;
		float sessionDuration;
		std::vector<std::vector<LoggedAction*>*>* threadLogs;

		~LoggedSession()
		{
			for (auto threadLog : *threadLogs)
			{
				for (auto action : *threadLog)
					delete(action);
				delete(threadLog);
			}
			delete(threadLogs);
		}
	};

	class DECLSPEC ThreadLogger
	{
	public:
		~ThreadLogger();
		static ThreadLogger& GetInstance();

		void LogBeginWork(unsigned int _threadId);

		void LogWorkDone(unsigned int _threadId, const WorkItem* _workItem);

		/* Creates a new memory. Current sessions' memory will be cleared, but not a pulled sessions memory. */
		void CreateNewSession();

		/* Pulls the current session. Don't forget to free its memory. */
		LoggedSession* PullSession();

	private:
		ThreadLogger();

		Uint64 m_frequency;
		LoggedSession* m_currentSession;
		void Initialize();
		void DeleteSession();
		void AddNewAction(unsigned int _threadId, LoggedAction* _loggedAction);

		float Duration(Uint64 _end, Uint64 _start);
		Uint64 Now();
	};
};

#endif