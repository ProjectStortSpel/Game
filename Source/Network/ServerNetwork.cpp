#include "ServerNetwork.h"

//#include <algorithm>

#ifdef WIN32
#else
#include <sys/socket.h>
#endif



void ServerNetwork::NetPasswordAttempt(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection)
{
	char type = _packetHandler->GetNetTypeMessageId(_id);

	std::string password = _packetHandler->ReadString(_id);
	if (m_password.compare(password) == 0)
	{
		uint64_t id2 = _packetHandler->StartPack(NetTypeMessageId::ID_CONNECTION_ACCEPTED);
		auto newPacket = _packetHandler->EndPack(id2);
		m_connectedClients[_connection]->SetAccepted(true);
		Send(newPacket, _connection);

		if (m_onPlayerConnected)
			m_onPlayerConnected(_connection);
	}
	else
	{
		uint64_t id2 = _packetHandler->StartPack(NetTypeMessageId::ID_PASSWORD_INVALID);
		auto newPacket = _packetHandler->EndPack(id2);
		Send(newPacket, _connection);
		SAFE_DELETE(m_connectedClients[_connection]);
		m_connectedClients.erase(_connection);
	}
}

void ServerNetwork::NetConnectionLost(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection)
{
	m_connectedClients[_connection]->SetAccepted(false);
	if (m_onPlayerTimedOut)
		m_onPlayerTimedOut(_connection);
}

void ServerNetwork::NetConnectionDisconnected(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection)
{

	

	SAFE_DELETE(m_connectedClients[_connection]);
	if(m_onPlayerDisconnected)
		m_onPlayerDisconnected(_connection);
}

ServerNetwork::ServerNetwork()
	: BaseNetwork()
{
	m_incomingPort = 5357;
	m_maxConnections = 8;

	m_listenForConnectionsAlive = false;
	m_maxConnections = 8;

	m_listenSocket = 0;

	m_networkFunctions[NetTypeMessageId::ID_PASSWORD_ATTEMPT] = std::bind(&ServerNetwork::NetPasswordAttempt, this, NetworkHookPlaceholders);
	m_networkFunctions[NetTypeMessageId::ID_CONNECTION_LOST] = std::bind(&ServerNetwork::NetConnectionLost, this, NetworkHookPlaceholders);
	m_networkFunctions[NetTypeMessageId::ID_CONNECTION_DISCONNECTED] = std::bind(&ServerNetwork::NetConnectionDisconnected, this, NetworkHookPlaceholders);
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
	for (unsigned int i = 0; i < m_receivePacketsAlive.size(); ++i)
		m_receivePacketsAlive[i] = false;

	for (unsigned int i = 0; i < m_receivePacketsThreads.size(); ++i)
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
		if (it->first == _exclude || !it->second->GetAccepted())
			continue;

		it->second->Send((char*)_packet->Data, _packet->Length);
	}

	SAFE_DELETE(_packet);
}

void ServerNetwork::Send(Packet* _packet, NetConnection _receiver)
{
	auto result = m_connectedClients.find(_receiver);

	if (result == m_connectedClients.end() || !result->second->GetAccepted()) // Could be a problem, check and make sure
	{
		if (NET_DEBUG)
			printf("Connection to receiver \"%s:%i\" was not found.\n", _receiver.IpAddress.c_str(), _receiver.Port);
		SAFE_DELETE(_packet);
		return;
	}

	m_connectedClients[_receiver]->Send((char*)_packet->Data, _packet->Length);

	SAFE_DELETE(_packet);
}

void ServerNetwork::ReceivePackets(ISocket* _socket, bool* _alive)
{
	while (*_alive)
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

		bool* b = new bool(true);

		m_receivePacketsAlive.push_back(b);
		m_receivePacketsThreads.push_back(std::thread(&ServerNetwork::ReceivePackets, this, newConnection, b));
	}
}

void ServerNetwork::SetOnPlayerConnected(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnPlayerConnected.\n");

	m_onPlayerConnected = _function;
}

void ServerNetwork::SetOnPlayerDisconnected(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnPlayerDisconnected.\n");

	m_onPlayerDisconnected = _function;
}

void ServerNetwork::SetOnPlayerTimedOut(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnPlayerTimedOut.\n");

	m_onPlayerTimedOut = _function;
}