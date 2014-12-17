#include "Simulation.h"
using namespace ECSL;

Simulation::Simulation(DataManager* _dataManager, SystemManager* _systemManager)
{
	m_dataManager = _dataManager;
	m_systemManager = _systemManager;
}

Simulation::~Simulation()
{

}

void Simulation::Update(float _dt)
{
	/* Add and remove changed entities from systems */
	m_systemManager->SystemEntitiesUpdate();

	/* Clear entity component data */
	m_dataManager->ClearComponentData();

	/* Recycle the id back to the list of ids */
	m_dataManager->RecycleEntityIds();

	/* Clear all the used lists */
	m_dataManager->ClearChangeLists();

	/* Update systems */
	m_systemManager->Update(_dt);
}