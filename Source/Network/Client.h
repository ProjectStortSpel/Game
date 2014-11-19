#ifndef PROJEKTCLIENT_H
#define PROJEKTCLIENT_H


#include "Network/BaseNetwork.h"

class DECLSPEC Client : public BaseNetwork
{
public:

	Client();
	~Client();

	// Connect to the server using already set values on the ip address, password, outoing port & incoming port
	void Connect();
	// Connect to the server using specific values on the ip address, password, outgoing port & incoming port
	// Note that any old values will be overriden
	void Connect(const char* _ipAddress, const char* m_password, const int _outgoingPort, const int _incomingPort);
	// Disconnect from the conencted server
	void Disconect();

	// Send a packet to the connected server
	void SendToServer(PacketHandler::Packet _packet);

	// Return the remote address the client will use to connect with
	const char* GetRemoteAddress(void) { return m_remoteAddress.c_str(); }
	// Return the outgoing port the client will use to connect with
	const int GetOutgoingPort(void) { return m_outgoingPort; }

	// Set the remote address the client will use to connect with
	void SetRemoteAddress(const char *_ip) { m_remoteAddress = _ip; }
	// Set the outgoing port the client will use to connect with
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
