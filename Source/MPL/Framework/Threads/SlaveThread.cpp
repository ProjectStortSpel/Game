#include "SlaveThread.h"

#include <assert.h>
#include "MPL/Managers/ThreadLogger.h"

using namespace MPL;

SlaveThread::SlaveThread(TaskPool* _taskPool, std::vector<SlaveThread*>* _slaves)
: m_taskPool(_taskPool), m_slaves(_slaves)
{

}

SlaveThread::~SlaveThread()
{
	if (m_thread)
	{
		SDL_DetachThread(m_thread);
		m_thread = 0;
	}
	SDL_DestroySemaphore(m_sleepSem);
}

bool SlaveThread::StartThread(const std::string& _name, unsigned int _threadId)
{
	m_threadId = _threadId;
	m_alive = true;
	m_sleeping = false;
	m_sleepSem = SDL_CreateSemaphore(0);
	m_logger = &ThreadLogger::GetInstance();
	m_thread = SDL_CreateThread(BeginThreadLoop, _name.c_str(), this);
	return (m_thread != 0);
}

void SlaveThread::WakeUp()
{
	SDL_SemPost(m_sleepSem);
}

int SlaveThread::BeginThreadLoop(void* _thread)
{
	return ((SlaveThread*)_thread)->ThreadLoop();
}

int SlaveThread::ThreadLoop()
{
	while (m_alive)
	{
		FetchWorkStatus fetchWorkStatus;
		WorkItem* workItem = m_taskPool->FetchWork(fetchWorkStatus);
		if (fetchWorkStatus == OK)
		{
			m_logger->LogBeginWork(m_threadId);
			workItem->Work(workItem->Data);
			m_logger->LogWorkDone(m_threadId, workItem);
			WorkDoneStatus workDoneStatus = m_taskPool->WorkDone(workItem);
			if (!workDoneStatus.OpenListEmpty && workDoneStatus.TaskCompleted)
				WakeThreads();
		}
		else
			Sleep();
	}
	return 0;
}

void SlaveThread::Sleep()
{
	m_sleeping = true;
	SDL_SemWait(m_sleepSem);
	m_sleeping = false;
}

void SlaveThread::WakeThreads()
{
	for (auto slaveThread : *m_slaves)
		if (slaveThread != this)
			if (slaveThread->IsSleeping())
				slaveThread->WakeUp();
}