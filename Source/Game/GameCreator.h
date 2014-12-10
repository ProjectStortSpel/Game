#ifndef GAMECREATOR_H
#define GAMECREATOR_H
#include <SDL/SDL.h>
#include "Console/Console.h"
#include "TextInput.h"
#include "Renderer/GraphicDevice.h"
#include "Input/InputWrapper.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "LuaBridge/LuaBridge.h"
#include "ECSL/ECSL.h"
#include "GameConsole.h"
#include "Network/ServerNetwork.h"
#include "Network/ClientNetwork.h"

class GameCreator
{
public:
	GameCreator();
	~GameCreator();

	void InitializeGraphics();
	void InitializeConsole();
	void InitializeInput();
	void InitializeNetwork();
	void InitializeWorld();
	void InitializeLua();
	

	void StartGame();

private:
	void PollSDLEvent();

	void UpdateConsole();
	void RenderConsole();

private:
	Renderer::GraphicDevice*	m_graphics;
	Input::InputWrapper*		m_input;
	ECSL::World*				m_world;
	GameConsole*				m_console;
	Network::ClientNetwork*		m_client;
	Network::ServerNetwork*		m_server;

	Console::ConsoleManager		m_consoleManager;
	TextInput					m_consoleInput;
};

#endif