#ifndef SQSIMULATION_H
#define SQSIMULATION_H

#include "Simulation.h"

namespace ECSL
{
	class SQSimulation : public Simulation
	{
	private:

	protected:

	public:
		SQSimulation();
		~SQSimulation();

		void Update(float _dt);
	};
}

#endif