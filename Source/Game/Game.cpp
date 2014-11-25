#include <SDL/SDL.h>
#include <iostream>
#include <string>
#include <algorithm>

#include "ECSL/ECSL.h"
#include "Game/GameServer.h"
#include "Game/GameClient.h"

#ifdef _WIN32
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


bool connected = false;

void RunServer()
{
	GameServer gameServer = GameServer();
	gameServer.SetState(new LobbyState(&gameServer));


	while (true)
	{
		gameServer.Update();
		NetSleep(30);
	}

}


void RunClient()
{
	GameClient gameClient = GameClient();


	while (true)
	{
		gameClient.Update();
		NetSleep(30);
	}
}


int main(int argc, char** argv)
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	SDL_Init(SDL_INIT_EVERYTHING);

	std::string input;

	printf("s for server, c for client\n");
	std::getline(std::cin, input);
	ClearConsole();

	if (input == "s")
	{
		RunServer();
	}

	else if (input == "c")
	{
		RunClient();
	}

	else if (input == "q")
	{

	}

	SDL_Quit();
	return 0;
}