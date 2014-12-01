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
	void SetOnPlayerConnected(NetMessageHook _function);
	// Bind function which will trigger when another player disconnects from the server
	void SetOnPlayerDisconnected(NetMessageHook _function);

	// Bind function which will trigger when another player disconnects from the server
	void SetOnPlayerTimedOut(NetMessageHook _function);

private:
	void ReceivePackets(ISocket* _socket, int _id);
	void ListenForConnections(void);

	void TestNetwork(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection);
	void TestUser(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection);

	void TestNewUser(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection);

private:

#pragma warning( disable : 4251 )

	bool m_listenForConnectionsAlive;
	std::thread m_listenForConnectionsThread;

	int m_maxConnections;

	ISocket* m_listenSocket;
	std::map<NetConnection, ISocket*> m_connectedClients;

	std::vector<std::thread> m_receivePacketsThreads;
	std::vector<bool> m_receivePacketsAlive;

#pragma warning( default : 4251 )

};

#endif