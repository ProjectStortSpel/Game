#include "Scheduler.h"

#include "MPL/Managers/TaskManager.h"

using namespace MPL;

Scheduler::Scheduler()
{

}

Scheduler::~Scheduler()
{

}

void Scheduler::SendTasks()
{
	TaskManager::GetInstance().Execute(*m_tasks);
}