#ifndef GAMECREATOR_H
#define GAMECREATOR_H
#include <SDL/SDL.h>

#include "Renderer/GraphicDevice.h"
#include "Input/InputWrapper.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "LuaBridge/LuaBridge.h"
#include "ECSL/ECSL.h"

class GameCreator
{
public:
	GameCreator();
	~GameCreator();

	void InitializeGraphics();
	void InitializeInput();
	void InitializeNetwork();
	void InitializeWorld();
	void InitializeLua();
	

	void StartGame();

private:
	void PollSDLEvent();

private:
	Renderer::GraphicDevice*	m_graphics;
	Input::InputWrapper*		m_input;
	ECSL::World*				m_world;

};

#endif