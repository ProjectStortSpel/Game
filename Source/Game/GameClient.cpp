#include <iostream>

#include "Game/GameClient.h"
#include "Game/GameShared.h"

GameClient::GameClient()
{
	m_packetHandler.AddNetMessageHook("printstring", &PrintString);

	m_packetHandler.AddNetMessageHook("selectcards", std::bind(&GameClient::SelectCards, this, std::placeholders::_1, std::placeholders::_2));
	m_packetHandler.AddNetMessageHook("sendusername", std::bind(&GameClient::SendUsername, this, std::placeholders::_1, std::placeholders::_2));
	m_packetHandler.AddNetMessageHook("setusername_client", std::bind(&GameClient::SetUsername, this, std::placeholders::_1, std::placeholders::_2));
	m_packetHandler.AddNetMessageHook("setgamesettings", std::bind(&GameClient::SetGameSettings, this, std::placeholders::_1, std::placeholders::_2));

	m_client = new Client();

	m_client->SetOnConnectedToServer(std::bind(&GameClient::OnConnect, this, std::placeholders::_1));
	m_client->SetOnDisconnectedFromServer(std::bind(&GameClient::OnDisconnect, this, std::placeholders::_1));
	m_client->SetOnTimedOutFromServer(std::bind(&GameClient::OnDisconnect, this, std::placeholders::_1));


	std::string input;

	printf("Enter ip\n");
	std::getline(std::cin, input);

	if (input == "")
		input = "localhost";

	m_client->SetRemoteAddress(input.c_str());
	ClearConsole();

	printf("Enter incoming port\n");
	std::getline(std::cin, input);

	if (input == "")
		input = "5358";

	m_client->SetIncomingPort(atoi(input.c_str()));
	ClearConsole();
	printf("Connecting...\n");

	m_client->Connect();
}

GameClient::~GameClient()
{
	SAFE_DELETE(m_currentState);
}

void GameClient::Update()
{
	if (m_connected)
	{

		PacketHandler::Packet* packetIn = 0;
		
		do
		{
			packetIn = m_client->GetPacket();

			if (packetIn)
				m_packetHandler.ConsumePacket(packetIn);

		} while (packetIn);

	}
	else
	{
		
	}
	if (m_currentState)
		m_currentState->Update();
}

void GameClient::SelectCards(PacketHandler* _ph, NetConnection* _connection)
{
	SetState(NULL);

	MovementCard* cards = new MovementCard[m_cardsInHand];

	for (int i = 0; i < m_cardsInHand; ++i)
	{
		cards[i].name = _ph->ReadString();
		cards[i].prio = _ph->ReadFloat();
	}

	// Welcome msg
	std::string input;
	ClearConsole();
	std::cout << "Name: " << m_name << "\n";
	std::cout << "Your turn\n";
	std::cout << "-------------------------\n";
	std::cout << "Press ENTER to begin.\n";
	std::getline(std::cin, input);

	// Print cards
	for (int j = 0; j < m_cardsInHand; j++)
	{
		std::cout << (char)(j + 48) << ": " << cards[j].name << "\t" << cards[j].prio << "\n";
	}
	std::cout << "-------------------------\n";
	std::cout << "Type cards to play and press ENTER. (like this '01234' + ENTER)\n";
	std::cout << "CARDS: ";
	std::getline(std::cin, input);

	while (!CheckValidMove(input, m_cardsToPlay, m_cardsInHand))
	{
		std::cout << "CARDS (redo): ";
		std::getline(std::cin, input);
	}

	int *selectedcards = new int[m_cardsToPlay];

	// send cards to server
	m_packetHandler.StartPack("selectedcards");
	for (int j = 0; j < input.size(); j++)
	{
		m_packetHandler.WriteInt((int)input[j] - 48);
	}

	PacketHandler::Packet p = m_packetHandler.EndPack();

	delete[] selectedcards;
	delete[] cards;
	m_client->SendToServer(p);

	ClearConsole();
	std::cout << "Name: " << m_name << "\n";
	std::cout << "Wait for your next turn\n";

}

void GameClient::SetGameSettings(PacketHandler* _ph, NetConnection* _connection)
{
	m_cardsInHand = _ph->ReadInt();
	m_cardsToPlay = _ph->ReadInt();

	if (m_currentState)
		m_currentState->SetGameSettings(_ph, _connection);
}

void GameClient::OnConnect(NetConnection* _connection)
{
	printf("Connected\n");
	m_connected = true;

	if (m_currentState)
		m_currentState->OnConnect(_connection);
}

void GameClient::OnDisconnect(NetConnection* _connection)
{
	printf("Disonnecte\n");
	m_connected = false;

	if (m_currentState)
		m_currentState->OnDisconnect(_connection);
}

void GameClient::SendUsername(PacketHandler* _ph, NetConnection* _connection)
{
	SetState(new UsernameState(this));
}

void GameClient::SetUsername(PacketHandler* _ph, NetConnection* _connection)
{
	m_name = _ph->ReadString();

	if (m_currentState)
		m_currentState->SetUsername(_ph, _connection);
}



ClientState::ClientState(GameClient* _gameClient)
{
	m_gameClient = _gameClient;
}

ClientState::~ClientState(void)
{
}







UsernameState::UsernameState(GameClient* _gameClient)
	: ClientState(_gameClient)
{

}

UsernameState::~UsernameState()
{

}

void UsernameState::Update()
{
	PrintState();

	std::string input;
	std::getline(std::cin, input);

	if (input != "")
	{
		PacketHandler* ph = m_gameClient->GetPacketHandler();

		ph->StartPack("setusername");
		ph->WriteString(input.c_str());

		PacketHandler::Packet p = ph->EndPack();

		m_gameClient->GetClient()->SendToServer(p);
	}
}

void UsernameState::SetUsername(PacketHandler* _ph, NetConnection* _connection)
{
	PrintState();
}

void UsernameState::PrintState()
{
	ClearConsole();
	std::cout << "Name: " << m_gameClient->GetName() << "\n";
	std::cout << "Enter your name\n";
}


//
//
//
//
//UsernameState::UsernameState(GameClient* _gameClient)
//	: ClientState(_gameClient)
//{
//
//}
//
//UsernameState::~UsernameState()
//{
//
//}
//
//void UsernameState::Update()
//{
//	PrintState();
//
//	std::string input;
//	ClearConsole();
//	std::getline(std::cin, input);
//
//	PacketHandler* ph = m_gameClient->GetPacketHandler();
//
//	ph->StartPack("setusername");
//	ph->WriteString(input.c_str());
//
//	PacketHandler::Packet p = ph->EndPack();
//
//	m_gameClient->GetClient()->SendToServer(p);
//}
//
//void UsernameState::SetUsername(PacketHandler* _ph, NetConnection* _connection)
//{
//	PrintState();
//}
//
//void UsernameState::PrintState()
//{
//	
//}