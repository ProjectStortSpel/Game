#ifndef MTSIMULATION_H
#define MTSIMULATION_H

#include "Simulation.h"

namespace ECSL
{
	class MTSimulation : public Simulation
	{
	private:

	protected:

	public:
		MTSimulation();
		~MTSimulation();

		void Update(float _dt);
	};
}

#endif