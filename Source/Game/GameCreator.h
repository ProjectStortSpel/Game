#ifndef GAMECREATOR_H
#define GAMECREATOR_H
#include <SDL/SDL.h>

#include "Renderer/GraphicDevice.h"
#include "Input/InputWrapper.h"
#include "ECSL/ECSL.h"

class GameCreator
{
public:
	GameCreator();
	~GameCreator();

	void InitializeGraphics();
	void InitializeInput();
	void InitializeWorld();

	void StartGame();

private:
	void PollSDLEvent();

private:
	Renderer::GraphicDevice*	m_graphics;
	Input::InputWrapper*		m_input;
	ECSL::World*				m_world;

};

#endif