#include "GameServer.h"
#include <iostream>

GameServer::GameServer()
{
	m_packetHandler.AddNetMessageHook("printstring", &PrintString);

	m_packetHandler.AddNetMessageHook("selectedcards", std::bind(&GameServer::SelectedCards, this, std::placeholders::_1, std::placeholders::_2));
	m_packetHandler.AddNetMessageHook("setusername", std::bind(&GameServer::SetUsername, this, std::placeholders::_1, std::placeholders::_2));


	m_server = new Server();
	m_server->SetOnPlayerConnected(std::bind(&GameServer::OnPlayerConnected, this, std::placeholders::_1));
	m_server->SetOnPlayerDisconnected(std::bind(&GameServer::OnPlayerDisonnected, this, std::placeholders::_1));
	m_server->SetOnPlayerTimedOut(std::bind(&GameServer::OnPlayerDisonnected, this, std::placeholders::_1));

	m_server->Start();

	m_currentState = 0;
}

GameServer::~GameServer()
{
	SAFE_DELETE(m_currentState);
}

void GameServer::Update()
{
	//lyssna på paket
	PacketHandler::Packet* packetIn = 0;
	do
	{
		packetIn = m_server->GetPacket();

		if (packetIn)
			m_packetHandler.ConsumePacket(packetIn);
		
	} while (packetIn);
	
	//uppdatera state
	if (m_currentState)
		m_currentState->Update();
}

void GameServer::ResetGame()
{
	int NrOfPlayers = m_players.size();
	int NrOfCards = 0;


	// Setup items
	std::vector<int> prioList;
	std::vector<CardAddInstruction> cardSetup;
	int ThisManyCards;



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
	if (NrOfCards < m_cardsInHand * NrOfPlayers)
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
			m_mCardDeck.push_back(MovementCard(cardSetup[i].name, prioList[prioCard]));
			prioList.erase(prioList.begin() + prioCard);
		}
	}

	
	//skicka spelinställningar
	m_packetHandler.StartPack("setgamesettings");
	m_packetHandler.WriteInt(m_cardsInHand);
	m_packetHandler.WriteInt(m_cardsToPlay);
	m_server->Broadcast(m_packetHandler.EndPack());

	// Welcome msg
	std::string input;
	ClearConsole();
	std::cout << "New round started!\n";
	std::cout << "-------------------------\n";
	std::cout << "Players:   \t" << NrOfPlayers << "\n";
	std::cout << "Cards in Hand: \t" << m_cardsInHand << "\n";
	std::cout << "Cards To Play: \t" << m_cardsToPlay << "\n";
	std::cout << "-------------------------\n";
	std::cout << "PRESS ENTER KEY TO START\n";
	std::getline(std::cin, input);
	ClearConsole();

	m_playedCards = new MovementCard[m_cardsToPlay*NrOfPlayers];
}

ServerState::ServerState(GameServer* _gameServer)
{
	m_gameServer = _gameServer;
}

ServerState::~ServerState(void)
{
}




LobbyState::LobbyState(GameServer* _gameServer)
	: ServerState(_gameServer)
{
	PrintState();
}

void LobbyState::PrintState()
{
	ClearConsole();
	auto players = m_gameServer->GetPlayers();
	printf("Players connected: %i\n", players->size());

	for (int i = 0; i < players->size(); ++i)
	{
		std::cout << i << ": " << players->at(i).name << "\n";
	}

	printf("\nWait for all clients to connect before you start!\n");
	printf("WRITE start TO START!\n");
}

void LobbyState::Update()
{
	std::string input;
	std::getline(std::cin, input);

	if (input == "start") // && m_gameServer->GetPlayers()->size() >= 2)
	{
		m_gameServer->ResetGame();

		m_gameServer->SetState(new DealState(m_gameServer));
	}
	//else
	{
		//för få spelare
	}
	
}


void LobbyState::SetUsername(PacketHandler* _ph, NetConnection* _connection)
{
	//ändra namn
	auto players = m_gameServer->GetPlayers();
	for (int i = 0; i < players->size(); ++i)
	{
		if (players->at(i).connection == _connection)
		{
			players->at(i).name = _ph->ReadString();


			_ph->StartPack("setusername_client");
			_ph->WriteString(players->at(i).name.c_str());
			m_gameServer->GetServer()->Send(_ph->EndPack(), *_connection);

			PrintState();
			break;
		}
	}

	//skicka ok
}

void LobbyState::OnPlayerConnected(NetConnection* _connection)
{
	//adda honom
	auto players = m_gameServer->GetPlayers();

	Player p;
	p.connection = _connection;
	p.name = "Player ";
	p.name.append(std::to_string(players->size()).c_str());
	players->push_back(p);


	auto ph = m_gameServer->GetPacketHandler();

	ph->StartPack("setusername_client");
	ph->WriteString(p.name.c_str());
	m_gameServer->GetServer()->Send(ph->EndPack(), *_connection);

	ph->StartPack("sendusername");
	
	m_gameServer->GetServer()->Send(ph->EndPack(), *_connection);

	PrintState();
}

void LobbyState::OnPlayerDisonnected(NetConnection* _connection)
{
	//ta bort honom

	auto players = m_gameServer->GetPlayers();

	for (int i = players->size() - 1; i >= 0; --i)
	{
		if (players->at(i).connection == _connection)
		{
			players->erase(players->begin() + i);
			break;
		}
	}

	ClearConsole();
	printf("Players connected: %i\n", players->size());
	printf("Wait for all clients to connect before starting!\n");
	printf("PRESS ENTER TO START!\n");
}


DealState::DealState(GameServer* _gameServer)
	: ServerState(_gameServer)
{
	ClearConsole();
	printf("Waiting for players to select cards.\n");


	auto players = _gameServer->GetPlayers();

	auto mCardDeck = m_gameServer->GetMovementCardDeck();

	PacketHandler::Packet packet;
	for (int i = 0; i < players->size(); ++i)
	{
		m_playersWaitingFor.push_back(players->at(i).connection);

		while (players->at(i).movementCards.size() < m_gameServer->GetCardsInHand())
		{
			int cardId = rand() % mCardDeck->size();
			players->at(i).movementCards.push_back(mCardDeck->at(cardId));
			mCardDeck->erase(mCardDeck->begin() + cardId);
		}

		// Send Cards to player

		PacketHandler* ph = m_gameServer->GetPacketHandler();

		ph->StartPack("selectcards");
		for (MovementCard card : players->at(i).movementCards)
		{
			ph->WriteString(card.name.c_str());
			ph->WriteFloat(card.prio);
		}

		packet = ph->EndPack();

		m_gameServer->GetServer()->Send(packet, *players->at(i).connection);

	}
}

DealState::~DealState(void)
{

}

void DealState::Update()
{
	/*
	
	if (tiden är slut)
	{
		välja random kort åt spelare i m_playersWaitingFor
		sluta vänta på resterande spelare ( töm m_playersWaitingFor )
	}
	*/

	if (m_playersWaitingFor.empty())
	{
		m_gameServer->SetState(new StepState(m_gameServer));
	}
}

void DealState::SelectedCards(PacketHandler* _ph, NetConnection* _connection)
{
	//välj kort
	int *selectedcards = new int[m_gameServer->GetCardsToPlay()];

	for (int i = 0; i < m_gameServer->GetCardsToPlay(); ++i)
		selectedcards[i] = _ph->ReadInt();


	auto players = m_gameServer->GetPlayers();

	for (int i = 0; i < players->size(); ++i)
	{
		if (players->at(i).connection == _connection)
		{
			// add cards to playedCards
			for (int j = 0; j < m_gameServer->GetCardsToPlay(); j++)
			{
				Player p = players->at(i);

				m_gameServer->GetPlayedCards()[m_gameServer->GetCardsToPlay()*i + j] = players->at(i).movementCards[selectedcards[j]];
				//playedCards.push_back(players[i].movementCards[selectedcards[j]]);
			}
			// remove cards
			for (int j = players->at(i).movementCards.size() - 1; j >= 0; j--)
			{
				for (int k = 0; k < m_gameServer->GetCardsToPlay(); k++)
				{
					if (selectedcards[k] == j)
					{
						players->at(i).movementCards.erase(players->at(i).movementCards.begin() + selectedcards[k]);
						break;
					}
				}
			}
			printf("%s is ready!\n", players->at(i).name.c_str());
			break;
		}
	}

	m_playersWaitingFor.erase(std::remove<std::vector<NetConnection*>::iterator, NetConnection*>(m_playersWaitingFor.begin(), m_playersWaitingFor.end(), _connection), m_playersWaitingFor.end());
}

void DealState::OnPlayerConnected(NetConnection* _connection)
{
	//kicka honom
	m_gameServer->GetServer()->KickClient(_connection, "Game in progress");
}

void DealState::OnPlayerDisonnected(NetConnection* _connection)
{
	//ta bort honom
	auto players = m_gameServer->GetPlayers();

	for (int i = players->size() - 1; i >= 0; --i)
	{
		if (players->at(i).connection == _connection)
		{
			players->erase(players->begin() + i);
			break;
		}
	}

	//sluta vänta på den spelaren
	m_playersWaitingFor.erase(std::remove<std::vector<NetConnection*>::iterator, NetConnection*>(m_playersWaitingFor.begin(), m_playersWaitingFor.end(), _connection), m_playersWaitingFor.end());
}




StepState::StepState(GameServer* _gameServer)
	: ServerState(_gameServer)
{
	m_currentStep = 0;
	std::string input;
	std::getline(std::cin, input);
}

StepState::~StepState(void)
{

}

void StepState::Update()
{
	auto players = m_gameServer->GetPlayers();
	int CardsToPlay = m_gameServer->GetCardsToPlay();
	int NrOfPlayers = players->size();
	auto playedCards = m_gameServer->GetPlayedCards();

	if (m_currentStep < m_gameServer->GetCardsToPlay())
	{

		std::vector<MovementCard> prioList;
		std::vector<std::string> nameList;

		bool hest = false;

		prioList.push_back(playedCards[0 + m_currentStep]);
		nameList.push_back(players->at(0).name);
		for (int j = 0; j < NrOfPlayers; ++j)
		{
			hest = false;
			for (int q = 0; q < prioList.size(); ++q)
			{
				if (prioList[q].prio > playedCards[CardsToPlay * j + m_currentStep].prio)
				{
					auto pos = prioList.begin() + q;
					auto namePos = nameList.begin() + q;

					prioList.insert(pos, playedCards[CardsToPlay * j + m_currentStep]);
					nameList.insert(namePos, players->at(j).name);
					hest = true;
					break;
				}

			}
			if (!hest)
			{
				if (j == 0)
					continue;

				prioList.push_back(playedCards[CardsToPlay * j + m_currentStep]);
				nameList.push_back(players->at(j).name);
			}
		}

		ClearConsole();
		std::cout << "Turn " << (m_currentStep + 1) << "\n";
		std::cout << "-------------------------\n";
		// Print moves
		for (int j = 0; j < NrOfPlayers; j++)
		{
			std::cout << nameList[j] << ": " << prioList[j].name << "\t" << prioList[j].prio << "\n";
			//std::cout << players[j].name << ": " << playedCards[CardsToPlay*j + i].name << "\t" << playedCards[CardsToPlay*j + i].prio << "\n";
		}
		std::cout << "-------------------------\n";
		std::cout << "Move players and ENTER.\n";
		std::string input;
		std::getline(std::cin, input);
		ClearConsole();
		++m_currentStep;
	}
	else
	{

		// Shuffle back the cards
		for (int i = 0; i < CardsToPlay * NrOfPlayers; i++)
		{
			m_gameServer->GetMovementCardDeck()->push_back(playedCards[i]);
		}
		//playedCards.clear();


		std::cout << "PRESS ENTER KEY TO START NEW ROUND\n";
		std::string input;
		std::getline(std::cin, input);
		ClearConsole();

		m_gameServer->SetState(new DealState(m_gameServer));
	}
	

	/*

	if (enter är tryckt)
	{

		if (current_step < 5)
			Visa nästa steg
			current_step++

		else
			m_gameServer->SetState(new DealState());
			

	}

	*/
}


void StepState::OnPlayerConnected(NetConnection* _connection)
{
	//Kicka honom
	m_gameServer->GetServer()->KickClient(_connection, "Game in progress");
}

void StepState::OnPlayerDisonnected(NetConnection* _connection)
{
	//ta bort honom
	auto players = m_gameServer->GetPlayers();

	for (int i = players->size() - 1; i >= 0; --i)
	{
		if (players->at(i).connection == _connection)
		{
			players->erase(players->begin() + i);
			break;
		}
	}
}