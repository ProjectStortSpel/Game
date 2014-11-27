#ifndef SERVERNETWORK_H
#define SERVERNETWORK_H

#include <thread>

#include "Network/BaseNetwork.h"

class DECLSPEC ServerNetwork : public BaseNetwork
{
public:
	ServerNetwork(void);
	~ServerNetwork(void);

	bool Start(unsigned int _incomingPort, const char* _password, unsigned int _maxConnections);
	bool Start(void);
	bool Stop(void);

	void Broadcast(Packet _packet, NetConnection _exclude = NetConnection());
	void Send(Packet _packet, NetConnection _connection);


	// Bind function which will trigger when another player connects to the server
	void SetOnPlayerConnected(NetMessageHook _function);
	// Bind function which will trigger when another player disconnects from the server
	void SetOnPlayerDisconnected(NetMessageHook _function);

	// Bind function which will trigger when another player disconnects from the server
	void SetOnPlayerTimedOut(NetMessageHook _function);

private:
	void ReceivePackets(ISocket* _socket, int _id);
	void ListenForConnections(void);

private:

	bool m_listenForConnectionsAlive;
	std::thread m_listenForConnectionsThread;

	int m_maxConnections;

	ISocket* m_listenSocket;
	std::map<NetConnection, ISocket*> m_connectedClients;

	std::vector<std::thread> m_receivePacketsThreads;
	std::vector<bool> m_receivePacketsAlive;

};

#endif