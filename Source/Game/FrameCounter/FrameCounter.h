#ifndef FRAMECOUNTER_H
#define FRAMECOUNTER_H
#include <SDL/SDL.h>

namespace Utility
{ 
	class FrameCounter
	{
	public:
		~FrameCounter();

		static FrameCounter& GetInstance();

		int GetFPS();
		int GetAverageFPS();
		float GetDeltaTime();
		float GetAverageDeltaTime();

		void Tick();
	private:
		FrameCounter();

		static const unsigned int m_numberOfSamples = 128;

		float m_nrSampleDivided;
		float m_countsPerSecond;
		Uint64 m_frameTimes[m_numberOfSamples];
		unsigned int m_currentIndex = 0;
		Uint64 m_timeStamp;
	};
}





#endif