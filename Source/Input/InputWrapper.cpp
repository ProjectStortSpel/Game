#include "InputWrapper.h"
#include <stdio.h>
using namespace Input;

#pragma region Constructor/Destructor and GetInstance
InputWrapper::InputWrapper()
{
	if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0)
	{
		std::string initErrorMessage = "Initializing SDL_INIT_GAMECONTROLLER failed!\n";
		printf("%s", initErrorMessage.c_str());
	}

	m_keyboard = new Keyboard();
	m_mouse = new Mouse();
}
InputWrapper::~InputWrapper()
{
	if (m_keyboard)
	{
		delete m_keyboard;
		m_keyboard = 0;
	}
	if (m_mouse)
	{
		delete m_mouse;
		m_mouse = 0;
	}

}

InputWrapper& InputWrapper::GetInstance()
{
	static InputWrapper* instance = new InputWrapper();
	return *instance;
}
#pragma endregion

#pragma region Update
void InputWrapper::Update()
{
	m_keyboard->Update();
	m_mouse->Update();
}
#pragma endregion

#pragma region Get Devices
Keyboard* InputWrapper::GetKeyboard()
{
	return m_keyboard;
}
Mouse* InputWrapper::GetMouse()
{
	return m_mouse;
}
#pragma endregion

#pragma region Poll SDL Event
void InputWrapper::PollEvent(SDL_Event& _e)
{
	switch (_e.type)
	{
		/*	MOUSE	*/
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
	case SDL_MOUSEMOTION:
	case SDL_MOUSEWHEEL:
		m_mouse->PollEvent(_e);
		break;

		/*	KEYBOARD	*/
	case SDL_KEYDOWN:
	case SDL_KEYUP:
	case SDL_TEXTINPUT:
		m_keyboard->PollEvent(_e);
		break;

		/*	CONTROLLER	*/
	case SDL_CONTROLLERDEVICEADDED:

		break;
	case SDL_CONTROLLERDEVICEREMOVED:

		break;
	case SDL_CONTROLLERDEVICEREMAPPED:

		break;

	case SDL_CONTROLLERBUTTONDOWN:
	case SDL_CONTROLLERBUTTONUP:

		break;

	case SDL_CONTROLLERAXISMOTION:

		break;
	}

}
#pragma endregion
