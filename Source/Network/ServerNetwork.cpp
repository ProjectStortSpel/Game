#include "ServerNetwork.h"

//#include <algorithm>

#ifdef WIN32
#else
#include <sys/socket.h>
#endif

void ServerNetwork::TestNetwork(PacketHandler* _packetHandler, Packet* _packet)
{
	printf("TestNetwork.\n");
}

void ServerNetwork::TestUser(PacketHandler* _packetHandler, Packet* _packet)
{
	printf("TestUser.\n");
}

void ServerNetwork::TestNewUser(PacketHandler* _packetHandler, Packet* _packet)
{
	char type = _packetHandler->StartUnpack(_packet);

	switch (type)
	{
	case NetTypeMessageId::ID_PASSWORD_ATTEMPT:
	{
		std::string password = _packetHandler->ReadString();
		if (m_password.compare(password) == 0)
		{
			_packetHandler->StartPack(NetTypeMessageId::ID_CONNECTION_ACCEPTED);
			auto newPacket = _packetHandler->EndPack();
			m_connectedClients[_packet->Sender]->Send((char*)newPacket->Data, newPacket->Length);
		}
		else
		{
			_packetHandler->StartPack(NetTypeMessageId::ID_PASSWORD_INVALID);
			auto newPacket = _packetHandler->EndPack();
			m_connectedClients[_packet->Sender]->Send((char*)newPacket->Data, newPacket->Length);
			SAFE_DELETE(newPacket);
		}
		break;
	}
	case NetTypeMessageId::ID_PASSWORD_INVALID:
		break;
	default:
		break;
	}

	_packetHandler->EndUnpack();
}

ServerNetwork::ServerNetwork()
	: BaseNetwork()
{
	m_incomingPort = 5357;
	m_maxConnections = 8;

	m_listenForConnectionsAlive = false;
	m_maxConnections = 8;

	m_listenSocket = 0;

	m_networkFunctions[NetTypeMessageId::ID_PASSWORD_ATTEMPT] = std::bind(&ServerNetwork::TestNewUser, this, std::placeholders::_1, std::placeholders::_2);
	m_networkFunctions[NetTypeMessageId::ID_PASSWORD_INVALID] = std::bind(&ServerNetwork::TestNewUser, this, std::placeholders::_1, std::placeholders::_2);

	m_userFunctions["Test"] = std::bind(&ServerNetwork::TestUser, this, std::placeholders::_1, std::placeholders::_2);

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

	m_receivePacketsAlive.clear();
	m_receivePacketsThreads.clear();

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

void ServerNetwork::Broadcast(Packet* _packet, NetConnection _exclude)
{
	for (auto it = m_connectedClients.begin(); it != m_connectedClients.end(); it++)
	{
		if (it->first == _exclude)
			continue;

		it->second->Send((char*)_packet->Data, _packet->Length);
	}

	SAFE_DELETE(_packet);
}

void ServerNetwork::Send(Packet* _packet, NetConnection _receiver)
{
	if (m_connectedClients.find(_receiver) == m_connectedClients.end())
	{
		if (NET_DEBUG)
			printf("Connection to receiver \"%s:%i\" was not found.\n", _receiver.IpAddress.c_str(), _receiver.Port);
		SAFE_DELETE(_packet);
		return;
	}

	m_connectedClients[_receiver]->Send((char*)_packet->Data, _packet->Length);

	SAFE_DELETE(_packet);
}

void ServerNetwork::ReceivePackets(ISocket* _socket, int _id)
{
	while (m_receivePacketsAlive[_id])
	{
		int result = _socket->Receive(m_packetData, MAX_PACKET_SIZE);
		
		if (result > 0)
		{
			unsigned short packetSize = result;

			Packet* p = new Packet();
			p->Data = new unsigned char[packetSize];
			p->Length = packetSize;
			p->Sender = _socket->GetNetConnection();
			memcpy(p->Data, m_packetData, packetSize);

			if (NET_DEBUG)
				printf("Received message with length \"%i\" from client \"%s:%i\".\n", packetSize, p->Sender.IpAddress.c_str(), p->Sender.Port);
			HandlePacket(p);


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
		m_connectedClients[nc] = newConnection;

		if (NET_DEBUG)
			printf("New incoming connection from %s:%d\n", nc.IpAddress.c_str(), nc.Port);

		m_receivePacketsAlive.push_back(true);
		m_receivePacketsThreads.push_back(std::thread(&ServerNetwork::ReceivePackets, this, newConnection, m_receivePacketsThreads.size()));
	}
}