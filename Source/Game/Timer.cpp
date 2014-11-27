#include "Timer.h"

Timer::Timer()
{
	__int64 countsPerSecond;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);
	m_secondsPerCount = 1.0f / (float)countsPerSecond;

	Reset();
}

#ifdef WIN32
void Timer::Reset()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_prevTimePoint);
}

float Timer::ElapsedTimeInMilliseconds()
{
	return 1000.0f * ElapsedTimeInSeconds();
}

float Timer::ElapsedTimeInSeconds()
{
	__int64 currTimePoint;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTimePoint);
	return (float)(currTimePoint - m_prevTimePoint) * m_secondsPerCount;
}
#else
void Timer::Reset()
{
	m_prevTimePoint = std::chrono::high_resolution_clock::now();
}

float Timer::ElapsedTimeInMilliseconds()
{
	return (float)std::chrono::duration_cast<std::chrono::microseconds>(
		std::chrono::high_resolution_clock::now() - m_prevTimePoint).count();
}

float Timer::ElapsedTimeInSeconds()
{
	return 0.001f * ElapsedTimeInMilliseconds();
}
#endif