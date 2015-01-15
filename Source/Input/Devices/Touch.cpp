#include "Touch.h"
#include <stdio.h>
using namespace Input;

Touch::Touch()
{
}

Touch::~Touch()
{
}

void Touch::Update()
{
	for (std::map<SDL_FingerID, bool>::iterator it = m_thisState.begin(); it != m_thisState.end(); it++)
		m_lastState[it->first] = it->second;

	m_deltaPositions.clear();
}

void Touch::PollEvent(SDL_Event e)
{
	switch (e.type)
	{
	case SDL_FINGERDOWN:
		m_thisState[e.tfinger.fingerId] = true;
		m_positions[e.tfinger.fingerId] = FingerPosition(e.tfinger.x, e.tfinger.y);
		break;

	case SDL_FINGERUP:
		m_thisState[e.tfinger.fingerId] = false;
		m_positions[e.tfinger.fingerId] = FingerPosition(e.tfinger.x, e.tfinger.y);
		break;

	case SDL_FINGERMOTION:
		m_positions[e.tfinger.fingerId] = FingerPosition(e.tfinger.x, e.tfinger.y);
		m_deltaPositions[e.tfinger.fingerId] = FingerPosition(e.tfinger.dx, e.tfinger.dy);
		break;
	}
}

InputState Touch::GetFingerState(SDL_FingerID _finger)
{
	bool tLast = m_lastState.find(_finger) != m_lastState.end() ? m_lastState[_finger] : false;
	bool tThis = m_thisState.find(_finger) != m_thisState.end() ? m_thisState[_finger] : false;

	if (tThis && !tLast)
		return InputState::PRESSED;

	else if (tThis && tLast)
		return InputState::DOWN;

	else if (!tThis && tLast)
		return InputState::RELEASED;

	return InputState::UP;
}