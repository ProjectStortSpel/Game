#ifndef SERVERSTATE_H
#define SERVERSTATE_H

#include "Network/ServerNetwork.h"


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



class ServerState
{
protected:

	class GameServer* m_gameServer;

public:
	ServerState(GameServer* _gameServer);
	~ServerState(void);

	virtual void Update() = 0;

	virtual void SelectedCards(PacketHandler* _ph, NetConnection* _connection) = 0;
	virtual void SetUserName(PacketHandler* _ph, NetConnection* _connection) = 0;

	virtual void OnPlayerConnected(char _token, NetConnection* _connection) = 0;
	virtual void OnPlayerDisonnected(char _token, NetConnection* _connection) = 0;
};

class GameServer
{
private:
	ServerState* m_currentState;
	PacketHandler m_packetHandler;

	// Game items
	std::vector<Player> m_players;
	std::vector<MovementCard> m_mcardDeck;
	std::vector<AbilityCard> m_acardDeck;
	MovementCard *m_playedCards;

public:

	GameServer();
	~GameServer();

	void Update();

	void SetState(std::string _state);

	void SetState(ServerState* _state) { SAFE_DELETE(m_currentState); m_currentState = _state; }

	void SelectedCards(PacketHandler* _ph, NetConnection* _connection) { m_currentState->SelectedCards(_ph, _connection); }
	void SetUserName(PacketHandler* _ph, NetConnection* _connection) { m_currentState->SetUserName(_ph, _connection); }

	void OnPlayerConnected(char _token, NetConnection* _connection) { m_currentState->OnPlayerConnected(_token, _connection); }
	void OnPlayerDisonnected(char _token, NetConnection* _connection) { m_currentState->OnPlayerDisonnected(_token, _connection); }

};



class LobbyState : public ServerState
{

public:

	LobbyState(GameServer* _gameServer);
	~LobbyState(void);

	void Update();

	void SelectedCards(PacketHandler* _ph, NetConnection* _connection);
	void SetUserName(PacketHandler* _ph, NetConnection* _connection);

	void OnPlayerConnected(char _token, NetConnection* _connection);
	void OnPlayerDisonnected(char _token, NetConnection* _connection);


};

class DealState : public ServerState
{

public:

	DealState(GameServer* _gameServer);
	~DealState(void);

	void Update();

	void SelectedCards(PacketHandler* _ph, NetConnection* _connection);
	void SetUserName(PacketHandler* _ph, NetConnection* _connection);

	void OnPlayerConnected(char _token, NetConnection* _connection);
	void OnPlayerDisonnected(char _token, NetConnection* _connection);


};

class StepState : public ServerState
{

public:

	StepState(GameServer* _gameServer);
	~StepState(void);

	void Update();

	void SelectedCards(PacketHandler* _ph, NetConnection* _connection);
	void SetUserName(PacketHandler* _ph, NetConnection* _connection);

	void OnPlayerConnected(char _token, NetConnection* _connection);
	void OnPlayerDisonnected(char _token, NetConnection* _connection);


};


#endif