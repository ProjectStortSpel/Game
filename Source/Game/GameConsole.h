#ifndef GAMECONSOLE_H
#define GAMECONSOLE_H

#include "Console/Console.h"
#include "Renderer/GraphicDevice.h"
#include "ECSL/ECSL.h"
#include "NetworkInstance.h"

class GameConsole
{
public:
	GameConsole(Renderer::GraphicDevice* _graphics, ECSL::World* _world);
	~GameConsole();

	void SetupHooks(Console::ConsoleManager* _consoleManager);

	void AddPointlight(std::vector<Console::Argument>* _args);
	void RemovePointlight(std::vector<Console::Argument>* _args);

	void CreateObject(std::vector<Console::Argument>* _args);
	void RemoveObject(std::vector<Console::Argument>* _args);
	void AddComponent(std::vector<Console::Argument>* _args);
	void ChangeComponent(std::vector<Console::Argument>* _args);
	void RemoveComponent(std::vector<Console::Argument>* _args);
	void ListCommands(std::vector<Console::Argument>* _args);

	void HostServer(std::vector<Console::Argument>* _args);
	void StopServer(std::vector<Console::Argument>* _args);

	void ConnectClient(std::vector<Console::Argument>* _args);
	void DisconnectClient(std::vector<Console::Argument>* _args);

	void SetDebugTexture(std::vector<Console::Argument>* _args);

	void SetTextColor(std::vector<Console::Argument>* _args);
	void ToggleText(std::vector<Console::Argument>* _args);

	void ECSLMemoryUsage(std::vector<Console::Argument>* _args);

	void SetWorld(ECSL::World* _world){ m_world = _world; }
private:
	ECSL::World*	m_world;
	Renderer::GraphicDevice* m_graphics;
	Console::ConsoleManager* m_consoleManager;

};
#endif