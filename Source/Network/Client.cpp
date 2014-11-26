#include "Client.h"

#include "Network/PacketHandler.h"

#ifdef WIN32
#else
#include <sys/socket.h>
#endif

Client::Client()
	: BaseNetwork()
{
	m_outgoingPort = 6112;
	m_incomingPort = 0;
	m_remoteAddress = "127.0.0.1";
	m_socketBound = false;
	m_receivePacketsThreadAlive = false;
}

Client::~Client()
{
	if (m_receivePacketsThreadAlive)
	{
		m_receivePacketsThreadAlive = false;
		m_receivePacketThread.join();
	}

	SAFE_DELETE(m_socket);
}

bool Client::Connect(const char* _ipAddress, const char* _password, const int _outgoingPort, const int _incomingPort)
{

	m_remoteAddress = _ipAddress;
	m_password		= _password;
	m_outgoingPort	= _outgoingPort;
	m_incomingPort	= _incomingPort;

	return Connect();
}
bool Client::Connect()
{
	m_socket = ISocket::CreateISocket(AF_INET, SOCK_STREAM, 0);

	if (NET_DEBUG)
	{
		printf("Client connecting to server:\n");
		printf("Ip address: \"%s\"\n", m_remoteAddress.c_str());
		printf("Remote Port: \"%i\"\n", m_outgoingPort);
		printf("Local Port: \"%i\"\n", m_incomingPort);
		printf("Password: \"%s\"\n", m_password.c_str());
	}

	if (!m_socketBound)
	{
		m_socket->Bind(m_incomingPort);
		m_socketBound = true;
	}

	if (!m_socket->Connect(m_remoteAddress.c_str(), m_outgoingPort))
		return false;

	m_receivePacketsThreadAlive = true;
	m_receivePacketThread = std::thread(&Client::ReceivePackets, this);

	if (NET_DEBUG)
		printf("Started listen on new thread.\n\n");

	return true;
}

void Client::Disconect()
{
	if (NET_DEBUG)
		printf("Client disconnected from server.\n");
	
	if (m_receivePacketsThreadAlive)
	{
		m_receivePacketsThreadAlive = false;
		m_receivePacketThread.join();
	}

	SAFE_DELETE(m_socket);
	m_socketBound = false;
}
//
void Client::SendToServer(Packet _packet)
{
	if (m_socketBound)
		m_socket->Send((char*)_packet.Data, _packet.Length, 0);

	if (NET_DEBUG)
		printf("Client sent packet to server.\n");
}

void Client::ReceivePackets()
{
	while (m_receivePacketsThreadAlive)
	{
		int result = m_socket->Recv(m_packetData, MAX_PACKET_SIZE, 0);

		if (result > 0)
		{
			unsigned int packetSize = result;

			if (NET_DEBUG)
				printf("Recieved message with length from server with length: %i.\n", packetSize);

			Packet* p = new Packet();
			p->Data		= new unsigned char[packetSize];
			p->Length	= packetSize;
			p->Sender	= m_socket->GetNetConnection();
			memcpy(p->Data, m_packetData, packetSize);

			m_packetLock.lock();
			m_packets.push(p);
			m_packetLock.unlock();

		}
		else if (result == 0)
		{
			// Connection shut down graceful
		}
		else
		{
			if (NET_DEBUG)
				printf("ReveivePackets failed. Error Code: %d\n.", WSAGetLastError());
		}
		
//		NetSleep(30);

	}
}

void Client::SetOnConnectedToServer(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnConnectedToServer.\n");

	m_onConnectedToServer = _function;
}

void Client::SetOnDisconnectedFromServer(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnDisconnectedFromServer.\n");

	m_onDisconnectedFromServer = _function;
}

void Client::SetOnTimedOutFromServer(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnTimedOutFromServer.\n");

	m_onTimedOutFromServer = _function;
}

void Client::SetOnFailedToConnect(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnFailedToConnect.\n");

	m_onFailedToConnect = _function;
}

void Client::SetOnRemotePlayerConnected(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnRemotePlayerConnected.\n");

	m_onRemotePlayerConnected = _function;
}

void Client::SetOnRemotePlayerDisconnected(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnRemotePlayerDisconnected.\n");

	m_onRemotePlayerDisconnected = _function;
}

void Client::SetOnRemotePlayerTimedOut(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnRemotePlayerTimedOut.\n");

	m_onRemotePlayerTimedOut = _function;
}
