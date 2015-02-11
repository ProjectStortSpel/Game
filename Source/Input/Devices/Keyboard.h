#ifndef INPUTWRAPPER_KEYBOARD_H
#define INPUTWRAPPER_KEYBOARD_H
#include <SDL/SDL.h>
#include <string>
#include "../InputEnums.h"

namespace Input
{
	class DECLSPEC Keyboard
	{
	public:
		Keyboard();
		~Keyboard();

		void StartTextInput();
		void StopTextInput();
		SDL_bool IsTextInputActive() { return SDL_IsTextInputActive(); }

		char* GetTextInput() { return (char*)m_textInput->c_str(); }
		void SetTextInput(const char* _text);
		void ResetTextInput() { *m_textInput = ""; }

		InputState GetKeyState(SDL_Scancode _key);
		void Update();
		void PollEvent(SDL_Event e);

	private:
		static const int m_numberOfKeys = SDL_NUM_SCANCODES;
		bool m_thisState[m_numberOfKeys];
		bool m_lastState[m_numberOfKeys];
		SDL_mutex*	m_textInputMutex;

		int	m_startUsingTextInput;
		int	m_stopUsingTextInput;

		std::string* m_textInput;
	};
}

#endif