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
		MessageManager* m_messageManager;
	protected:

	public:
		Simulation(DataManager* _dataManager, SystemManager* _systemManager, MessageManager* _messageManager);
		~Simulation();

		void Update(float _dt);
	};
}

#endif