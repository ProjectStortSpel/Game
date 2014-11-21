#include  "Controller.h"
#include <stdio.h>
using namespace Input;

float Controller::m_axisNormalizeValue = 1.0f / 32767.0f;

Controller::Controller(SDL_GameController* _gameController)
{
	for (int i = 0; i < m_numberOfButtons; ++i)
	{
		m_thisState[i] = false;
		m_lastState[i] = false;
	}

	for (int n = 0; n < SDL_CONTROLLER_AXIS_MAX; ++n)
	{
		m_axisValues[n] = 0.0f;
		m_axisDeadZones[n] = 0.1f;
	}

	m_gameController = _gameController;
}
Controller::~Controller()
{

}

void Controller::Update()
{
	for (int i = 0; i < m_numberOfButtons; ++i)
		m_lastState[i] = m_thisState[i];
}

void Controller::PollEvent(SDL_Event e)
{
	switch (e.type)
	{
	case SDL_CONTROLLERBUTTONDOWN:
		m_thisState[e.cbutton.button] = true;
		break;

	case SDL_CONTROLLERBUTTONUP:
		m_thisState[e.cbutton.button] = false;
		break;
		
	case SDL_CONTROLLERAXISMOTION:
		Uint8	axisID = e.caxis.axis;
		float	axisValue = e.caxis.value * m_axisNormalizeValue;
		m_axisDeadZones[axisID] = (m_axisDeadZones[axisID] < SDL_fabs(axisValue)) ? axisValue : 0.0f;
	}
}

InputState Controller::GetButtonState(SDL_GameControllerButton _button)
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

SDL_GameController* Controller::GetController()
{
	return m_gameController;
}