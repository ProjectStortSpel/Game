#include <SDL/SDL.h>
#include <iostream>
#include <string>
#include <algorithm>
#include "Network/ServerNetwork.h"
#include "Network/ClientNetwork.h"
#include "Network/PacketHandler.h"
#include "Timer.h"
#include "ECSL/ECSL.h"
#include "Input/InputWrapper.h"
#include "Renderer/GraphicDevice.h"

#ifdef WIN32
#define _CRTDBG_MAP_ALLOC

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG


#include <stdlib.h>
#include <crtdbg.h>
#endif

int CardsInHand = 8;
int CardsToPlay = 5;
int waitforplayers;

std::string myName = "";

void ClearConsole()
{
#ifdef WIN32
	system("cls");
#else
	system("clear");
#endif
}

void TestECSL()
{
	//ECSL::ComponentTypeManager::GetInstance().LoadComponentTypesFromDirectory("content/components");
	//ECSL::WorldCreator worldCreator = ECSL::WorldCreator();
	//worldCreator.AddSystemGroup();
	//worldCreator.AddComponentType("Position");
	//worldCreator.AddComponentType("Velocity");

	//ECSL::World* world = worldCreator.CreateWorld(100);

	//int id = world->CreateNewEntity();
	//world->CreateComponentAndAddTo("Velocity", id);
	//world->CreateComponentAndAddTo("Position", id);

	//world->KillEntity(id);

	//delete world;
	//delete(&ECSL::ComponentTypeManager::GetInstance());
	//delete(&ECSL::BitSet::BitSetConverter::GetInstance());
}


int main(int argc, char** argv)
{
#ifdef WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	Timer timer;

	Renderer::GraphicDevice RENDERER = Renderer::GraphicDevice();
	Input::InputWrapper* INPUT = &Input::InputWrapper::GetInstance();
	RENDERER.Init();

	std::string input;
	printf("Starting network:\n\n");
	printf("Press 's' to start a new server, 'c' to start a new client,\nor anything else to skip network: ");
	std::getline(std::cin, input);
	ClearConsole();
	ServerNetwork server;
	ClientNetwork client;
	bool isServer = false;


	if (input.compare("s") == 0)
	{
		isServer = true;
		server.Start(6112, "localhest", 8);
	}
	else if (input.compare("c") == 0)
	{
		client.Connect("127.0.0.1", "loca453lhest", 6112, 0);
	}

	TestECSL();
	bool lol = true;
	float cd = 1.0f;
	while (lol)
	{
		// DT COUNTER
		float dt = timer.ElapsedTimeInSeconds();
		timer.Reset();

		if (isServer)
		{
			do
			{
			} while (server.TriggerPacket() > 0);
		}
		else
		{
			do
			{
			} while (server.TriggerPacket() > 0);
		}

		INPUT->Update();
		RENDERER.Update(dt);
		RENDERER.RenderSimpleText("This text render from GAME! \nThe x and y values in the function isn't pixel \ncoordinates, it's char position. Every char is \n8x16 pixels in size. Use \\n to change line.\n\n  !Not all chars is supported!\n\nRight now it clear the whole output image as well (Tell me when to remove this).", 10, 2);

		RENDERER.Render();

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_WINDOWEVENT:
				RENDERER.PollEvent(e);
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			case SDL_FINGERMOTION:
			case SDL_FINGERDOWN:
			case SDL_FINGERUP:
			case SDL_JOYAXISMOTION:
			case SDL_JOYBALLMOTION:
			case SDL_JOYHATMOTION:
			case SDL_JOYBUTTONDOWN:
			case SDL_JOYBUTTONUP:
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEWHEEL:
			case SDL_MULTIGESTURE:
				INPUT->PollEvent(e);
				break;
			}


		}

		if (INPUT->GetKeyboard()->GetKeyState(SDL_SCANCODE_ESCAPE) == Input::InputState::PRESSED)
		{
			lol = false;
		}
	}
	delete(INPUT);

#ifdef WIN32
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}