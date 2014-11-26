#include <SDL/SDL.h>
#include <iostream>
#include <string>
#include <algorithm>
#include "Network/Server.h"
#include "Network/Client.h"
#include "ECSL/ECSL.h"

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


int main(int argc, char** argv)
{
#ifdef WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	SDL_Init(SDL_INIT_EVERYTHING);

	Client c;
	c.Connect("194.47.150.5", "localhest", 5358, 0);

	ISocket::Shutdown();

	std::string input;
	std::getline(std::cin, input);

	SDL_Quit();
	return 0;
}