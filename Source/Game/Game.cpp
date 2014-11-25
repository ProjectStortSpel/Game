#include <SDL/SDL.h>
#include <iostream>
#include <string>
#include <algorithm>
#include "Network/Server.h"
#include "Network/Client.h"
#include "ECSL/ECSL.h"

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

int CardsInHand = 8;
int CardsToPlay = 5;
int waitforplayers;

std::string myName = "";

void ClearConsole()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

bool CheckValidMove(std::string moves)
{
	if (moves.size() != CardsToPlay)
	{
		printf("WRONG SIZE\n");
		return false;
	}
	for (int i = 0; i < moves.size(); i++)
	{
		int nr = (int)moves[i] - 48;
		if (nr < 0 || nr >= CardsInHand)
		{
			printf("WRONG NUMBER\n");
			return false;
		}
		int count = (int)std::count(moves.begin(), moves.end(), moves[i]);
		if (count > 1)
		{
			printf("Does not exist\n");
			return false;
		}
	}
	return true;
}

struct CardAddInstruction
{
	std::string name;
	int nrOfCards;
	CardAddInstruction(std::string n, int nr)
	{
		name = n;
		nrOfCards = nr;
	}
};

struct MovementCard
{
	std::string name;
	float prio;
	MovementCard()
	{

	}
	MovementCard(std::string n, float p)
	{
		name = n;
		prio = p;
	}
};

struct AbilityCard
{
	std::string name;
	float prio;
	AbilityCard(std::string n, float p)
	{
		name = n;
		prio = p;
	}
};

struct Player
{
	std::string name;
	std::vector<MovementCard> movementCards;
	std::vector<AbilityCard> abilityCards;
	NetConnection* connection;
};


PacketHandler packetHandler;

Server *s = NULL;
Client *c = NULL;

bool connected = false;

// Game items
std::vector<Player> players;
std::vector<MovementCard> mcardDeck;
std::vector<AbilityCard> acardDeck;
MovementCard *playedCards;

void OnConnect(unsigned char _token, NetConnection* _connection)
{
	//connections.push_back(_connection);
	connected = true;
}

void OnDisconnect(unsigned char _token, NetConnection* _connection)
{
	//connections.erase(std::remove(connections.begin(), connections.end(), _connection), connections.end());
	connected = false;
}

void OnPlayerConnect(unsigned char _token, NetConnection* _connection)
{
	Player p;
	p.connection = _connection;
	players.push_back(p);

	ClearConsole();
	printf("Players connected: %i\n", players.size());
	printf("Wait for all clients to connect before starting!\n");
	printf("PRESS ENTER TO START!\n");

}

void OnPlayerDisconnect(unsigned char _token, NetConnection* _connection)
{
	for (int i = players.size() - 1; i >= 0; --i)
	{
		if (players[i].connection == _connection)
			players.erase(players.begin() + i);
	}
}

void printstring(PacketHandler* _ph, NetConnection* _connection)
{
	printf("%s\n", _ph->ReadString());
}


void setusername(PacketHandler* _ph, NetConnection* _connection)
{
	for (int i = 0; i < players.size(); ++i)
	{
		if (players[i].connection == _connection)
		{
			players[i].name = _ph->ReadString();
			printf("%s is ready!\n", players[i].name.c_str());
			break;
		}
	}
	--waitforplayers;
}


void sendusername(PacketHandler* _ph, NetConnection* _connection)
{
	std::string input;
	ClearConsole();
	std::cout << "Enter your name\n";
	std::getline(std::cin, input);


	packetHandler.StartPack("setusername");
	packetHandler.WriteString(input.c_str());

	PacketHandler::Packet p = packetHandler.EndPack();

	c->SendToServer(p);

	myName = input;
	ClearConsole();
	std::cout << "Name: " << myName << "\n";
	std::cout << "Wait for your next turn\n";
}

void selectcards(PacketHandler* _ph, NetConnection* _connection)
{
	MovementCard* cards = new MovementCard[CardsInHand];

	for (int i = 0; i < CardsInHand; ++i)
	{
		cards[i].name = _ph->ReadString();
		cards[i].prio = _ph->ReadFloat();
	}

	// Welcome msg
	std::string input;
	ClearConsole();
	std::cout << "Name: " << myName << "\n";
	std::cout << "Your turn\n";
	std::cout << "-------------------------\n";
	std::cout << "Press ENTER to begin.\n";
	std::getline(std::cin, input);

	// Print cards
	for (int j = 0; j < CardsInHand; j++)
	{
		std::cout << (char)(j + 48) << ": " << cards[j].name << "\t" << cards[j].prio << "\n";
	}
	std::cout << "-------------------------\n";
	std::cout << "Type cards to play and press ENTER. (like this '01234' + ENTER)\n";
	std::cout << "CARDS: ";
	std::getline(std::cin, input);

	while (!CheckValidMove(input))
	{
		std::cout << "CARDS (redo): ";
		std::getline(std::cin, input);
	}

	int *selectedcards = new int[CardsToPlay];

	// send cards to server
	packetHandler.StartPack("selectedcards");
	for (int j = 0; j < input.size(); j++)
	{
		packetHandler.WriteInt((int)input[j] - 48);
	}

	PacketHandler::Packet p = packetHandler.EndPack();

	delete[] selectedcards;
	delete[] cards;
	c->SendToServer(p);

	ClearConsole();
	std::cout << "Name: " << myName << "\n";
	std::cout << "Wait for your next turn\n";

}

void startwaitforallplayers(int num)
{
	waitforplayers = num;
}

void selectedcards(PacketHandler* _ph, NetConnection* _connection)
{
	int *selectedcards = new int[CardsToPlay];

	for (int i = 0; i < CardsToPlay; ++i)
		selectedcards[i] = packetHandler.ReadInt();


	for (int i = 0; i < players.size(); ++i)
	{
		if (players[i].connection == _connection)
		{
			// add cards to playedCards
			for (int j = 0; j < CardsToPlay; j++)
			{
				playedCards[CardsToPlay*i + j] = players[i].movementCards[selectedcards[j]];
				//playedCards.push_back(players[i].movementCards[selectedcards[j]]);
			}			
			// remove cards
			for (int j = players[i].movementCards.size() - 1; j >= 0; j--)
			{
				for (int k = 0; k < CardsToPlay; k++)
				{
					if (selectedcards[k] == j)
					{
						players[i].movementCards.erase(players[i].movementCards.begin() + selectedcards[k]);
						break;
					}
				}
			}
			printf("%s is ready!\n", players[i].name.c_str());
			break;
		}
	}

	waitforplayers--;
}


void RunServer()
{
	packetHandler.AddNetMessageHook("selectedcards", &selectedcards);
	packetHandler.AddNetMessageHook("setusername", &setusername);

	PacketHandler::Packet p;
	std::string input;

	s = new Server();
	s->SetOnPlayerConnected(&OnPlayerConnect);
	s->SetOnPlayerDisconnected(&OnPlayerDisconnect);
	s->SetOnPlayerTimedOut(&OnPlayerDisconnect);
	s->Start();

	delete s;
	s = 0;

	return;
	printf("Players connected: 0\n");
	printf("Wait for all clients to connect before starting!\n");
	printf("PRESS ENTER TO START!\n");
	std::getline(std::cin, input);
	ClearConsole();

	
	

	int NrOfPlayers = players.size();
	int NrOfCards = 0;

	

	// Setup items
	std::vector<int> prioList;
	std::vector<CardAddInstruction> cardSetup;
	int ThisManyCards;


	//Get user names from players
	printf("Waiting for usernames\n");
	packetHandler.StartPack("sendusername");
	p = packetHandler.EndPack();

	s->Broadcast(p);

	startwaitforallplayers(NrOfPlayers);
	while (waitforplayers > 0)
	{
		PacketHandler::Packet* packetIn = 0;
		while (!packetIn)
			packetIn = s->GetPacket();
		packetHandler.StartUnPack(packetIn);
	}


	// Movement Cards Settings
	ThisManyCards = NrOfPlayers * 2;
	cardSetup.push_back(CardAddInstruction("^ Forward    ", ThisManyCards)); NrOfCards += ThisManyCards;

	ThisManyCards = NrOfPlayers * 2;
	cardSetup.push_back(CardAddInstruction("v Backward   ", ThisManyCards)); NrOfCards += ThisManyCards;

	ThisManyCards = NrOfPlayers * 1;
	cardSetup.push_back(CardAddInstruction("> Right      ", ThisManyCards)); NrOfCards += ThisManyCards;

	ThisManyCards = NrOfPlayers * 1;
	cardSetup.push_back(CardAddInstruction("< Left       ", ThisManyCards)); NrOfCards += ThisManyCards;

	ThisManyCards = NrOfPlayers * 1;
	cardSetup.push_back(CardAddInstruction("@ Turn around", ThisManyCards)); NrOfCards += ThisManyCards;

	ThisManyCards = NrOfPlayers * 1;
	cardSetup.push_back(CardAddInstruction("- Jump       ", ThisManyCards)); NrOfCards += ThisManyCards;


	// Check if there is too few cards
	if (NrOfCards < CardsInHand * NrOfPlayers)
	{
		std::cout << "Too few cards in deck. (PROGRAM MIGHT CRASH)\n";
		NetSleep(1000);
	}

	// Generation priolist
	for (int i = 0; i < NrOfCards; i++)
		prioList.push_back(i);



	// Add Movement cards
	for (int i = 0; i < cardSetup.size(); i++)
	{
		// Get prio
		for (int j = 0; j < cardSetup[i].nrOfCards; j++)
		{
			int prioCard = rand() % prioList.size();
			mcardDeck.push_back(MovementCard(cardSetup[i].name, prioList[prioCard]));
			prioList.erase(prioList.begin() + prioCard);
		}
	}

	// Welcome msg
	ClearConsole();
	std::cout << "New round started!\n";
	std::cout << "-------------------------\n";
	std::cout << "Players:   \t" << NrOfPlayers << "\n";
	std::cout << "Cards in Hand: \t" << CardsInHand << "\n";
	std::cout << "Cards To Play: \t" << CardsToPlay << "\n";
	std::cout << "-------------------------\n";
	std::cout << "PRESS ENTER KEY TO START\n";
	std::getline(std::cin, input);
	ClearConsole();

	playedCards = new MovementCard[CardsToPlay*NrOfPlayers];
	while (true)
	{
		std::cout << "Waiting for all players to select cards.\n";
		// Deal cards
		for (int i = 0; i < players.size(); i++)
		{
			while (players[i].movementCards.size() < CardsInHand)
			{
				int cardId = rand() % mcardDeck.size();
				players[i].movementCards.push_back(mcardDeck[cardId]);
				mcardDeck.erase(mcardDeck.begin() + cardId);
			}
			
			// Send Cards to player
			
			packetHandler.StartPack("selectcards");
			for (MovementCard card : players[i].movementCards)
			{
				packetHandler.WriteString(card.name.c_str());
				packetHandler.WriteFloat(card.prio);
			}

			p = packetHandler.EndPack();

			s->Send(p, *players[i].connection);
		}

		startwaitforallplayers(NrOfPlayers);
		while (waitforplayers > 0)
		{
			PacketHandler::Packet* packetIn = 0;
			while (!packetIn)
				packetIn = s->GetPacket();
			packetHandler.StartUnPack(packetIn);
		}
		ClearConsole();
		
		std::vector<MovementCard> prioList;
		std::vector<std::string> nameList;
		bool hest = false;
		// Move turns
		for (int i = 0; i < CardsToPlay; i++)
		{
			
			prioList.clear();
			nameList.clear();
			prioList.push_back(playedCards[0 + i]);
			nameList.push_back(players[0].name);
			for (int j = 0; j < NrOfPlayers; ++j)
			{
				hest = false;
				for (int q = 0; q < prioList.size(); ++q)
				{
					if (prioList[q].prio > playedCards[CardsToPlay * j + i].prio)
					{
						auto pos = prioList.begin() + q;
						auto namePos = nameList.begin() + q;

						prioList.insert(pos, playedCards[CardsToPlay * j + i]);
						nameList.insert(namePos, players[j].name);
						hest = true;
						break;
					}

				}
				if (!hest)
				{
					if (j == 0)
						continue;

					prioList.push_back(playedCards[CardsToPlay * j + i]);
					nameList.push_back(players[j].name);
				}
			}

			std::cout << "Turn " << (i + 1) << "\n";
			std::cout << "-------------------------\n";
			// Print moves
			for (int j = 0; j < NrOfPlayers; j++)
			{
				std::cout << nameList[j] << ": " << prioList[j].name << "\t" << prioList[j].prio << "\n";
				//std::cout << players[j].name << ": " << playedCards[CardsToPlay*j + i].name << "\t" << playedCards[CardsToPlay*j + i].prio << "\n";
			}
			std::cout << "-------------------------\n";
			std::cout << "Move players and ENTER.\n";
			std::getline(std::cin, input);
			ClearConsole();
		}

		// Shuffle back the cards
		for (int i = 0; i < CardsToPlay * NrOfPlayers; i++)
		{
			mcardDeck.push_back(playedCards[i]);
		}
		//playedCards.clear();


		std::cout << "PRESS ENTER KEY TO START NEW ROUND\n";
		std::getline(std::cin, input);
		ClearConsole();
	}

	delete playedCards;

	s->Stop();
}

void RunClient()
{
	packetHandler.AddNetMessageHook("selectcards", &selectcards);
	packetHandler.AddNetMessageHook("sendusername", &sendusername);
	
	std::string input;

	c = new Client();

	printf("Enter ip\n");
	std::getline(std::cin, input);

	if (input == "")
		input = "localhost";

	c->SetRemoteAddress(input.c_str());
	ClearConsole();

	printf("Enter incoming port\n");
	std::getline(std::cin, input);

	if (input == "")
		input = "5358";

	c->SetIncomingPort(atoi(input.c_str()));
	ClearConsole();
	printf("Connecting...\n");

	c->SetOnConnectedToServer(&OnConnect);
	c->SetOnDisconnectedFromServer(&OnDisconnect);
	c->SetOnTimedOutFromServer(&OnDisconnect);
	c->Connect();
	//c->StartListen();

	while (!connected)
		NetSleep(30);

	ClearConsole();
	printf("Connected!\n");
	printf("Waiting for the server to start the game.\n");

	while (connected)
	{
		PacketHandler::Packet* packetIn = 0;
		while (!packetIn)
			packetIn = c->GetPacket();

		packetHandler.StartUnPack(packetIn);
	}


	c->Disconect();
}

int main(int argc, char** argv)
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	SDL_Init(SDL_INIT_EVERYTHING);

	packetHandler.AddNetMessageHook("printstring", &printstring);

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