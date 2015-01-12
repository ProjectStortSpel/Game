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

	/* Update entity table data */
	MPL::TaskId updateEntityTable = m_scheduler->ScheduleUpdateEntityTable(lastUpdateTask);

	/* Update system entity lists */
	MPL::TaskId updateSystemEntityLists = m_scheduler->ScheduleUpdateSystemEntityLists(updateEntityTable);

	/* Clear dead entities from entity table */
	MPL::TaskId clearDeadEntities = m_scheduler->ScheduleClearDeadEntities(updateSystemEntityLists);

	MPL::TaskManager::GetInstance().WaitFor(clearDeadEntities);

	/* Recycle all dead ids back to the list of available ids */
	m_dataManager->RecycleEntityIds();

	/* Clear all the used lists */
	m_dataManager->ClearChangeLists();
}