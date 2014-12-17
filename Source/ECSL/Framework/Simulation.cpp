#include "Simulation.h"
using namespace ECSL;

Simulation::Simulation(DataManager* _dataManager, Scheduler* _scheduler, SystemManager* _systemManager)
:	m_dataManager(_dataManager), m_scheduler(_scheduler), m_systemManager(_systemManager)
{

}

Simulation::~Simulation()
{

}

void Simulation::Update(float _dt)
{
	/* Update systems */
	m_scheduler->ScheduleUpdate(_dt);

	/* Add and remove changed entities from systems */
	m_systemManager->SystemEntitiesUpdate();

	/* Clear entity component data */
	m_dataManager->ClearComponentData();

	/* Recycle the id back to the list of ids */
	m_dataManager->RecycleEntityIds();

	/* Clear all the used lists */
	m_dataManager->ClearChangeLists();
}