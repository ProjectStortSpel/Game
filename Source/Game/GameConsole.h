#ifndef GAMECONSOLE_H
#define GAMECONSOLE_H

#include "Console/Console.h"
#include "Renderer/GraphicDevice.h"
#include "ECSL/ECSL.h"

class GameConsole
{
public:
	GameConsole(Renderer::GraphicDevice* _graphics, ECSL::World* _world);
	~GameConsole();

	void SetupHooks(Console::ConsoleManager* _consoleManager);

	void SpawnGraphicalObjectAt(std::vector<Console::Argument>* _args);

private:
	ECSL::World*	m_world;
	Renderer::GraphicDevice* m_graphics;
	Console::ConsoleManager* m_consoleManager;


};
#endif