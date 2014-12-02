#ifndef SERVERNETWORK_H
#define SERVERNETWORK_H

#include <thread>

#include "BaseNetwork.h"

class DECLSPEC ServerNetwork : public BaseNetwork
{

public:
	ServerNetwork(void);
	~ServerNetwork(void);

	// Start the server using predefined settings
	bool Start(void);
	// Start the server using custom incoming port, password & max number of connections
	// Note that all previously information will be overridden
	bool Start(unsigned int _incomingPort, const char* _password, unsigned int _maxConnections);
	// Stops the server
	// This is called from the deconstructor and is not needed to close the server gracyfully
	bool Stop(void);

	// Send a message to all connected clients
	// use _exclude to prevent the message to being sent to one client
	void Broadcast(Packet* _packet, NetConnection _exclude = NetConnection());
	// Send a message to a specific client
	void Send(Packet* _packet, NetConnection _connection);


	// Bind function which will trigger when another player connects to the server
	void SetOnPlayerConnected(NetEvent _function);
	// Bind function which will trigger when another player disconnects from the server
	void SetOnPlayerDisconnected(NetEvent _function);
	// Bind function which will trigger when another player disconnects from the server
	void SetOnPlayerTimedOut(NetEvent _function);

private:
	void ReceivePackets(ISocket* _socket);
	void ListenForConnections(void);

	void NetPasswordAttempt(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection);
	void NetConnectionLost(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection);
	void NetConnectionDisconnected(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection);

private:

#pragma warning( disable : 4251 )

	bool m_listenForConnectionsAlive;
	std::thread m_listenForConnectionsThread;

	int m_maxConnections;

	ISocket* m_listenSocket;
	std::map<NetConnection, ISocket*> m_connectedClients;

	std::map<NetConnection, std::thread> m_receivePacketsThreads;

	//std::map<NetConnection

	NetEvent m_onPlayerConnected;
	NetEvent m_onPlayerDisconnected;
	NetEvent m_onPlayerTimedOut;

#pragma warning( default : 4251 )

};

#endif