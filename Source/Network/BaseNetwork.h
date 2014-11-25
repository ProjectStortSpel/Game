#ifndef BASENETWORK_H
#define BASENETWORK_H

#include <SDL/SDL.h>
#include <queue>
#include <mutex>
#include <thread>

#include "Network/Stdafx.h"
#include "Network/PacketHandler.h"
#include "Network/ISocket.h"

class DECLSPEC BaseNetwork
{


public:

	virtual ~BaseNetwork();

	// Return the local address
	const char* GetLocalAddress(void) { return m_localAddress.c_str(); }
	// Return the server password
	const char* GetServerPassword(void) { return m_password.c_str(); }
	// Return the incoming port 
	// This is the port the client will connect WITH
	// and the port the server will allow clients to connect to
	const int	GetIncomingPort(void) { return m_incomingPort; }

	// Will return a packet if any packet has been received
	PacketHandler::Packet* GetPacket();

	// Set the incoming port
	// This is the port the client will connect WITH
	// and the port the server will allow clients to connect to
	void SetIncomingPort(const int _port) { m_incomingPort = _port; }
	// Set the server password
	void SetServerPassword(const char* _password) { m_password = _password; }

	// Start listen for packets.
	// Will be called when Start() on the server, or Connect() on the client is called
	void StartListen();
	// Stop listen for packets
	// Will be called when Stop() on the server, or Disconnect() on the client is called
	void StopListen();

protected:
	virtual void ReceivePackets(void) = 0;
	void TriggerEvent(NetEvent _function, unsigned char _identifier);// , RakNet::SystemAddress _address);

protected:
	std::vector<NetConnection> m_connections;
	//std::map<NetConnection, RakNet::SystemAddress> m_addressMap;
	//std::map<RakNet::SystemAddress, NetConnection> m_connectionMap;

	ISocket* m_listenSocket;

	std::string m_localAddress;
	std::string m_password;
	unsigned int m_incomingPort;
	bool m_receiveThreadAlive;

	std::queue<PacketHandler::Packet*> m_packets;
	std::mutex m_packetLock;

private:
};

#endif