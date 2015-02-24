#include "ThreadLogger.h"

#include "TaskManager.h"

using namespace MPL;

ThreadLogger::ThreadLogger()
{
	Initialize();
}

ThreadLogger::~ThreadLogger()
{
	DeleteSession();
}

ThreadLogger& ThreadLogger::GetInstance()
{
	static ThreadLogger* instance = new ThreadLogger();
	return *instance;
}

void ThreadLogger::Initialize()
{
	m_frequency = SDL_GetPerformanceFrequency();
	unsigned int threadCount = TaskManager::GetInstance().GetThreadCount();
	m_currentSession = 0;
}

void ThreadLogger::CreateNewSession()
{
	DeleteSession();

	m_currentSession = new LoggedSession();
	m_currentSession->threadCount = TaskManager::GetInstance().GetThreadCount();
	m_currentSession->threadLogs = new std::vector<std::vector<LoggedAction*>*>(m_currentSession->threadCount);
	for (unsigned int i = 0; i < m_currentSession->threadCount; ++i)
	{
		(*m_currentSession->threadLogs)[i] = new std::vector<LoggedAction*>();
	}
	m_currentSession->sessionStartTime = Now();
}

void ThreadLogger::EndSession()
{
	if (!m_currentSession)
		return;

	Uint64 now = Now();
	m_currentSession->sessionEndTime = now;
	m_currentSession->sessionDuration = Duration(m_currentSession->sessionEndTime, m_currentSession->sessionStartTime);

	/* Add the last overhead action to each thread (from last work -> end session) */
	for (unsigned int threadIndex = 0; threadIndex < m_currentSession->threadCount; ++threadIndex)
	{
		std::vector<LoggedAction*>* actions = (*m_currentSession->threadLogs)[threadIndex];

		LoggedAction* loggedAction = new LoggedAction();
		loggedAction->type = ActionType::OVERHEAD;
		loggedAction->creationTime = now;
		loggedAction->duration = actions->size() == 0
			? Duration(now, m_currentSession->sessionStartTime)
			: Duration(now, (*actions).back()->creationTime);
		AddNewAction(threadIndex, loggedAction);
	}
}

LoggedSession* ThreadLogger::PullSession()
{
	if (!m_currentSession)
		return 0;

	LoggedSession* currentSession = m_currentSession;
	m_currentSession = 0;
	return currentSession;
}

void ThreadLogger::AddNewAction(unsigned int _threadId, LoggedAction* _loggedAction)
{
	std::vector<LoggedAction*>* actions = (*m_currentSession->threadLogs)[_threadId];
	/* Ignore the action if the log is full */
	if (actions->size() == LOG_MAX_SIZE)
	{
		delete(_loggedAction);
		return;
	}
	actions->push_back(_loggedAction);
}

void ThreadLogger::DeleteSession()
{
	if (m_currentSession)
	{
		delete(m_currentSession);
		m_currentSession = 0;
	}
}

void ThreadLogger::LogBeginWork(unsigned int _threadId)
{
	if (!m_currentSession)
		return;

	Uint64 now = Now();
	std::vector<LoggedAction*>* actions = (*m_currentSession->threadLogs)[_threadId];

	LoggedAction* loggedAction = new LoggedAction();
	loggedAction->type = ActionType::OVERHEAD;
	loggedAction->creationTime = now;
	loggedAction->duration = actions->size() == 0
		? Duration(now, m_currentSession->sessionStartTime)
		: Duration(now, (*actions).back()->creationTime);
	AddNewAction(_threadId, loggedAction);
}

void ThreadLogger::LogWorkDone(unsigned int _threadId, const WorkItem* _workItem)
{
	if (!m_currentSession)
		return;

	Uint64 now = Now();
	std::vector<LoggedAction*>* actions = (*m_currentSession->threadLogs)[_threadId];

	LoggedAction* loggedAction = new LoggedAction();
	loggedAction->type = ActionType::WORK;
	loggedAction->creationTime = now;
	loggedAction->duration = Duration(now, (*actions).back()->creationTime);
	loggedAction->workItem = _workItem;
	AddNewAction(_threadId, loggedAction);
}

float ThreadLogger::Duration(Uint64 _end, Uint64 _start)
{
	Uint64 elapsedTime = _end - _start;
	elapsedTime *= 1000000;
	return (float)elapsedTime / m_frequency;
}

Uint64 ThreadLogger::Now()
{
	return SDL_GetPerformanceCounter();
}