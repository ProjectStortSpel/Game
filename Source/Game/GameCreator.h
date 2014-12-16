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
#include "FrameCounter/FrameCounter.h"

class GameCreator
{
public:
	GameCreator();
	~GameCreator();

	void InitializeGraphics();
	void InitializeConsole();
	void InitializeInput();
	void InitializeNetwork();
	void InitializeWorld(std::string _gameMode);
	void InitializeLua();
	

	void StartGame();

private:
	void PollSDLEvent();

	void UpdateNetwork(float _dt);
	void UpdateConsole();
	void RenderConsole();
	
	void GameMode(std::vector<Console::Argument>* _args);
	void Reload(std::vector<Console::Argument>* _args);

	void OnConnectedToServer(Network::NetConnection _nc, const char* _message);
	void LuaPacket(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);

private:
	Renderer::GraphicDevice*	m_graphics;
	Input::InputWrapper*		m_input;
	ECSL::World*				m_world;
	GameConsole*				m_console;

	Console::ConsoleManager&	m_consoleManager;
	TextInput					m_consoleInput;
	Utility::FrameCounter*		m_frameCounter;
	std::string					m_userName;
	std::string					m_gameMode;
};

#endif