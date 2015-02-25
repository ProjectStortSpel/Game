#ifndef FRAMECOUNTER_H
#define FRAMECOUNTER_H
#include <SDL/SDL.h>

namespace Utility
{ 
	class FrameCounter
	{
	public:
		FrameCounter();
		~FrameCounter();

		int GetFPS();
		int GetAverageFPS();
		float GetDeltaTime();
		float GetAverageDeltaTime();

		void Tick();
		
		void Reset();
		float GetMinDeltaTime();
		float GetMaxDeltaTime();
		
	private:
		static const unsigned int m_numberOfSamples = 128;

		float m_nrSampleDivided;
		float m_countsPerSecond;
		Uint64 m_frameTimes[m_numberOfSamples];
		unsigned int m_currentIndex = 0;
		Uint64 m_timeStamp;
	};
}





#endif