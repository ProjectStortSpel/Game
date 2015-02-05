#ifndef GAMECONSOLE_H
#define GAMECONSOLE_H

#include "Console/Console.h"
#if defined(__ANDROID__) || defined(__IOS__)
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#include "Renderer/GraphicsHigh.h"
#include "Renderer/GraphicsLow.h"
#endif
#include "ECSL/ECSL.h"
#include "Network/NetworkInstance.h"

class GameConsole
{
public:
	GameConsole(Renderer::GraphicDevice* _graphics, ECSL::World* _world);
	~GameConsole();

	void SetGraphicDevice(Renderer::GraphicDevice* _graphics){ m_graphics = _graphics; }

	void SetupHooks(Console::ConsoleManager* _consoleManager);

	void AddPointlight(std::string _command, std::vector<Console::Argument>* _args);
	void RemovePointlight(std::string _command, std::vector<Console::Argument>* _args);

	void CreateObject(std::string _command, std::vector<Console::Argument>* _args);
	void RemoveObject(std::string _command, std::vector<Console::Argument>* _args);
	void AddComponent(std::string _command, std::vector<Console::Argument>* _args);
	void ChangeComponent(std::string _command, std::vector<Console::Argument>* _args);
	void RemoveComponent(std::string _command, std::vector<Console::Argument>* _args);
	void ListCommands(std::string _command, std::vector<Console::Argument>* _args);
	void ClearHistory(std::string _command, std::vector<Console::Argument>* _args);

	void HostServer(std::string _command, std::vector<Console::Argument>* _args);
	void StopServer(std::string _command, std::vector<Console::Argument>* _args);

	void ConnectClient(std::string _command, std::vector<Console::Argument>* _args);
	void DisconnectClient(std::string _command, std::vector<Console::Argument>* _args);

	void SetDebugTexture(std::string _command, std::vector<Console::Argument>* _args);

	void SetTextColor(std::string _command, std::vector<Console::Argument>* _args);
	void ToggleText(std::string _command, std::vector<Console::Argument>* _args);

	void ECSLMemoryUsage(std::string _command, std::vector<Console::Argument>* _args);

	void SetWorld(ECSL::World* _world){ m_world = _world; }
private:
	ECSL::World*	m_world;
	Renderer::GraphicDevice* m_graphics;
	Console::ConsoleManager* m_consoleManager;

};
#endif