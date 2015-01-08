#include "FrameCounter.h"
#include <algorithm>
using namespace Utility;

FrameCounter::FrameCounter()
{
	m_nrSampleDivided = 1.0f / m_numberOfSamples;
	m_countsPerSecond = 1.0f / SDL_GetPerformanceFrequency();
	m_timeStamp = SDL_GetPerformanceCounter();
	m_currentIndex = 0.0f;

	for (int i = 0; i < m_numberOfSamples; ++i)
		m_frameTimes[i] = (float)(SDL_GetPerformanceFrequency());
}

FrameCounter::~FrameCounter()
{
}

void FrameCounter::Tick()
{
	Uint64	tTimestamp = SDL_GetPerformanceCounter();
	m_frameTimes[m_currentIndex] = tTimestamp - m_timeStamp;
	m_timeStamp = tTimestamp;

	++m_currentIndex;
	if (m_currentIndex == m_numberOfSamples)
		m_currentIndex = 0;
}

int FrameCounter::GetAverageFPS()
{
	Uint64 tTotal = 0;
	for (int i = 0; i < m_numberOfSamples; ++i)
		tTotal += m_frameTimes[i];

	return (int)(1.0f / (((float)tTotal * m_nrSampleDivided) * m_countsPerSecond));
}

int FrameCounter::GetFPS()
{
	float dt = m_frameTimes[m_currentIndex] * m_countsPerSecond;

	return (int)(1.0f / ((float)m_frameTimes[m_currentIndex] * m_countsPerSecond));
}

float FrameCounter::GetDeltaTime()
{
	return (float)m_frameTimes[m_currentIndex] * m_countsPerSecond;
}

float FrameCounter::GetAverageDeltaTime()
{
	Uint64 tTotal = 0;
	for (int i = 0; i < m_numberOfSamples; ++i)
		tTotal += m_frameTimes[i];

	return (((float)tTotal * m_nrSampleDivided) * m_countsPerSecond);
}

void FrameCounter::Reset()
{
	m_timeStamp = SDL_GetPerformanceCounter();
}

float FrameCounter::GetMinDeltaTime()
{
	return (float)*std::min_element(m_frameTimes, m_frameTimes + m_numberOfSamples) * m_countsPerSecond;
}

float FrameCounter::GetMaxDeltaTime()
{
	return (float)*std::max_element(m_frameTimes, m_frameTimes + m_numberOfSamples) * m_countsPerSecond;
}