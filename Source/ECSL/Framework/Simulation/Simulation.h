#ifndef SIMULATION_H
#define SIMULATION_H

#include "ECSL/Framework/Components/DataManager.h"
#include "ECSL/Framework/Multithreading/Scheduler.h"
#include "ECSL/Framework/Systems/SystemWorkGroup.h"
#include "ECSL/Framework/Systems/SystemManager.h"

namespace ECSL
{
	class Simulation
	{
	public:
		Simulation();
		virtual ~Simulation() = 0;

		void Initialize(DataManager* _dataManager, SystemManager* _systemManager, MessageManager* _messageManager);

		virtual void Update(float _dt) = 0;

	protected:
		Scheduler* m_scheduler;

	private:
		DataManager* m_dataManager;
		SystemManager* m_systemManager;
		MessageManager* m_messageManager;
	};
}

#endif