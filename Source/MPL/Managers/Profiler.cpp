#include "Profiler.h"

#include "TaskManager.h"

using namespace MPL;

Profiler::Profiler()
{
	Initialize();
}

Profiler::~Profiler()
{
	DeleteSession();
}

Profiler& Profiler::GetInstance()
{
	static Profiler* instance = new Profiler();
	return *instance;
}

void Profiler::Initialize()
{
	m_frequency = SDL_GetPerformanceFrequency();
	unsigned int threadCount = TaskManager::GetInstance().GetThreadCount();
	m_currentSession = 0;
}

void Profiler::CreateNewSession()
{
	unsigned int threadCount = TaskManager::GetInstance().GetThreadCount();
	m_currentSession = new LoggedSession();
	m_currentSession->threadLogs = new std::vector<std::vector<LoggedAction*>*>(threadCount);
	for (unsigned int i = 0; i < threadCount; ++i)
	{
		(*m_currentSession->threadLogs)[i] = new std::vector<LoggedAction*>();
	}
	m_currentSession->sessionStartTime = Now();
}

LoggedSession* Profiler::PullSession()
{
	m_currentSession->sessionEndTime = Now();
	m_currentSession->sessionDuration = Duration(m_currentSession->sessionEndTime, m_currentSession->sessionStartTime);
	LoggedSession* currentSession = m_currentSession;
	m_currentSession = 0;
	return currentSession;
}

void Profiler::AddNewAction(unsigned int _threadId, LoggedAction* _loggedAction)
{
	std::vector<LoggedAction*>* actions = (*m_currentSession->threadLogs)[_threadId];
	/* Destroy the log if the max size is reached */
	if (actions->size() == LOG_MAX_SIZE)
	{
		DeleteSession();
		CreateNewSession();
	}
	//actions->push_back(_loggedAction);
	(*m_currentSession->threadLogs)[_threadId]->push_back(_loggedAction);
}

void Profiler::DeleteSession()
{
	if (m_currentSession)
	{
		delete(m_currentSession);
		m_currentSession = 0;
	}
}

void Profiler::LogBeginWork(unsigned int _threadId)
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

void Profiler::LogWorkDone(unsigned int _threadId, const WorkItem* _workItem)
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

float Profiler::Duration(Uint64 _end, Uint64 _start)
{
	Uint64 elapsedTime = _end - _start;
	elapsedTime *= 1000000;
	return (float)elapsedTime / m_frequency;
}

Uint64 Profiler::Now()
{
	return SDL_GetPerformanceCounter();
}