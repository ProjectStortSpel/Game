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
		if (NET_DEBUG)
			printf("Player accepted. IP: %s:%d\n", _connection.IpAddress.c_str(), _connection.Port);

		uint64_t id2 = _packetHandler->StartPack(NetTypeMessageId::ID_CONNECTION_ACCEPTED);
		auto newPacket = _packetHandler->EndPack(id2);
		m_connectedClientsLock.lock();
		m_connectedClients[_connection]->SetActive(2);
		m_connectedClientsLock.unlock();
		Send(newPacket, _connection);


		uint64_t id3 = _packetHandler->StartPack(NetTypeMessageId::ID_REMOTE_CONNECTION_ACCEPTED);
		_packetHandler->WriteString(id3, "Username_Temp");
		Packet* p = _packetHandler->EndPack(id3);

		Broadcast(p, _connection);

		if (m_onPlayerConnected)
			m_onPlayerConnected(_connection);
	}
	else
	{
		if (NET_DEBUG)
			printf("Player connected with invalid password. IP: %s:%d\n", _connection.IpAddress.c_str(), _connection.Port);

		uint64_t id2 = _packetHandler->StartPack(NetTypeMessageId::ID_PASSWORD_INVALID);
		auto newPacket = _packetHandler->EndPack(id2);
		Send(newPacket, _connection);
		m_connectedClientsLock.lock();
		m_connectedClients[_connection]->SetActive(0);
		m_connectedClientsLock.unlock();
		m_receivePacketsThreads[_connection].join();
	}
}

void ServerNetwork::NetConnectionLost(NetConnection _connection)
{
	if (NET_DEBUG)
		printf("Player timed out. IP: %s:%d\n", _connection.IpAddress.c_str(), _connection.Port);

	m_connectedClientsLock.lock();
	m_connectedClients[_connection]->SetActive(0);
	m_connectedClientsLock.unlock();

	uint64_t id = m_packetHandler.StartPack(NetTypeMessageId::ID_REMOTE_CONNECTION_LOST);
	m_packetHandler.WriteString(id, "Username_Temp"); // remove laterz
	Packet* p = m_packetHandler.EndPack(id);

	Broadcast(p, _connection);

	if (m_onPlayerTimedOut)
		m_onPlayerTimedOut(_connection);

	m_timeOutLock.lock();
	m_currentIntervallCounter.erase(_connection);
	m_currentTimeOutIntervall.erase(_connection);
	m_timeOutLock.unlock();

	m_receivePacketsThreads[_connection].join();
}

void ServerNetwork::NetConnectionDisconnected(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection)
{
	if (NET_DEBUG)
		printf("Player disconnected. IP: %s:%d\n", _connection.IpAddress.c_str(), _connection.Port);

	m_connectedClientsLock.lock();
	m_connectedClients[_connection]->SetActive(0);
	m_connectedClientsLock.unlock();

	uint64_t id = _packetHandler->StartPack(NetTypeMessageId::ID_REMOTE_CONNECTION_DISCONNECTED);
	_packetHandler->WriteString(id, "Username_Temp");
	Packet* p = _packetHandler->EndPack(id);

	Broadcast(p, _connection);

	if(m_onPlayerDisconnected)
		m_onPlayerDisconnected(_connection);

	m_timeOutLock.lock();	
	m_currentIntervallCounter.erase(_connection);
	m_currentTimeOutIntervall.erase(_connection);
	m_timeOutLock.unlock();


	m_receivePacketsThreads[_connection].join();
}

void ServerNetwork::NetPing(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection)
{
	if (NET_DEBUG)
		printf("Ping from: %s:%d\n", _connection.IpAddress.c_str(), _connection.Port);

	uint64_t id = _packetHandler->StartPack(ID_PONG);
	Packet* p = _packetHandler->EndPack(id);
	Send(p, _connection);
}

void ServerNetwork::NetPong(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection)
{
	if (NET_DEBUG)
		printf("Pong from: %s:%d\n", _connection.IpAddress.c_str(), _connection.Port);
}

ServerNetwork::ServerNetwork()
	: BaseNetwork()
{
	m_incomingPort = 6112;
	m_listenForConnectionsAlive = false;
	m_maxConnections = 8;

	m_listenSocket = 0;

	m_networkFunctions[NetTypeMessageId::ID_PASSWORD_ATTEMPT] = std::bind(&ServerNetwork::NetPasswordAttempt, this, NetworkHookPlaceholders);
	m_networkFunctions[NetTypeMessageId::ID_CONNECTION_DISCONNECTED] = std::bind(&ServerNetwork::NetConnectionDisconnected, this, NetworkHookPlaceholders);
	m_networkFunctions[NetTypeMessageId::ID_PING] = std::bind(&ServerNetwork::NetPing, this, NetworkHookPlaceholders);
	m_networkFunctions[NetTypeMessageId::ID_PONG] = std::bind(&ServerNetwork::NetPong, this, NetworkHookPlaceholders);
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
	uint64_t id = m_packetHandler.StartPack(NetTypeMessageId::ID_CONNECTION_DISCONNECTED);
	Packet* p = m_packetHandler.EndPack(id);
	Broadcast(p);

	m_connectedClientsLock.lock();
	for (auto it = m_connectedClients.begin(); it != m_connectedClients.end(); ++it)
	{
		it->second->SetActive(0);
	}
	m_connectedClientsLock.unlock();

	for (auto it = m_receivePacketsThreads.begin(); it != m_receivePacketsThreads.end(); ++it)
	{
		if(it->second.joinable())
			it->second.join();
	}

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

	m_currentIntervallCounter.clear();
	m_currentTimeOutIntervall.clear();

	return true;
}

void ServerNetwork::Broadcast(Packet* _packet, NetConnection _exclude)
{
	m_connectedClientsLock.lock();
	for (auto it = m_connectedClients.begin(); it != m_connectedClients.end(); it++)
	{
		if (it->first == _exclude || it->second->GetActive() != 2)
			continue;

		it->second->Send((char*)_packet->Data, _packet->Length);
	}
	m_connectedClientsLock.unlock();

	SAFE_DELETE(_packet);
}

void ServerNetwork::Send(Packet* _packet, NetConnection _receiver)
{
	m_connectedClientsLock.lock();
	auto result = m_connectedClients.find(_receiver);

	if (result == m_connectedClients.end() || result->second->GetActive() != 2) // Could be a problem, check and make sure
	{
		m_connectedClientsLock.unlock();

		if (NET_DEBUG)
			printf("Connection to receiver \"%s:%i\" was not found.\n", _receiver.IpAddress.c_str(), _receiver.Port);

		SAFE_DELETE(_packet);
		return;
	}
	
	result->second->Send((char*)_packet->Data, _packet->Length);
	m_connectedClientsLock.unlock();

	SAFE_DELETE(_packet);
}

void ServerNetwork::ReceivePackets(ISocket* _socket)
{
	while (_socket->GetActive() != 0)
	{
		int result = _socket->Receive(m_packetData, MAX_PACKET_SIZE);

		if (result > 0)
		{
			unsigned short packetSize = result;

			m_timeOutLock.lock();

			if (m_currentIntervallCounter.find(_socket->GetNetConnection()) != m_currentIntervallCounter.end())
				m_currentIntervallCounter[_socket->GetNetConnection()] = 0;

			if (m_currentTimeOutIntervall.find(_socket->GetNetConnection()) != m_currentTimeOutIntervall.end())
				m_currentTimeOutIntervall[_socket->GetNetConnection()] = 0.0f;

			m_timeOutLock.unlock();

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

	m_connectedClientsLock.lock();
	m_connectedClients.erase(_socket->GetNetConnection());
	m_connectedClientsLock.unlock();
	SAFE_DELETE(_socket);
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

		m_connectedClientsLock.lock();
		m_connectedClients[nc] = newConnection;
		size_t size = m_connectedClients.size();
		m_connectedClientsLock.unlock();

		if (size > m_maxConnections)
		{
			uint64_t id = m_packetHandler.StartPack(ID_SERVER_FULL);
			Packet* p = m_packetHandler.EndPack(id);

			newConnection->Send((char*)p->Data, p->Length);
			
			printf("%s:%d tried to connect, but the server is full.\n", nc.IpAddress.c_str(), nc.Port);

			SAFE_DELETE(newConnection);

			m_connectedClientsLock.lock();
			m_connectedClients.erase(nc);
			m_connectedClientsLock.unlock();

			continue;
		}


		if (NET_DEBUG)
			printf("New incoming connection from %s:%d\n", nc.IpAddress.c_str(), nc.Port);

		bool* b = new bool(true);

		m_receivePacketsThreads[nc] = std::thread(&ServerNetwork::ReceivePackets, this, newConnection);
		m_timeOutLock.lock();
		m_currentTimeOutIntervall[nc] = 0.0f;
		m_currentIntervallCounter[nc] = 0;
		m_timeOutLock.unlock();
	}
}

void ServerNetwork::UpdateTimeOut(float _dt)
{
	std::vector<NetConnection> trash;

	m_timeOutLock.lock();
	for (auto it = m_currentTimeOutIntervall.begin(); it != m_currentTimeOutIntervall.end(); ++it)
	{
		it->second += _dt;
		if (it->second >= m_maxTimeOutIntervall)
		{
			++m_currentIntervallCounter[it->first];
			it->second = 0;

			if (m_currentIntervallCounter[it->first] >= m_maxIntervallCounter)
			{
				m_currentIntervallCounter[it->first] = 0;
				//NetConnectionLost(it->first);

				trash.push_back(it->first);

				continue;
			}
			uint64_t id = m_packetHandler.StartPack(ID_PING);
			Packet* p = m_packetHandler.EndPack(id);

			Send(p, it->first);
		}
	}
	m_timeOutLock.unlock();

	for (int i = 0; i < trash.size(); ++i)
	{
		NetConnectionLost(trash[i]);
	}

	m_timeOutLock.lock();
	for (int i = 0; i < trash.size(); ++i)
	{
		m_currentIntervallCounter.erase(trash[i]);
		m_currentTimeOutIntervall.erase(trash[i]);
	}
	m_timeOutLock.unlock();

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