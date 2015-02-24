#ifndef TOUCH_H
#define TOUCH_H
#include <SDL/SDL.h>
#include "../InputEnums.h"
#include <map>

namespace Input
{
	struct FingerPosition
	{
		float x, y;
		FingerPosition()
		{
			this->x = 0.0f;
			this->y = 0.0f;
		}
		FingerPosition(float x, float y)
		{
			this->x = x;
			this->y = y;
		}
	};

	class DECLSPEC Touch
	{
	public:
		Touch();
		~Touch();

		InputState GetFingerState(SDL_FingerID _finger);
		void Update();
		void PollEvent(SDL_Event e);

		float GetX(SDL_FingerID _finger) { return m_positions.find(_finger) != m_positions.end() ? m_positions[_finger].x : 0.0f; }
		float GetY(SDL_FingerID _finger) { return m_positions.find(_finger) != m_positions.end() ? m_positions[_finger].y : 0.0f; }
		float GetdX(SDL_FingerID _finger) { return m_deltaPositions.find(_finger) != m_deltaPositions.end() ? m_deltaPositions[_finger].x : 0.0f; }
		float GetdY(SDL_FingerID _finger) { return m_deltaPositions.find(_finger) != m_deltaPositions.end() ? m_deltaPositions[_finger].y : 0.0f; }

	private:
		std::map<SDL_FingerID, bool> m_thisState;
		std::map<SDL_FingerID, bool> m_lastState;
		std::map<SDL_FingerID, bool> m_pressedState;
		std::map<SDL_FingerID, bool> m_releasedState;

		std::map<SDL_FingerID, FingerPosition> m_positions;
		std::map<SDL_FingerID, FingerPosition> m_deltaPositions;
	};
}

#endif