#include "Mouse.h"
#include <stdio.h>
using namespace Input;

Mouse::Mouse()
{
	for (int i = 0; i < m_numberOfButtons; ++i)
	{
		m_thisState[i] = false;
		m_lastState[i] = false;
	}

	m_x = 0;
	m_y = 0;
	m_dx = 0;
	m_dy = 0;
	m_xScroll = 0;
	m_yScroll = 0;
}

Mouse::~Mouse()
{

}

void Mouse::Update()
{
	for (int i = 0; i < m_numberOfButtons; ++i)
		m_lastState[i] = m_thisState[i];

	m_dx = 0;
	m_dy = 0;
	m_xScroll = 0;
	m_yScroll = 0;
}

void Mouse::PollEvent(SDL_Event e)
{
	switch (e.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		m_thisState[e.button.button] = true;
		break;

	case SDL_MOUSEBUTTONUP:
		m_thisState[e.button.button] = false;
		break;

	case SDL_MOUSEMOTION:
		m_x = e.motion.x;
		m_y = e.motion.y;

		m_dx = e.motion.xrel;
		m_dy = e.motion.yrel;
		break;

	case SDL_MOUSEWHEEL:
		m_yScroll = e.wheel.y;
		m_xScroll = e.wheel.x;
		break;
	}
}

InputState Mouse::GetButtonState(MouseButton _button)
{
	bool tLast = m_lastState[_button];
	bool tThis = m_thisState[_button];

	if (tThis && !tLast)
		return InputState::PRESSED;

	else if (tThis && tLast)
		return InputState::DOWN;

	else if (!tThis && tLast)
		return InputState::RELEASED;

	return InputState::UP;
}