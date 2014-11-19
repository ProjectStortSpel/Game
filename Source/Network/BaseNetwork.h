#ifndef BASENETWORK_H
#define BASENETWORK_H

#include <SDL/SDL.h>
#include <queue>
#include <mutex>
#include <thread>
#include <RakNet/RakPeerInterface.h>

#include "Network/PacketHandler.h"
#include "Network/Stdafx.h"

class DECLSPEC BaseNetwork
{


public:
	BaseNetwork();
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

	// Bind function which will trigger when another player connects to the server
	void SetOnPlayerConnected(NetEvent _function);
	// Bind function which will trigger when another player disconnects from the server
	void SetOnPlayerDisconnected(NetEvent _function);

	// Start listen for packets.
	// Will be called when Start() on the server, or Connect() on the client is called
	void StartListen();
	// Stop listen for packets
	// Will be called when Stop() on the server, or Disconnect() on the client is called
	void StopListen();

protected:
	virtual void ReceivePackets(void) = 0;
	unsigned char GetPacketIdentifier(RakNet::Packet *p);
	void TriggerEvent(NetEvent _function, unsigned char _identifier);

protected:

#pragma warning( disable : 4251 )

	std::string m_localAddress;
	std::string m_password;
	unsigned int m_incomingPort;
	bool m_receiveThreadAlive;

	RakNet::RakPeerInterface* m_rakInterface;
	std::queue<PacketHandler::Packet*> m_packets;
	std::mutex m_packetLock;

	NetEvent m_onPlayerConnected;
	NetEvent m_onPlayerDisconnected;

#pragma warning( default : 4251 )

private:
	void Run(void);

private:

#pragma warning( disable : 4251 )

	std::thread m_thread;

#pragma warning( default : 4251 )

};

#endif