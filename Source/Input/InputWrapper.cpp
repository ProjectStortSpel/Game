#include "InputWrapper.h"
#include <stdio.h>
using namespace Input;

InputWrapper* InputWrapper::m_iwInstance = 0;

#pragma region Constructor/Destructor and GetInstance
InputWrapper::InputWrapper()
{
	if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0)
	{
		std::string initErrorMessage = "Initializing SDL_INIT_GAMECONTROLLER failed!\n";
		printf(initErrorMessage.c_str());
	}

	m_keyboard = new Keyboard();
}
InputWrapper::~InputWrapper()
{
	delete m_keyboard;
	m_keyboard = 0;
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
	std::vector<SDL_Event> invalidEvents = std::vector<SDL_Event>();
	SDL_Event	tEvent;

	while (SDL_PollEvent(&tEvent))
	{
		switch (tEvent.type)
		{
			/*	MOUSE	*/
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEMOTION:
		case SDL_MOUSEWHEEL:

			break;

			/*	KEYBOARD	*/
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			m_keyboard->PollEvent(tEvent);
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

		default:
			invalidEvents.push_back(tEvent);
			break;
		}
	}

	//	Push back events that couldn't be parsed
	if (invalidEvents.size() > 0)
	{
		for (int n = 0; n < invalidEvents.size(); ++n)
			SDL_PushEvent(&invalidEvents[n]);

		invalidEvents.clear();
	}
}
#pragma endregion

#pragma region Get Devices
Keyboard* InputWrapper::GetKeyboard()
{
	return m_keyboard;
}
#pragma endregion
