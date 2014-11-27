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

typedef std::function<void(PacketHandler*, NetConnection)> NetMessageHook;

class DECLSPEC BaseNetwork
{
public:
	BaseNetwork();
	virtual ~BaseNetwork();

	const char* GetLocalAddress(void) { return m_localAddress.c_str(); }
	const char* GetServerPassword(void) { return m_password.c_str(); }
	const int GetIncomingPort(void) { return m_incomingPort; }

	//Packet* GetPacket();

	void SetIncomingPort(const int _port) { m_incomingPort = _port; }
	void SetServerPassword(const char* _password) { m_password = _password; }

	//NetMessageHook* GetNetworkFunction(NetTypeMessageId _function);

protected:
	void TriggerEvent(NetMessageHook _function, NetConnection _connection);
protected:

	std::vector<NetConnection> m_connections;
	std::map<NetTypeMessageId, NetMessageHook> m_networkFunctionMap;

	std::queue<Packet*> m_packets;
	//std::mutex m_packetLock;

	std::string m_localAddress;
	std::string m_password;
	unsigned int m_incomingPort;

	char m_packetData[MAX_PACKET_SIZE];

};

#endif