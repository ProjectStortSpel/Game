#ifndef INPUTWRAPPER_H
#define INPUTWRAPPER_H

#ifdef WIN32
	#include <VLD/vld.h>
#else
#endif

#include "Devices/Keyboard.h"
#include "Devices/Mouse.h"
#include "Devices/Touch.h"

#include <SDL/SDL.h>
#include <vector>
#include <string>

namespace Input
{
	class DECLSPEC InputWrapper
	{
	public:
		~InputWrapper();
		static InputWrapper& GetInstance();
		void PollEvent(SDL_Event& _e);

		void Update(void);

		Keyboard* GetKeyboard();
		Mouse* GetMouse();
		Touch* GetTouch();

	private:
		InputWrapper();

		Keyboard* m_keyboard;
		Mouse* m_mouse;
		Touch* m_touch;
	};
}

#endif