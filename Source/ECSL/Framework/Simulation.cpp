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
	/*	Update all systems	*/
	m_systemManager->Update(_dt);

	/*	Fetch messages sent from the systems	*/

	
	/*	Add/Remove entities	*/

}