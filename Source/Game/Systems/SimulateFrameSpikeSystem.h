#ifndef SIMULATEFRAMESPIKESYSTEM_H
#define SIMULATEFRAMESPIKESYSTEM_H

#include "ECSL/Interfaces/System.h"

class SimulateFrameSpikeSystem : public ECSL::System
{
public:
	SimulateFrameSpikeSystem();
	~SimulateFrameSpikeSystem();

	void Initialize();
	void Update(const ECSL::RuntimeInfo& _runtime);

private:
	const float SpikesPerSecond = 1.0f;
	const float SpikeDuration = 0.3f;
	float m_spikeTimer;

	void ResetTimer();
};

#endif