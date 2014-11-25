#ifndef GAMESERVER_H
#define GAMESERVER_H

#include "Network/Server.h"
#include "Game/GameShared.h"


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


class GameServer;

class ServerState
{
protected:

	GameServer* m_gameServer = NULL;

public:
	ServerState(GameServer* _gameServer);
	~ServerState(void);

	virtual void Update(){}

	virtual void SelectedCards(PacketHandler* _ph, NetConnection* _connection){}
	virtual void SetUsername(PacketHandler* _ph, NetConnection* _connection){}

	virtual void OnPlayerConnected(NetConnection* _connection){}
	virtual void OnPlayerDisonnected(NetConnection* _connection){}
};

class GameServer
{
private:
	ServerState* m_currentState = NULL;
	PacketHandler m_packetHandler;

	// Game items
	std::vector<Player> m_players;
	std::vector<MovementCard> m_mCardDeck;
	std::vector<AbilityCard> m_aCardDeck;
	MovementCard *m_playedCards = NULL;

	Server* m_server = NULL;

	int m_cardsInHand = 8;
	int m_cardsToPlay = 5;



public:

	GameServer();
	~GameServer();

	void Update();

	void SetState(ServerState* _state) { SAFE_DELETE(m_currentState); m_currentState = _state; }

	void SelectedCards(PacketHandler* _ph, NetConnection* _connection) { if (m_currentState) m_currentState->SelectedCards(_ph, _connection); }
	void SetUsername(PacketHandler* _ph, NetConnection* _connection) { if (m_currentState) m_currentState->SetUsername(_ph, _connection); }

	void OnPlayerConnected(NetConnection* _connection) { if (m_currentState) m_currentState->OnPlayerConnected(_connection); }
	void OnPlayerDisonnected(NetConnection* _connection) { if (m_currentState) m_currentState->OnPlayerDisonnected(_connection); }

	Server* GetServer() { return m_server; }

	std::vector<Player>* GetPlayers() { return &m_players; }

	MovementCard* GetPlayedCards() { return m_playedCards; }

	int GetCardsInHand() { return m_cardsInHand; }
	int GetCardsToPlay() { return m_cardsToPlay; }

	std::vector<MovementCard>* GetMovementCardDeck() { return &m_mCardDeck; };

	PacketHandler* GetPacketHandler() { return &m_packetHandler; }

	void ResetGame();

};



class LobbyState : public ServerState
{

	void PrintState();

public:

	LobbyState(GameServer* _gameServer);
	~LobbyState(void);

	void Update();

	void SetUsername(PacketHandler* _ph, NetConnection* _connection);

	void OnPlayerConnected(NetConnection* _connection);
	void OnPlayerDisonnected(NetConnection* _connection);


};

class DealState : public ServerState
{
private:
	std::vector<NetConnection*> m_playersWaitingFor;

public:

	DealState(GameServer* _gameServer);
	~DealState(void);

	void Update();

	void SelectedCards(PacketHandler* _ph, NetConnection* _connection);

	void OnPlayerConnected(NetConnection* _connection);
	void OnPlayerDisonnected(NetConnection* _connection);


};

class StepState : public ServerState
{

	int m_currentStep;

public:

	StepState(GameServer* _gameServer);
	~StepState(void);

	void Update();

	void OnPlayerConnected(NetConnection* _connection);
	void OnPlayerDisonnected(NetConnection* _connection);


};


#endif