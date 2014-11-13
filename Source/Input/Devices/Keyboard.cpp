#include  "Keyboard.h"
using namespace Input;

Keyboard::Keyboard()
{
	for (int i = 0; i < m_numberOfKeys; ++i)
	{
		m_thisState[i] = false;
		m_lastState[i] = false;
	}
}
Keyboard::~Keyboard()
{
}

void Keyboard::Update()
{
	for (int i = 0; i < m_numberOfKeys; ++i)
		m_lastState[i] = m_thisState[i];
}

void Keyboard::PollEvent(SDL_Event e)
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
		m_thisState[e.key.keysym.scancode] = true;
		break;

	case SDL_KEYUP:
		m_thisState[e.key.keysym.scancode] = false;
		break;
	}
}

InputState Keyboard::GetKeyState(SDL_Scancode _key)
{
	bool tLast = m_lastState[_key];
	bool tThis = m_thisState[_key];

	if (tThis && !tLast)
		return InputState::PRESSED;

	else if (tThis && tLast)
		return InputState::DOWN;

	else if (!tThis && tLast)
		return InputState::RELEASED;

	return InputState::UP;
}