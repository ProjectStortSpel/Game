#ifndef SIMULATION_H
#define SIMULATION_H

#include <SDL/SDL.h>
#include "Components/DataManager.h"
#include "Multithreading/Scheduler.h"
#include "Systems/SystemWorkGroup.h"
#include "Systems/SystemManager.h"

namespace ECSL
{
	class DECLSPEC Simulation
	{
	private:
		Scheduler* m_scheduler;
		DataManager* m_dataManager;
		SystemManager* m_systemManager;
	protected:

	public:
		Simulation(DataManager* _dataManager, Scheduler* _scheduler, SystemManager* _systemManager);
		~Simulation();

		void Update(float _dt);
	};
}

#endif