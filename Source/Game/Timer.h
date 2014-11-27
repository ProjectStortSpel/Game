#ifndef TIMER_H
#define TIMER_H

#ifdef WIN32
#include <Windows.h>
#include <cmath>
#else
#include <chrono>
#endif

class Timer
{
public:
	Timer();

	void Reset();

	float ElapsedTimeInMilliseconds();
	float ElapsedTimeInSeconds();

private:
#ifdef WIN32
	float m_secondsPerCount;
	__int64 m_prevTimePoint;
#else
	std::chrono::high_resolution_clock::time_point m_prevTimePoint;
#endif
};

#endif