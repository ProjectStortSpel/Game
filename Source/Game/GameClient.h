#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include "Network/Client.h"


class GameClient;

class ClientState
{
protected:

	GameClient* m_gameClient = NULL;

public:
	ClientState(GameClient* _gameClient);
	~ClientState(void);

	virtual void Update() {}

	virtual void SetUsername(PacketHandler* _ph, NetConnection* _connection) {}
	virtual void SetGameSettings(PacketHandler* _ph, NetConnection* _connection) {}

	virtual void OnConnect(NetConnection* _connection) {}
	virtual void OnDisconnect(NetConnection* _connection) {}
};


class GameClient
{
private:

	ClientState* m_currentState = NULL;

	bool m_connected;

	int m_cardsInHand;
	int m_cardsToPlay;
	std::string m_name;

	PacketHandler m_packetHandler;

	Client* m_client;

public:
	GameClient();
	~GameClient();

	void Update();

	void SetState(ClientState* _state) { SAFE_DELETE(m_currentState); m_currentState = _state; }

	void SendUsername(PacketHandler* _ph, NetConnection* _connection);
	void SetUsername(PacketHandler* _ph, NetConnection* _connection);
	void SelectCards(PacketHandler* _ph, NetConnection* _connection);
	
	void SetGameSettings(PacketHandler* _ph, NetConnection* _connection);

	void OnConnect(NetConnection* _connection);
	void OnDisconnect(NetConnection* _connection);


	std::string GetName() { return m_name; }

	PacketHandler* GetPacketHandler() { return &m_packetHandler; }

	Client* GetClient() { return m_client; }
};


class UsernameState : public ClientState
{

	void PrintState();

public:

	UsernameState(GameClient* _gameClient);
	~UsernameState();

	void Update();
	void SetUsername(PacketHandler* _ph, NetConnection* _connection);

};

class SelectCardState : public ClientState
{

	void PrintState();

public:

	SelectCardState(GameClient* _gameClient);
	~SelectCardState();

	void Update();
	void SetUsername(PacketHandler* _ph, NetConnection* _connection);

};

#endif