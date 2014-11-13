#ifndef INPUTWRAPPER_H
#define INPUTWRAPPER_H
#include "Devices/Keyboard.h"

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


		void Update(void);

		Keyboard* GetKeyboard();

	private:
		InputWrapper();
		static InputWrapper* m_iwInstance;

		Keyboard* m_keyboard;
	};
}

#endif