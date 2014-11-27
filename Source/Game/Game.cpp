#include <SDL/SDL.h>
#include <iostream>
#include <string>
#include <algorithm>
#include "Network/ServerNetwork.h"
#include "Network/ClientNetwork.h"
#include "Network/PacketHandler.h"
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

	std::string input;
	std::getline(std::cin, input);
	
	ClientNetwork c;
	ServerNetwork s;
	
	if (input == "c")
	{
		c.Connect("127.0.0.1", "localhest", 6112, 0);

		std::getline(std::cin, input);
	}
	else if (input == "s")
	{
		s.Start(6112, "localhest", 8);
		std::getline(std::cin, input);
	}	
	SDL_Quit();
	return 0;
}