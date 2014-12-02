#ifndef CLIENTNETWORK_H
#define CLIENTNETWORK_H

#include <thread>

#include "BaseNetwork.h"

class DECLSPEC ClientNetwork : public BaseNetwork
{
public:

	ClientNetwork();
	~ClientNetwork();

	bool Connect();
	bool Connect(const char* _ipAddress, const char* _password, const int _outgoing, const int _incomingPort);
	void Disconnect();
	void Send(Packet* _packet);

	const char* GetRemoteAddress(void) { return m_remoteAddress.c_str(); }
	const int GetOutgoingPort(void) { return m_outgoingPort; }

	void SetRemoteAddress(const char* _ipAddress) { m_remoteAddress = _ipAddress; }
	void SetOutgoingPort(const int _port) { m_outgoingPort = _port; }

	// Bind function which will trigger when the client connect to the server
	void SetOnConnectedToServer(NetMessageHook _function);
	// Bind function which will trigger when the client disconnect from the server
	void SetOnDisconnectedFromServer(NetMessageHook _function);
	// Bind function which will trigger when the client disconnect from the server
	void SetOnTimedOutFromServer(NetMessageHook _function);
	// Bind function which will trigger when the client fails to connect to the server
	void SetOnFailedToConnect(NetMessageHook _function);

	// Bind function which will trigger when the client connect to the server
	void SetOnRemotePlayerConnected(NetMessageHook _function);
	// Bind function which will trigger when the client disconnect from the server
	void SetOnRemotePlayerDisconnected(NetMessageHook _function);
	// Bind function which will trigger when the client fails to connect to the server
	void SetOnRemotePlayerTimedOut(NetMessageHook _function);

private:

	void ReceivePackets(void);

private:

#pragma warning( disable : 4251 )

	std::string m_remoteAddress;
	int m_outgoingPort;
	bool m_socketBound;

	ISocket* m_socket;

	std::thread m_receivePacketsThread;
	bool m_receivePacketsThreadAlive;

#pragma warning( default : 4251 )
};

#endif