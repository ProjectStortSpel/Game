#include "Simulation.h"

#include <MPL/Managers/TaskManager.h>

using namespace ECSL;

Simulation::Simulation(DataManager* _dataManager, SystemManager* _systemManager, Scheduler* _scheduler)
:	m_dataManager(_dataManager), m_scheduler(_scheduler), m_systemManager(_systemManager)
{
	m_scheduler->AddUpdateTasks(*(m_systemManager->GetSystemWorkGroups()));
	m_scheduler->AddUpdateEntityTableTask();
	m_scheduler->AddClearDeadEntitiesTask();
}

Simulation::~Simulation()
{

}

void Simulation::Update(float _dt)
{
	/* Update dt for every work item */
	m_scheduler->UpdateDt(_dt);

	/* Update systems */
	MPL::TaskId lastUpdateTask = m_scheduler->ScheduleUpdate();

	/* Update entity component data*/
	MPL::TaskId updateEntityTable = m_scheduler->ScheduleUpdateEntityTable(lastUpdateTask);

	/* Clear dead entities from entity table */
	MPL::TaskId clearDeadEntities = m_scheduler->ScheduleClearDeadEntities(updateEntityTable);

	MPL::TaskManager::GetInstance().WaitFor(clearDeadEntities);

	/* Add and remove changed entities from systems */
	//m_systemManager->UpdateSystemEntityLists();

	/* Clear entity component data */
	//m_dataManager->ClearComponentData();

	/* Recycle the id back to the list of ids */
	//m_dataManager->RecycleEntityIds();

	/* Clear all the used lists */
	//m_dataManager->ClearChangeLists();
}