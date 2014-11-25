#include "GameServer.h"


GameServer::GameServer()
{
	m_currentState = 0;
}

GameServer::~GameServer()
{
	SAFE_DELETE(m_currentState);
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

}

void LobbyState::Update()
{
	/*
	
	if (Enter är tryckt && 2 spelare är anslutna)
	{
		
		m_gameServer->SetState(new DealState());
		
	}
	
	*/
}

void LobbyState::SelectedCards(PacketHandler* _ph, NetConnection* _connection)
{
	//return
}

void LobbyState::SetUserName(PacketHandler* _ph, NetConnection* _connection)
{
	//ändra namn
	//skicka ok
}

void LobbyState::OnPlayerConnected(char _token, NetConnection* _connection)
{
	//adda honom
}

void LobbyState::OnPlayerDisonnected(char _token, NetConnection* _connection)
{
	//ta bort honom
}


DealState::DealState(GameServer* _gameServer)
	: ServerState(_gameServer)
{

}

DealState::~DealState(void)
{

}

void DealState::Update()
{
	/*
	
	if (tiden är slut)
	{
		sluta vänta på resterande spelare
	}

	if (antal spelare vi väntar på == 0)
	{
		
		m_gameServer->SetState(new StepState());

	}
	
	*/
}

void DealState::SelectedCards(PacketHandler* _ph, NetConnection* _connection)
{
	//välj kort
}

void DealState::SetUserName(PacketHandler* _ph, NetConnection* _connection)
{
	//return
}

void DealState::OnPlayerConnected(char _token, NetConnection* _connection)
{
	//kicka honom
}

void DealState::OnPlayerDisonnected(char _token, NetConnection* _connection)
{
	//ta bort honom
	//sluta vänta på den spelaren
}




StepState::StepState(GameServer* _gameServer)
	: ServerState(_gameServer)
{

}

StepState::~StepState(void)
{

}

void StepState::Update()
{
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

void StepState::SelectedCards(PacketHandler* _ph, NetConnection* _connection)
{
	//return
}

void StepState::SetUserName(PacketHandler* _ph, NetConnection* _connection)
{
	//return
}

void StepState::OnPlayerConnected(char _token, NetConnection* _connection)
{
	//Kicka honom
}

void StepState::OnPlayerDisonnected(char _token, NetConnection* _connection)
{
	//ta bort honom
}