#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <SDL/SDL.h>
#include "../InputEnums.h"

namespace Input
{
	class DECLSPEC Controller
	{
	public:
		Controller(SDL_GameController* _gameController);
		~Controller();

		InputState GetButtonState(SDL_GameControllerButton _button);
		void Update();
		void PollEvent(SDL_Event e);
		SDL_GameController* GetController();

	private:
		static const int m_numberOfButtons = SDL_CONTROLLER_BUTTON_MAX;
		static float m_axisNormalizeValue;
		float	m_axisValues[SDL_CONTROLLER_AXIS_MAX];
		float	m_axisDeadZones[SDL_CONTROLLER_AXIS_MAX];
		bool	m_thisState[m_numberOfButtons];
		bool	m_lastState[m_numberOfButtons];

		SDL_GameController* m_gameController;
	};
}

#endif