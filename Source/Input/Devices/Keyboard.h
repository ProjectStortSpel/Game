#ifndef INPUTWRAPPER_KEYBOARD_H
#define INPUTWRAPPER_KEYBOARD_H
#include <SDL/SDL.h>
#include "../InputEnums.h"

namespace Input
{
	class DECLSPEC Keyboard
	{
	public:
		Keyboard();
		~Keyboard();

		InputState GetKeyState(SDL_Scancode _key);
		void Update();
		void PollEvent(SDL_Event e);

	private:
		static const int m_numberOfKeys = SDL_NUM_SCANCODES;
		bool m_thisState[m_numberOfKeys];
		bool m_lastState[m_numberOfKeys];
	};
}

#endif