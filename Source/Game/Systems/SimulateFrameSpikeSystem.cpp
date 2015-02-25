#include "SimulateFrameSpikeSystem.h"

#include <SDL/SDL.h>

SimulateFrameSpikeSystem::SimulateFrameSpikeSystem()
{
	ResetTimer();
}

SimulateFrameSpikeSystem::~SimulateFrameSpikeSystem()
{

}

void SimulateFrameSpikeSystem::Initialize()
{
	SetSystemName("SimulateFrameSpikeSystem");

	SetUpdateTaskCount(1);

	AddComponentTypeToFilter("Null", ECSL::FilterType::Mandatory);
}

void SimulateFrameSpikeSystem::Update(const ECSL::RuntimeInfo& _runtime)
{
	m_spikeTimer -= _runtime.Dt;
	if (m_spikeTimer <= 0.0f)
	{
		ResetTimer();
		Uint64 frequency = SDL_GetPerformanceFrequency();
		Uint64 start = SDL_GetPerformanceCounter();
		while (true)
		{
			Uint64 end = SDL_GetPerformanceCounter();
			if ((float)((end - start)) / frequency > SpikeDuration)
				break;
		}
	}
}

void SimulateFrameSpikeSystem::ResetTimer()
{
	m_spikeTimer = 1.0f / SpikesPerSecond;
}