#ifndef GAMECREATOR_H
#define GAMECREATOR_H
#include <SDL/SDL.h>
#include "Console/Console.h"
#include "TextInput.h"
#ifdef __ANDROID__
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif
#include "Input/InputWrapper.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "LuaBridge/LuaBridge.h"
#include "ECSL/ECSL.h"
#include "GameConsole.h"
#include "FrameCounter/FrameCounter.h"
#include "Game/RemoteConsole.h"

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
	

	void StartGame(int argc, char** argv);

private:


	void RunStartupCommands(int argc, char** argv);

	void PollSDLEvent();

	void UpdateNetwork(float _dt);
	void UpdateConsole();
	void RenderConsole();
	
	void GameMode(std::string _gamemode);
	void Reload();

	void ConsoleGameMode(std::string _command, std::vector<Console::Argument>* _args);
	void ConsoleReload(std::string _command, std::vector<Console::Argument>* _args);
	void ConsoleStartTemp(std::string _command, std::vector<Console::Argument>* _args);
	void ConsoleStopGame(std::string _command, std::vector<Console::Argument>* _args);

	void OnConnectedToServer(Network::NetConnection _nc, const char* _message);
	void LuaPacket(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
	void NetworkGameMode(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);

	void PrintSectionTime(const std::string& sectionName, Utility::FrameCounter* frameCounter, int x, int y);

private:
	Renderer::GraphicDevice*	m_graphics;
	Input::InputWrapper*		m_input;
	ECSL::World*				m_world;
	GameConsole*				m_console;
	RemoteConsole*				m_remoteConsole;

	Console::ConsoleManager&	m_consoleManager;
	TextInput					m_consoleInput;
	Utility::FrameCounter*		m_frameCounter;
	std::string					m_userName;
	std::string					m_gameMode;

	bool						m_running;
	
	Utility::FrameCounter m_inputCounter;
	Utility::FrameCounter m_worldCounter;
	Utility::FrameCounter m_networkCounter;
	Utility::FrameCounter m_graphicsCounter;

};

#endif