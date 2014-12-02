#ifndef INPUTWRAPPER_H
#define INPUTWRAPPER_H
#include "Devices/Keyboard.h"
#include "Devices/Mouse.h"

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

	private:
		InputWrapper();

		Keyboard* m_keyboard;
		Mouse* m_mouse;
	};
}

#endif