#ifndef PROJEKTCLIENT_H
#define PROJEKTCLIENT_H


#include "Network/BaseNetwork.h"

class DECLSPEC Client : public BaseNetwork
{
public:

	Client();
	~Client();

	void Connect();
	void Connect(const char* _ipAddress, const char* m_password, const int m_port, const int m_clientPort);
	void Disconect();

	void SendToServer(PacketHandler::Packet _packet);

	const char* GetRemoteAddress(void) { return m_remoteAddress.c_str(); }
	const int GetOutgoingPort(void) { return m_outgoingPort; }

	void SetRemoteAddress(const char *_ip) { m_remoteAddress = _ip; }
	void SetOutgoingPort(const int _port) { m_outgoingPort = _port; }

	// Bind function which will trigger when the client connect to the server
	void SetOnConnectedToServer(NetEvent _function);
	// Bind function which will trigger when the client disconnect from the server
	void SetOnDisconnectedFromServer(NetEvent _function);
	// Bind function which will trigger when the client fails to connect to the server
	void SetOnFailedToConnect(NetEvent _function);

private:
	void ReceivePackets(void);

private:

#pragma warning( disable : 4251 )

	std::string m_remoteAddress;
	int m_outgoingPort;

	NetEvent m_onConnectedToServer;
	NetEvent m_onDisconnectedFromServer;
	NetEvent m_onFailedToConnect;

#pragma warning( default : 4251 )

};

#endif
