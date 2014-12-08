#include "SlaveThread.h"

#include <assert.h>

using namespace MPL;

SlaveThread::SlaveThread()
{

}

SlaveThread::~SlaveThread()
{
	Release();
}

void SlaveThread::Release()
{
	if (m_thread)
	{
		SDL_DetachThread(m_thread);
		delete(m_thread);
		m_thread = 0;
		m_active = false;
		m_state = ThreadState::Dead;
	}
}

void SlaveThread::Deactivate()
{
	m_active = false;
}

bool SlaveThread::StartThread(const std::string& _name)
{
	m_active = true;
	m_thread = SDL_CreateThread(BeginThreadLoop, _name.c_str(), this);
	return m_thread;
}

void SlaveThread::Execute()
{
	/* Can't start execution if thread isn't in waiting state */
	assert(m_state == ThreadState::Waiting);

	m_state = ThreadState::Working;
}

int SlaveThread::BeginThreadLoop(void* _thread)
{
	return ((SlaveThread*)_thread)->ThreadLoop();
}

int SlaveThread::ThreadLoop()
{
	while (m_active)
	{
		while (m_state == ThreadState::Working)
		{
			/*
			** Perform task.
			** If no tasks are left: Tell other threads that this thread is done.
			** If all threads are done: Tell master to wake up.
			*/

			Task* currentTask = m_taskPool->GetTask();
			/* Task fetched successfully */
			if (currentTask)
			{
				/* PERFORM TASK */
			}
			/* No task available */
			else
				m_state = ThreadState::Waiting;

		}
	}
	Release();
	return 0;
}