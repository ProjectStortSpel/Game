#include "SlaveThread.h"

#include <assert.h>

using namespace MPL;

SlaveThread::SlaveThread(TaskPool* _taskPool)
: m_taskPool(_taskPool)
{

}

SlaveThread::~SlaveThread()
{
	if (m_thread)
	{
		SDL_DetachThread(m_thread);
		m_thread = 0;
	}
}

bool SlaveThread::StartThread(const std::string& _name)
{
	m_thread = SDL_CreateThread(BeginThreadLoop, _name.c_str(), this);
	return (m_thread != 0);
}

int SlaveThread::BeginThreadLoop(void* _thread)
{
	return ((SlaveThread*)_thread)->ThreadLoop();
}

int SlaveThread::ThreadLoop()
{
	while (true)
	{
		WorkItem* workItem = m_taskPool->FetchWork();
		if (workItem != 0)
		{
			workItem->Work(workItem->Data);
			m_taskPool->WorkDone(workItem);
		}
	}
	return 0;
}