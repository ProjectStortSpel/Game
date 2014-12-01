#ifndef CLIENTNETWORK_H
#define CLIENTNETWORK_H

#include <thread>

#include "BaseNetwork.h"

class DECLSPEC ClientNetwork : public BaseNetwork
{
public:

	ClientNetwork();
	~ClientNetwork();

	// Connect to server using already predefined settings
	bool Connect();

	// Connect to server with Ip Address, Password, outgoing port & incoming port
	// Note that all previously information will be overridden
	bool Connect(const char* _ipAddress, const char* _password, const int _outgoing, const int _incomingPort);
	// Disconnect from the server 
	// This is called in the deconstructor and is not necessary to close the program graceful
	void Disconnect();
	// Send a packet to the server
	void Send(Packet* _packet);
	
	// Returns the remote Ip Address the client connects to
	const char* GetRemoteAddress(void) { return m_remoteAddress.c_str(); }
	// Returns the outgoing port the client connects to
	const int GetOutgoingPort(void) { return m_outgoingPort; }

	// Set the remote Ip Address the client will connect to
	void SetRemoteAddress(const char* _ipAddress) { m_remoteAddress = _ipAddress; }
	// Set the outgoing port which the client will connect to
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

	void NetPasswordInvalid(PacketHandler* _packetHandler, Packet* _packet);
	void NetConnectionAccepted(PacketHandler* _packetHandler, Packet* _packet);

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