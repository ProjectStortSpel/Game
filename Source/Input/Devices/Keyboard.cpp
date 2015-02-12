#include  "Keyboard.h"
using namespace Input;

Keyboard::Keyboard()
{
	for (int i = 0; i < m_numberOfKeys; ++i)
	{
		m_thisState[i] = false;
		m_lastState[i] = false;
	}
	m_startUsingTextInput = 0;
	m_stopUsingTextInput = 0;
	m_textInput = new std::string();

	m_textInputMutex = SDL_CreateMutex();
	SDL_StopTextInput();
}
Keyboard::~Keyboard()
{
	delete m_textInput;
	SDL_DestroyMutex(m_textInputMutex);
}

void Keyboard::StartTextInput()
{
	SDL_LockMutex(m_textInputMutex);
	m_startUsingTextInput = m_stopUsingTextInput + 1;
	SDL_UnlockMutex(m_textInputMutex);
}
void Keyboard::StopTextInput()
{ 
	SDL_LockMutex(m_textInputMutex);
	m_stopUsingTextInput = m_startUsingTextInput + 1;
	SDL_UnlockMutex(m_textInputMutex);
}

void Keyboard::Update()
{
	for (int i = 0; i < m_numberOfKeys; ++i)
		m_lastState[i] = m_thisState[i];

	if (m_startUsingTextInput > m_stopUsingTextInput)
	{
		SDL_StartTextInput();
		if (m_stopUsingTextInput != 0)
			SDL_StopTextInput();
	}
	else if (m_stopUsingTextInput > m_startUsingTextInput)
	{
		SDL_StopTextInput();
		if (m_startUsingTextInput != 0)
			SDL_StartTextInput();
	}

	if (m_startUsingTextInput != 0 || m_stopUsingTextInput != 0)
	{
		m_startUsingTextInput = 0;
		m_stopUsingTextInput = 0;
	}
}

void Keyboard::PollEvent(SDL_Event e)
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
		m_thisState[e.key.keysym.scancode] = true;

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