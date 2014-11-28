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

typedef std::function<void(PacketHandler*, NetConnection)> NetMessageHook;

class DECLSPEC BaseNetwork
{

	friend class PacketHandler;

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
	void HandlePacket(Packet* _packet);	
protected:

#pragma warning( disable : 4251 )

	std::vector<NetConnection> m_connections;

	std::map < std::string, std::function<void(PacketHandler*, Packet*)> > m_userFunctions;
	std::map < char, std::function<void(PacketHandler*, Packet*)> > m_networkFunctions;

	std::queue<Packet*> m_packets;
	std::mutex m_packetLock;

	std::string m_localAddress;
	std::string m_password;
	unsigned int m_incomingPort;

	PacketHandler m_packetHandler;

	char m_packetData[MAX_PACKET_SIZE];

#pragma warning( default : 4251 )

private:
	std::function<void(PacketHandler*, Packet*)>* GetUserFunction(std::string _functionName);
	std::function<void(PacketHandler*, Packet*)>* GetNetworkFunction(char _functionIdentifier);

};

#endif