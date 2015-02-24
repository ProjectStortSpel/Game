#ifndef MOUSE_H
#define MOUSE_H
#include <SDL/SDL.h>
#include "../InputEnums.h"

namespace Input
{
	class DECLSPEC Mouse
	{
	public:
		Mouse();
		~Mouse();

		InputState GetButtonState(MouseButton _button);
		void Update();
		void PollEvent(SDL_Event e);

		int GetX(){ return m_x; }
		int GetY(){ return m_y; }
		int GetdX(){ return m_dx; }
		int GetdY(){ return m_dy; }
		int GetScroll(Axis _axis){ return (_axis == Axis::X) ? m_xScroll : m_yScroll; }

		void SetPosition(int _x, int _y)
		{
			SDL_WarpMouseInWindow(NULL, _x, _y);
			m_x = _x;
			m_y = _y;
		}

		void HideCursor(bool _hide)
		{
			if (_hide)
				SDL_ShowCursor(SDL_DISABLE);
			else
				SDL_ShowCursor(SDL_ENABLE);
		}
	private:
		static const int m_numberOfButtons = 6;
		bool m_thisState[m_numberOfButtons];
		bool m_lastState[m_numberOfButtons];
		bool m_pressedStates[m_numberOfButtons];
		bool m_releasedStates[m_numberOfButtons];

		int m_x, m_y;
		int m_dx, m_dy;
		int m_xScroll, m_yScroll;
	};
}

#endif