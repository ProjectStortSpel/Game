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

	std::string ip = "127.0.0.1";
	std::string pw = "localhest";
	int port = 5357;

	
	ClientNetwork c;
	ServerNetwork s;
	
	printf("Network console test!\n");

	printf("Press \"c\" to start a client, or \"s\" to start a server.\n");
	std::getline(std::cin, input);
	ClearConsole();

	if (input == "c")
	{
		printf("Starting new client.\n");
		printf("Enter Ip Address: ");
		std::getline(std::cin, input);

		if (input != "")
			ip = input;


		printf("Enter password: ");
		std::getline(std::cin, input);

		if (input != "")
			pw = input;

		printf("Enter port number: ");
		std::getline(std::cin, input);

		if (input != "")
			port = atoi(input.c_str());

		ClearConsole();

		c.Connect(ip.c_str(), pw.c_str(), port, 0);

		std::getline(std::cin, input);
	}
	else if (input == "s")
	{
		printf("Starting new server.\n");

		printf("Enter password: ");
		std::getline(std::cin, input);

		if (input != "")
			pw = input;

		printf("Enter port number: ");
		std::getline(std::cin, input);

		if (input != "")
			port = atoi(input.c_str());

		ClearConsole();

		s.Start(port, pw.c_str(), 8);
		std::getline(std::cin, input);

		PacketHandler ph;
		ph.StartPack(NetTypeMessageId::ID_CONNECTION_ACCEPTED);
		ph.WriteInt(1337);

		Packet* p = ph.EndPack();

		s.Broadcast(p);

		std::getline(std::cin, input);


	}	
	SDL_Quit();
	return 0;
}