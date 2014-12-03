#ifndef SIMULATION_H
#define SIMULATION_H

#include <SDL/SDL.h>
#include "Components/DataManager.h"
#include "Systems/SystemWorkGroup.h"
#include "Systems/SystemManager.h"

namespace ECSL
{
	class DECLSPEC Simulation
	{
	private:
		DataManager* m_dataManager;
		SystemManager* m_systemManager;
	protected:

	public:
		Simulation(DataManager* _dataManager, SystemManager* _systemManager);
		~Simulation();

		void Update(float _dt);
	};
}

#endif