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

	const char* GetLocalAddress(void) { return m_localAddress.c_str(); }
	const char* GetServerPassword(void) { return m_password.c_str(); }
	const int	GetIncomingPort(void) { return m_incomingPort; }
	
	PacketHandler::Packet* GetPacket();

	void SetIncomingPort(const int _port) { m_incomingPort = _port; }
	void SetServerPassword(const char* _password) { m_password = _password; }

	// std::bind(&Class:Function, pointer to object, number of arguments (0)

	// Bind function which will trigger when another player connects to the server
	void SetOnPlayerConnected(NetEvent _function);
	// Bind function which will trigger when another player disconnects from the server
	void SetOnPlayerDisconnected(NetEvent _function);

	void StartListen();
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