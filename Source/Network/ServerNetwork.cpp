#include "ServerNetwork.h"

//#include <algorithm>

#ifdef WIN32
#else
#include <sys/socket.h>
#endif

ServerNetwork::ServerNetwork()
	: BaseNetwork()
{
	m_incomingPort = 5357;
	m_maxConnections = 8;

	m_listenForConnectionsAlive = false;
	m_maxConnections = 8;

	m_listenSocket = 0;

}

ServerNetwork::~ServerNetwork()
{
	Stop();
}

bool ServerNetwork::Start(unsigned int _incomingPort, const char* _password, unsigned int _maxConnections)
{
	m_incomingPort = _incomingPort;
	m_password = _password;
	m_maxConnections = _maxConnections;

	return Start();
}

bool ServerNetwork::Start()
{
	m_listenSocket = ISocket::CreateSocket();
	m_listenSocket->SetNonBlocking(true);
	m_listenSocket->Bind(m_incomingPort);

	if (NET_DEBUG)
	{
		printf("Starting server:\n");
		printf("Ip address: \"INSERT IP HERE\"\n");
		printf("Port: \"%i\"\n", m_incomingPort);
		printf("Password: \"%s\"\n", m_password.c_str());
		printf("Max connections: \"%i\"\n", m_maxConnections);
	}

	if (!m_listenSocket->Listen(128))
		return false;

	m_listenForConnectionsAlive = true;
	m_listenForConnectionsThread = std::thread(&ServerNetwork::ListenForConnections, this);

	return true;
}

bool ServerNetwork::Stop()
{
	for (int i = 0; i < m_receivePacketsAlive.size(); ++i)
		m_receivePacketsAlive[i] = false;

	for (int i = 0; i < m_receivePacketsThreads.size(); ++i)
		m_receivePacketsThreads[i].join();

	if (m_listenForConnectionsAlive)
	{
		m_listenForConnectionsAlive = false;
		m_listenForConnectionsThread.join();
	}

	if (m_listenSocket)
		SAFE_DELETE(m_listenSocket);

	for (auto it = m_connectedClients.begin(); it != m_connectedClients.end(); ++it)
		SAFE_DELETE(it->second);
	m_connectedClients.clear();

	return true;
}

void ServerNetwork::Broadcast(Packet _packet, NetConnection _exclude)
{
}

void ServerNetwork::Send(Packet _packet, NetConnection _receiver)
{

}

void ServerNetwork::ReceivePackets(ISocket* _socket, int _id)
{
	while (m_receivePacketsAlive[_id])
	{
		int result = _socket->Receive(m_packetData, MAX_PACKET_SIZE);
		
		if (result > 0)
		{
			unsigned short packetSize = result;

			if (NET_DEBUG)
				printf("Received message with length \"%i\" from server.\n", packetSize);

			Packet p;
			p.Data = new unsigned char[packetSize];
			p.Length = packetSize;
			p.Sender = _socket->GetNetConnection();
			memcpy(p.Data, m_packetData, packetSize);

		}
		else if (result == 0)
		{
		}
		else
		{
		}

	}
}
void ServerNetwork::ListenForConnections(void)
{
	if (NET_DEBUG)
		printf("Start listen for incoming connections.\n");

	while (m_listenForConnectionsAlive)
	{
		NetSleep(50);

		ISocket* newConnection = m_listenSocket->Accept();
		if (!newConnection)
			continue;

		NetConnection nc = newConnection->GetNetConnection();

		if (NET_DEBUG)
			printf("New incoming connection from %s:%d\n", nc.IpAddress.c_str(), nc.Port);

		m_receivePacketsAlive.push_back(true);
		m_receivePacketsThreads.push_back(std::thread(&ServerNetwork::ReceivePackets, this, newConnection, m_receivePacketsThreads.size()));
	}
}