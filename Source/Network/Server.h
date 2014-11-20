#ifndef PROJEKTSERVER_H
#define PROJEKTSERVER_H

#include "Network/BaseNetwork.h"

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
	// Shutdown the server
	void Stop();

	// Broadcast a message to all connected clients
	// Use _exclude to tell the server to not broadcast to a specific client
	void Broadcast(PacketHandler::Packet _packet, NetConnection *_exclude = NULL);
	// Send a message to a specific connected client
	void Send(PacketHandler::Packet _packet, NetConnection _connection);
	// Kick a connected client for a specific reason
	void KickClient(NetConnection* _connection, const char* _reason);
	// Ban a ip address for a specific reason
	// _milliseconds determines how long the ip address will be banned
	void BanClient(NetConnection* _connection, const char* _reason, unsigned int _milliseconds = 0);
	// Unban a ip address from the server
	void UnbanClient(NetConnection* _connection);

private:
	void ReceivePackets(void);

private:
	int m_maxConnections;


};

#endif