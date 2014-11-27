#ifndef PROJEKTSERVER_H
#define PROJEKTSERVER_H

#include "Network/BaseNetwork.h"
#include "Network/Stdafx.h"
#include "Network/PacketHandler.h"

class DECLSPEC Server : public BaseNetwork
{

public:
	Server();
	~Server();

	// Start the server using specific values on the incomging port, password and max connections
	// Note that any old values will be overriden
	void Start(unsigned int _incomingPort, const char* _password, unsigned int _maxConnections);
	// Start the server using the already defined servers
	void Start();
	// Shutdown the server. Is called through the Decontructor and is not needed if you do not want to close the server early.
	void Stop();

	// Broadcast a message to all connected clients
	// Use _exclude to tell the server to not broadcast to a specific client
	void Broadcast(Packet _packet, NetConnection *_exclude = NULL);
	// Send a message to a specific connected client
	void Send(Packet _packet, NetConnection _connection);
	// Kick a connected client for a specific reason
	void KickClient(NetConnection* _connection, const char* _reason);
	// Ban a ip address for a specific reason
	// _milliseconds determines how long the ip address will be banned
	void BanClient(NetConnection* _connection, const char* _reason, unsigned int _milliseconds = 0);
	// Unban a ip address from the server
	void UnbanClient(NetConnection* _connection);


	// Bind function which will trigger when another player connects to the server
	void SetOnPlayerConnected(NetEvent _function);
	// Bind function which will trigger when another player disconnects from the server
	void SetOnPlayerDisconnected(NetEvent _function);

	// Bind function which will trigger when another player disconnects from the server
	void SetOnPlayerTimedOut(NetEvent _function);

private:
	void ReceivePackets(ISocket* _socket, int _id);
	void ListenForConnections(void);

	void NetConnectionAccepted(PacketHandler* _packetHandler, NetConnection _netConnection);

private:


#pragma warning( disable : 4251 )

	bool m_listenForConnectionsThreadAlive;
	int m_maxConnections;
	NetEvent m_onPlayerConnected;
	NetEvent m_onPlayerDisconnected;
	NetEvent m_onPlayerTimedOut;

	std::map<NetConnection, ISocket*> m_connectionClients;

	std::thread m_newConnectionsThread;

	ISocket* m_listenSocket;

	std::vector<std::thread> m_receivePacketThreads;
	std::vector<bool> m_receivePacketsActive;

#pragma warning( default : 4251 )


};

#endif