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
	delete(m_logIndices);
}

Profiler& Profiler::GetInstance()
{
	static Profiler* instance = new Profiler();
	return *instance;
}

void Profiler::Initialize()
{
	unsigned int threadCount = TaskManager::GetInstance().GetThreadCount();
	m_logIndices = new std::vector<unsigned int>(threadCount);
	m_hibernating = new std::vector<bool>(threadCount);
	for (unsigned int i = 0; i < threadCount; ++i)
		(*m_hibernating)[i] = true;
	m_currentSession = 0;
}

void Profiler::CreateNewSession()
{
	unsigned int threadCount = TaskManager::GetInstance().GetThreadCount();
	m_currentSession = new LoggedSession();
	m_currentSession->ThreadLogs = new std::vector<std::vector<LoggedAction*>*>(threadCount);
	for (unsigned int i = 0; i < threadCount; ++i)
	{
		(*m_currentSession->ThreadLogs)[i] = new std::vector<LoggedAction*>(LOG_MAX_SIZE);
		(*m_logIndices)[i] = 0;
	}
	m_currentSession->SessionStartTime = Clock::now();
}

void Profiler::AddNewAction(unsigned int _threadId, LoggedAction* _loggedAction)
{
	unsigned int logIndex = (*m_logIndices)[_threadId];

	/* Clear the vector if the log size is too big */
	if (logIndex == LOG_MAX_SIZE)
	{
		DeleteSession();
		CreateNewSession();
	}

	(*(*m_currentSession->ThreadLogs)[_threadId])[logIndex] = _loggedAction;
	++(*m_logIndices)[_threadId];
}

void Profiler::PullSession()
{

	//m_currentSession->m_log = new std::vector<
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

	Time now = Clock::now();
	unsigned int logIndex = (*m_logIndices)[_threadId] - 1;
	std::vector<LoggedAction*>* actions = (*m_currentSession->ThreadLogs)[_threadId];

	LoggedAction* loggedAction = new LoggedAction();
	loggedAction->Type = (*m_hibernating)[_threadId] ? ActionType::HIBERNATE : ActionType::OVERHEAD;
	loggedAction->CreationTime = now;
	loggedAction->Duration = (logIndex == -1)
		? Duration(now, m_currentSession->SessionStartTime)
		: Duration(now, (*actions)[logIndex]->CreationTime);
	AddNewAction(_threadId, loggedAction);

	if ((*m_hibernating)[_threadId])
		(*m_hibernating)[_threadId] = false;
}

void Profiler::LogWorkDone(unsigned int _threadId, const WorkItem* _workItem)
{
	if (!m_currentSession)
		return;

	Time now = Clock::now();
	unsigned int logIndex = (*m_logIndices)[_threadId] - 1;
	std::vector<LoggedAction*>* actions = (*m_currentSession->ThreadLogs)[_threadId];

	LoggedAction* loggedAction = new LoggedAction();
	loggedAction->Type = ActionType::WORK;
	loggedAction->CreationTime = now;
	loggedAction->Duration = Duration(now, (*actions)[logIndex]->CreationTime);
	loggedAction->WorkItem = _workItem;
	AddNewAction(_threadId, loggedAction);
}

void Profiler::LogBeginHibernate(unsigned int _threadId)
{
	(*m_hibernating)[_threadId] = true;
}