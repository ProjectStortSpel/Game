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
		m_thread = 0;
		m_active = false;
		m_state = ThreadState::Dead;
	}
}

void SlaveThread::Deactivate()
{
	m_awake = false;
	m_active = false;
}

bool SlaveThread::StartThread(const std::string& _name)
{
	m_active = true;
	m_awake = false;
	m_semSleep = SDL_CreateSemaphore(0);
	m_thread = SDL_CreateThread(BeginThreadLoop, _name.c_str(), this);
	return (m_thread != 0);
}

void SlaveThread::Execute()
{
	if (m_state == ThreadState::Waiting)
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
		SDL_SemWait(m_semSleep);

		while (m_awake)
		{
			while (m_state == ThreadState::Working)
			{
				Task* currentTask = m_taskPool->GetTask();
				/* Task fetched successfully */
				if (currentTask)
				{
					currentTask->GetFunction()(TaskInfo(0, 0), (void*)0);
				}
				/* No task available */
				else
				{
					m_state = ThreadState::Waiting;
				}
			}
		}
	}
	Release();
	return 0;
}

void SlaveThread::WakeUp()
{
	m_awake = true;
	while (SDL_SemPost(m_semSleep) == -1);
}

void SlaveThread::Sleep()
{
	m_awake = false;
	m_state = ThreadState::Waiting;
	//if (SDL_SemTryWait(m_sleep) == -1)
	//	printf("FAIL!\n");
}