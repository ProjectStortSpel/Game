#include  "Keyboard.h"
using namespace Input;

Keyboard::Keyboard()
{
	for (int i = 0; i < m_numberOfKeys; ++i)
	{
		m_thisState[i] = false;
		m_lastState[i] = false;
	}
	m_textInput = new std::string();
	SDL_StopTextInput();
}
Keyboard::~Keyboard()
{
	delete m_textInput;
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

		//text
		if (SDL_IsTextInputActive() && e.key.keysym.sym == SDLK_BACKSPACE && m_textInput->length() > 0)
		{
			*m_textInput = m_textInput->substr(0, m_textInput->length() - 1);
		}
		break;

	case SDL_KEYUP:
		m_thisState[e.key.keysym.scancode] = false;
		break;
	//text
	case SDL_TEXTINPUT:
		*m_textInput += e.text.text;
#if defined(__ANDROID__) || defined(__IOS__)
		SDL_Log("Text: %s", (*m_textInput).c_str());
#endif
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

void Keyboard::SetTextInput(const char* _text)
{
	*m_textInput = _text;
}