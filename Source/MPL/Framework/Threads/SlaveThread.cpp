#include "SlaveThread.h"

#include <assert.h>

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

bool SlaveThread::StartThread(const std::string& _name)
{
	m_sleepSem = SDL_CreateSemaphore(0);
	m_alive = true;
	m_sleeping = false;
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
			workItem->Work(workItem->Data);
			WorkDoneStatus workDoneStatus = m_taskPool->WorkDone(workItem);
			if (!workDoneStatus.OpenListEmpty && workDoneStatus.TaskCompleted)
				WakeThreads();
		}
		else //if (fetchWorkStatus == EMPTY_WORK_LIST)
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