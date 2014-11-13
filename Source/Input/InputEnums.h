#ifndef INPUTWRAPPER_INPUTENUMS_H
#define INPUTWRAPPER_INPUTENUMS_H
#include <SDL/SDL.h>

namespace Input
{
	enum DECLSPEC InputState
	{
		UP,
		PRESSED,
		DOWN,
		RELEASED
	};

	enum DECLSPEC Axis
	{
		X,
		Y
	};

	enum DECLSPEC MouseButton
	{
		LeftButton = SDL_BUTTON_LEFT,
		MiddleButton = SDL_BUTTON_MIDDLE,
		RightButton = SDL_BUTTON_RIGHT,
		Mouse4 = SDL_BUTTON_X1,
		Mouse5 = SDL_BUTTON_X2
	};
}

#endif