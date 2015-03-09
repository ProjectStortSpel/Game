#ifndef GAMECREATOR_H
#define GAMECREATOR_H
#include <SDL/SDL.h>
#include "Console/Console.h"
#include "TextInput.h"
#if defined(__ANDROID__) || defined(__IOS__)
#include "AndroidRenderer/GraphicsHigh.h"
#include "AndroidRenderer/GraphicsLow.h"
#else
#include "Renderer/GraphicsLow.h"
#include "Renderer/GraphicsHigh.h"
#endif
#include "Input/InputWrapper.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "LuaBridge/LuaBridge.h"
#include "ECSL/ECSL.h"
#include "GameConsole.h"
#include "FrameCounter/FrameCounter.h"
#include "Game/RemoteConsole.h"
#include "Profilers/ECSLProfiler.h"
#include "Systems/GraphicalSystem.h"
#include "Audio/Audio.h"

class GameCreator
{
public:
	GameCreator();
	~GameCreator();

	bool InitializeGraphics();
	void InitializeConsole();
	void InitializeInput();
	void InitializeNetwork();
	void InitializeNetworkEvents(bool _allowEntities);
	void InitializeThreads();
    void InitializeLobby();
	//void InitializeLua();
	void InitializeAudio();
	

	void StartGame(int argc, char** argv);

private:

    enum WorldType
    {
        Server, Client
    };
    
    void InitializeWorld(std::string _gameMode, WorldType _worldType, bool _isMainWorld, bool _includeMasterServer);
    void InitializeLua(WorldType _worldType);
    
	void RunStartupCommands(int argc, char** argv);

	void PollSDLEvent();

	void UpdateNetwork(float _dt);
	void UpdateConsole();
	void RenderConsole();
	
	void GameMode(std::string _gamemode);
    
    //void ReloadServer();
    //void ReloadClient();
    
	void Reload();

	void ConsoleGameMode(std::string _command, std::vector<Console::Argument>* _args);
	void ConsoleHostSettings(std::string _command, std::vector<Console::Argument>* _args);
	void ConsoleName(std::string _command, std::vector<Console::Argument>* _args);
	void ConsoleReload(std::string _command, std::vector<Console::Argument>* _args);
	void ConsoleStartTemp(std::string _command, std::vector<Console::Argument>* _args);
	void ConsoleStopGame(std::string _command, std::vector<Console::Argument>* _args);

	void LuaPacket(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
	void NetworkGameMode(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);

	void PrintSectionTime(const std::string& sectionName, Utility::FrameCounter* frameCounter, int x, int y);

	void ChangeGraphicsSettings(std::string _command, std::vector<Console::Argument>* _args);

	void ChangeTimeScale(std::string _command, std::vector<Console::Argument>* _args);

	void OnBannedFromServer(Network::NetConnection _nc, const char* _message);
	void OnConnectedToServer(Network::NetConnection _nc, const char* _message);
	void OnDisconnectedFromServer(Network::NetConnection _nc, const char* _message);
	void OnFailedToConnect(Network::NetConnection _nc, const char* _message);
	void OnKickedFromServer(Network::NetConnection _nc, const char* _message);
	void OnPasswordInvalid(Network::NetConnection _nc, const char* _message);
	void OnRemotePlayerBanned(Network::NetConnection _nc, const char* _message);
	void OnRemotePlayerConnected(Network::NetConnection _nc, const char* _message);
	void OnRemotePlayerDisconnected(Network::NetConnection _nc, const char* _message);
	void OnRemotePlayerKicked(Network::NetConnection _nc, const char* _message);
	void OnServerFull(Network::NetConnection _nc, const char* _message);
	void OnTimedOutFromServer(Network::NetConnection _nc, const char* _message);

	void OnPlayerConnected(Network::NetConnection _nc, const char* _message);
	void OnPlayerDisconnected(Network::NetConnection _nc, const char* _message);
	void OnPlayerTimedOut(Network::NetConnection _nc, const char* _message);

private:
	Renderer::GraphicDevice*	m_graphics;
	Input::InputWrapper*		m_input;
	ECSL::World*				m_clientWorld;
    ECSL::World*				m_serverWorld;
	GameConsole*				m_console;
	RemoteConsole*				m_remoteConsole;

	Console::ConsoleManager&	m_consoleManager;
	TextInput					m_consoleInput;
	Utility::FrameCounter*		m_frameCounter;
	std::string					m_userName;
	std::string					m_gameMode;

	bool						m_running;
	
	Utility::FrameCounter m_inputCounter;
	Utility::FrameCounter m_clientWorldCounter;
	Utility::FrameCounter m_serverWorldCounter;
	Utility::FrameCounter m_networkCounter;
	Utility::FrameCounter m_graphicsCounter;
	Utility::FrameCounter m_luaGarbageCollectionCounter;

	Profilers::ECSLProfiler* m_clientWorldProfiler;
	Profilers::ECSLProfiler* m_serverWorldProfiler;
	Profilers::ECSLProfiler* m_activeWorldProfiler;

	lua_State* m_clientLuaState;
	lua_State* m_serverLuaState;

	std::vector<GraphicalSystem*> m_graphicalSystems;

	float	m_timeScale;



	// HOST SETTINGS
	std::string m_name;
	std::string m_map;
	int m_fillAI;
	int m_allowSpectators;

};

#endif