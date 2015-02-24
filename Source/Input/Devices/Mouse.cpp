#include "Mouse.h"
#include <stdio.h>
using namespace Input;

Mouse::Mouse()
{
	for (int i = 0; i < m_numberOfButtons; ++i)
	{
		m_thisState[i] = false;
		m_lastState[i] = false;
		m_pressedStates[i]	=	false;
		m_releasedStates[i]	=	false;
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
	{
		m_lastState[i] = m_thisState[i];
	
		m_pressedStates[i]	=	false;
		m_releasedStates[i]	=	false;
	}
		

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
		m_thisState[e.button.button]		=	true;

		if(!m_lastState[e.button.button])
			m_pressedStates[e.button.button]	=	true;
		break;

	case SDL_MOUSEBUTTONUP:
		m_thisState[e.button.button]		=	false;

		if(m_lastState[e.button.button])
			m_releasedStates[e.button.button]	=	true;
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

	if (m_pressedStates[_button])
		return InputState::PRESSED;
	else if (m_releasedStates[_button])
		return InputState::RELEASED;
	else if (m_thisState[_button] && m_lastState[_button])
		return InputState::DOWN;

	return InputState::UP;
}