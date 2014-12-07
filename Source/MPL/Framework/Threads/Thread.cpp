#include "Thread.h"

#include <assert.h>

using namespace MPL;

Thread::Thread()
{

}

Thread::~Thread()
{
	
}

void Thread::Initialize(TaskPool* _taskPool)
{
	m_taskPool = _taskPool;
	m_data = 0;
}