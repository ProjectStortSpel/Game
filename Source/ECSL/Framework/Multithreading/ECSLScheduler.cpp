#include "ECSLScheduler.h"

#include "MPL/Managers/TaskManager.h"

using namespace ECSL;

ECSLScheduler::ECSLScheduler()
{
	m_workItems = new std::vector<MPL::WorkItem*>(32);

	for (unsigned int i = 0; i < 32; ++i)
	{
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = SystemUpdate;
		workItem->Data = (void*)0;
		(*m_workItems)[i] = workItem;
	}
}

ECSLScheduler::~ECSLScheduler()
{

}

void ECSLScheduler::Execute()
{
	MPL::TaskId id = MPL::TaskManager::GetInstance().BeginAdd(MPL::NoDependency);
	MPL::TaskManager::GetInstance().AddChildren(id, m_workItems);
	MPL::TaskManager::GetInstance().FinishAdd(id);
	MPL::TaskManager::GetInstance().WaitFor(id);
}

void ECSLScheduler::AddSystemGroupUpdate(std::vector<System*>* m_systems)
{
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

}