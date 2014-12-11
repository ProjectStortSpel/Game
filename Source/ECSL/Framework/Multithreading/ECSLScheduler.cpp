#include "ECSLScheduler.h"

using namespace ECSL;

ECSLScheduler::ECSLScheduler()
{
	m_tasks = new std::vector<MPL::Task*>(32);

	for (unsigned int i = 0; i < 32; ++i)
	{
		MPL::Task* task = new MPL::Task(SystemUpdate, (void*)0);
		(*m_tasks)[i] = task;
	}
}

ECSLScheduler::~ECSLScheduler()
{

}

void ECSLScheduler::Execute()
{
	SendTasks();
}

void ECSLScheduler::AddSystemGroupUpdate(std::vector<System*>* m_systems)
{
	//for (unsigned int i = 0; i < 32; ++i)
	//	SystemUpdate(MPL::TaskInfo(0, 0), (void*)0);

	Execute();
}

void ECSLScheduler::AddSystemGroupOnEntityAdded(std::vector<System*>* m_systems)
{
	
}

void ECSLScheduler::AddSystemGroupOnEntityRemoved(std::vector<System*>* m_systems)
{

}

void ECSL::SystemUpdate(const MPL::TaskInfo& _taskInfo, void* _data)
{
	for (unsigned int a = 0; a < 100; ++a)
		float abc = M_PI * 3.12321365 * 457.5476457457 / 3.349806346;
}