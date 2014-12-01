#ifndef BASENETWORK_H
#define BASENETWORK_H

#include <vector>
#include <map>
#include <queue>
#include <mutex>

#include "Stdafx.h"
#include "NetTypeMessageID.h"

#ifdef WIN32
#include "WinSocket.h"
#else
#include "LinSocket.h"
#endif

#include "PacketHandler.h"
#include "Packet.h"

typedef std::function<void(PacketHandler*, uint64_t, NetConnection)> NetMessageHook;

class DECLSPEC BaseNetwork
{

	friend class PacketHandler;

public:
	BaseNetwork();
	virtual ~BaseNetwork();

	// Returns get local Ip Address
	const char* GetLocalAddress(void) { return m_localAddress.c_str(); }
	// Returns the server password
	const char* GetServerPassword(void) { return m_password.c_str(); }
	// Returns the incoming port
	const int GetIncomingPort(void) { return m_incomingPort; }

	// Reads the oldest user specific packet and calls its specified function
	// Will return the number of packets remaining
	int TriggerPacket(void);

	// Set the incoming port
	void SetIncomingPort(const int _port) { m_incomingPort = _port; }
	// Set the server password
	void SetServerPassword(const char* _password) { m_password = _password; }

	//NetMessageHook* GetNetworkFunction(NetTypeMessageId _function);

protected:
	void TriggerEvent(NetMessageHook _function, uint64_t _packetId, NetConnection _connection);
	void HandlePacket(Packet* _packet);	
protected:

#pragma warning( disable : 4251 )

	std::vector<NetConnection> m_connections;

	std::map < std::string, NetMessageHook > m_userFunctions;
	std::map < char, NetMessageHook > m_networkFunctions;

	std::queue<Packet*> m_packets;
	std::mutex m_packetLock;

	std::string m_localAddress;
	std::string m_password;
	unsigned int m_incomingPort;

	PacketHandler m_packetHandler;

	char m_packetData[MAX_PACKET_SIZE];

#pragma warning( default : 4251 )

private:
	NetMessageHook* GetUserFunction(std::string _functionName);
	NetMessageHook* GetNetworkFunction(char _functionIdentifier);

};

#endif