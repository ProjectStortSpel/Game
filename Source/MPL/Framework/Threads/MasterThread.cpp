#include "MasterThread.h"

#include <assert.h>

using namespace MPL;

MasterThread::MasterThread()
{

}

MasterThread::~MasterThread()
{

}

void MasterThread::Deactivate()
{

}

bool MasterThread::StartThread(const std::string& _name)
{
	return true;
}


void MasterThread::Execute()
{
	/* Master thread must be in a waiting state to be able to start working. */
	assert(m_state == ThreadState::Waiting);

	m_state = ThreadState::Working;

	ThreadLoop();
}

void MasterThread::ThreadLoop()
{
	/* 
	** Master thread won't stay in a loop forever. 
	** After all tasks are done it will go back to do normal master thread work. 
	*/
	while (m_state == ThreadState::Working)
	{
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