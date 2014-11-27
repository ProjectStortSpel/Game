#ifndef BASENETWORK_H
#define BASENETWORK_H

#include <SDL/SDL.h>
#include <queue>
#include <mutex>
#include <thread>
#include <map>

#include "Network/Stdafx.h"
//#include "Network/PacketHandler.h"
#include "Network/ISocket.h"
#include "Network/NetTypeMessageID.h"

#ifdef WIN32
	#include "Network/WinSocket.h"
#else
	#include "Network/LinSocket.h"
#endif

class PacketHandler;
struct Packet;

typedef std::function<void(PacketHandler*, NetConnection)> NetMessageHook;

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
	Packet* GetPacket();

	// Set the incoming port
	// This is the port the client will connect WITH
	// and the port the server will allow clients to connect to
	void SetIncomingPort(const int _port) { m_incomingPort = _port; }
	// Set the server password
	void SetServerPassword(const char* _password) { m_password = _password; }

	NetMessageHook* GetNetworkFunction(NetTypeMessageId _function);

protected:
	void TriggerEvent(NetEvent _function, NetConnection _connection);

protected:

#pragma warning( disable : 4251 )

	std::vector<NetConnection> m_connections;
	std::map<NetTypeMessageId, NetMessageHook> m_networkFunctionMap;
	//std::map<NetConnection, RakNet::SystemAddress> m_addressMap;
	//std::map<RakNet::SystemAddress, NetConnection> m_connectionMap;

	std::string m_localAddress;
	std::string m_password;
	unsigned int m_incomingPort;

	std::queue<Packet*> m_packets;
	std::mutex m_packetLock;

	char m_packetData[MAX_PACKET_SIZE];

#pragma warning( default: 4251)

private:

private:
};

#endif