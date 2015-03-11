#include "ServerNetwork.h"
#include <sstream>
#include <algorithm>

#include "NetTypeMessageID.h"

#ifdef WIN32
#else
#include <sys/socket.h>
#endif

using namespace Network;

ServerNetwork::ServerNetwork()
	: BaseNetwork()
{
	m_running = new bool(false);
	m_maxConnections = new unsigned int(64);
	m_listenSocket = 0;

	m_connectedClients		= new std::map<NetConnection, ISocket*>();
	m_receivePacketThreads		= new std::map < NetConnection, std::thread >();
	m_currentTimeOutIntervall	= new std::map < NetConnection, float >();
	m_currentIntervallCounter	= new std::map < NetConnection, int >();
	m_connectedClientsNC		= new std::vector<NetConnection>();

	m_connectedClientsLock		= SDL_CreateMutex();
	m_dataSentLock			= SDL_CreateMutex();
	m_dataReceiveLock		= SDL_CreateMutex();
	m_timeOutLock			= SDL_CreateMutex();

	m_onPlayerConnected = new std::vector<NetEvent>();
	m_onPlayerDisconnected = new std::vector<NetEvent>();
	m_onPlayerTimedOut = new std::vector<NetEvent>();
	m_onServerShutdown = new std::vector<NetEvent>();

	m_listenThread = new std::thread();

	NetMessageHook hook = std::bind(&ServerNetwork::NetPasswordAttempt, this, NetworkHookPlaceholders);
	(*m_systemFunctions)[NetTypeMessageId::ID_PASSWORD_ATTEMPT] = hook;

	hook = std::bind(&ServerNetwork::NetConnectionDisconnected, this, NetworkHookPlaceholders);
	(*m_systemFunctions)[NetTypeMessageId::ID_CONNECTION_DISCONNECTED] = hook;

	hook = std::bind(&ServerNetwork::NetPing, this, NetworkHookPlaceholders);
	(*m_systemFunctions)[NetTypeMessageId::ID_PING] = hook;

	hook = std::bind(&ServerNetwork::NetPong, this, NetworkHookPlaceholders);
	(*m_systemFunctions)[NetTypeMessageId::ID_PONG] = hook;

}

ServerNetwork::~ServerNetwork()
{
	Stop();

	SAFE_DELETE(m_running);
	SAFE_DELETE(m_maxConnections);
	SAFE_DELETE(m_listenSocket);
	SAFE_DELETE(m_receivePacketThreads);
	SAFE_DELETE(m_currentTimeOutIntervall);
	SAFE_DELETE(m_currentIntervallCounter);
	SAFE_DELETE(m_connectedClientsNC);

	SAFE_DELETE(m_onPlayerConnected);
	SAFE_DELETE(m_onPlayerDisconnected);
	SAFE_DELETE(m_onPlayerTimedOut);
	SAFE_DELETE(m_onServerShutdown);

	SAFE_DELETE(m_listenThread);

	if (SDL_LockMutex(m_connectedClientsLock) == 0)
	{
		for (auto it = m_connectedClients->begin(); it != m_connectedClients->end(); ++it)
			SAFE_DELETE(it->second);

		SAFE_DELETE(m_connectedClients);
		SDL_UnlockMutex(m_connectedClientsLock);
	}
	else if (NET_DEBUG > 0)
		DebugLog("Failed to lock connectedClients lock. Error: %s.", LogSeverity::Error, SDL_GetError());
	SDL_DestroyMutex(m_connectedClientsLock);
	SDL_DestroyMutex(m_dataSentLock);
	SDL_DestroyMutex(m_dataReceiveLock);
	SDL_DestroyMutex(m_timeOutLock);




}

bool ServerNetwork::Start(unsigned int& _incomingPort, const char* _password, unsigned int& _maxConnections)
{
	*m_incomingPort = _incomingPort;
	*m_password = _password;
	*m_maxConnections = _maxConnections;

	return Start();
}

bool ServerNetwork::Start()
{
	if (*m_running)
		Stop();

	SAFE_DELETE(m_listenSocket);
	m_listenSocket = ISocket::CreateSocket();
	m_listenSocket->Bind(*m_incomingPort);
	m_listenSocket->SetNoDelay(true);
	m_listenSocket->SetNonBlocking(true);

	if (NET_DEBUG > 0)
		DebugLog("Starting server.", LogSeverity::Info);

	if (!m_listenSocket->Listen(128))
		return *m_running;

	m_listenSocket->SetActive(2);
	*m_listenThread = std::thread(&ServerNetwork::ListenForConnections, this);

	*m_running = true;
	return *m_running;
}

bool ServerNetwork::Stop()
{
	if (!*m_running)
	{
		if (NET_DEBUG > 0)
			DebugLog("Tried to stop a server which is not running.", LogSeverity::Warning);
		return true;
	}

	//uint64_t id = m_packetHandler->StartPack(NetTypeMessageId::ID_CONNECTION_DISCONNECTED);
	//Packet* p = m_packetHandler->EndPack(id);
	//Broadcast(p);

	if (SDL_LockMutex(m_connectedClientsLock) == 0)
	{
		for (auto it = m_connectedClients->begin(); it != m_connectedClients->end(); ++it)
			it->second->ShutdownSocket(2);

		SDL_UnlockMutex(m_connectedClientsLock);
	}
	else
		DebugLog("Failed to lock connectedClients. Error: %s.", LogSeverity::Error, SDL_GetError());

	NetSleep(10);

	for (auto it = m_receivePacketThreads->begin(); it != m_receivePacketThreads->end(); ++it)
	{
		if(it->second.joinable())
			it->second.join();
	}
	m_receivePacketThreads->clear();

	m_listenSocket->ShutdownSocket(2);
	m_listenSocket->SetActive(0);

	NetSleep(10);

	if(m_listenThread->joinable())
		m_listenThread->join();

	//for (auto it = m_connectedClients->begin(); it != m_connectedClients->end(); ++it)
	m_connectedClients->clear();

	m_currentIntervallCounter->clear();
	m_currentTimeOutIntervall->clear();

	NetConnection nc = NetConnection();
	TriggerEvent(m_onServerShutdown, nc, 0);

	*m_running = false;

	return !*m_running;
}

void ServerNetwork::Broadcast(Packet* _packet, const NetConnection& _exclude)
{
	int bytesSent = 0;

	if (SDL_LockMutex(m_connectedClientsLock) == 0)
	{

		for (auto it = m_connectedClients->begin(); it != m_connectedClients->end(); ++it)
		{
			if (it->first == _exclude || it->second->GetActive() != 2)
				continue;

			int size = it->second->Send((char*)_packet->Data, *_packet->Length);
			bytesSent += size;
		}

		SDL_UnlockMutex(m_connectedClientsLock);
	}
	else if (NET_DEBUG > 0)
		DebugLog("Failed to lock connectedClients. Error: %s.", LogSeverity::Error, SDL_GetError());


	if (bytesSent >= 0)
	{
		if (SDL_LockMutex(m_dataSentLock) == 0)
		{
			*m_totalDataSent += bytesSent;
			*m_currentDataSent += bytesSent;
			SDL_UnlockMutex(m_dataSentLock);
		}
		else if(NET_DEBUG > 0)
			DebugLog("Failed to lock sentData. Error: %s.", LogSeverity::Error, SDL_GetError());
	}

	SAFE_DELETE(_packet);
}

void ServerNetwork::Send(Packet* _packet, NetConnection& _receiver, bool _deletePacket)
{
	int bytesSent = 0;

	if (SDL_LockMutex(m_connectedClientsLock) == 0)
	{

		auto result = m_connectedClients->find(_receiver);

		if (result == m_connectedClients->end() || result->second->GetActive() < 1) // Could be a problem, check and make sure
		{
			if (NET_DEBUG > 0)
				DebugLog("Connection to receiver %s:%d was not found.", LogSeverity::Warning, _receiver.GetIpAddress(), _receiver.GetPort());
		}
		else
			bytesSent = result->second->Send((char*)_packet->Data, *_packet->Length);

		SDL_UnlockMutex(m_connectedClientsLock);
	}
	else if (NET_DEBUG > 0)
		DebugLog("Failed to lock connectedClients. Error: %s.", LogSeverity::Error, SDL_GetError());


	if (bytesSent >= 0)
	{
		if (SDL_LockMutex(m_dataSentLock) == 0)
		{
			*m_totalDataSent += bytesSent;
			*m_currentDataSent += bytesSent;
			SDL_UnlockMutex(m_dataSentLock);
		}
		else if (NET_DEBUG > 0)
			DebugLog("Failed to lock sentData. Error: %s.", LogSeverity::Error, SDL_GetError());
	}

	if (_deletePacket)
		SAFE_DELETE(_packet);
}

void ServerNetwork::Send(Packet* _packet, NetConnection& _receiver)
{
	Send(_packet, _receiver, true);
}

void ServerNetwork::Send(Packet* _packet, std::vector<NetConnection>& _receivers)
{
	for (int i = 0; i < _receivers.size(); ++i)
	{
		Send(_packet, _receivers[i], false);
	}
	SAFE_DELETE(_packet);
}

void ServerNetwork::Send(Packet* _packet, ISocket* _socket)
{
	int bytesSent = 0;

	if (!_socket)
		return;

	if(_socket->GetActive() > 0)
		bytesSent = _socket->Send((char*)_packet->Data, *_packet->Length);

	if (bytesSent >= 0)
	{
		if (SDL_LockMutex(m_dataSentLock) == 0)
		{
			*m_totalDataSent += bytesSent;
			*m_currentDataSent += bytesSent;
			SDL_UnlockMutex(m_dataSentLock);
		}
		else if (NET_DEBUG > 0)
			DebugLog("Failed to lock sentData. Error: %s.", LogSeverity::Error, SDL_GetError());
	}

	SAFE_DELETE(_packet);
}

void ServerNetwork::ReceivePackets(ISocket* _socket, const std::string _name)
{
	char* packetData = new char[MAX_PACKET_SIZE];
	short nextPacketSize;
	short dataReceived;

	while (_socket->GetActive() != 0)
	{
		dataReceived = _socket->Receive(packetData, MAX_PACKET_SIZE);

		if (dataReceived > 0)
		{

			if (SDL_LockMutex(m_timeOutLock) == 0)
			{
				if (m_currentIntervallCounter->find(_socket->GetNetConnection()) != m_currentIntervallCounter->end())
					(*m_currentIntervallCounter)[_socket->GetNetConnection()] = 0;
				if (m_currentTimeOutIntervall->find(_socket->GetNetConnection()) != m_currentTimeOutIntervall->end())
					(*m_currentTimeOutIntervall)[_socket->GetNetConnection()] = 0.f;

				SDL_UnlockMutex(m_timeOutLock);
			}
			else if (NET_DEBUG > 0)
				DebugLog("Failed to lock timeout. Error: %s.", LogSeverity::Error, SDL_GetError());

			Packet* p = new Packet();
			p->Data = new unsigned char[dataReceived];
			*p->Length = dataReceived;
			*p->Sender = _socket->GetNetConnection();
			memcpy(p->Data, packetData, dataReceived);

			if (_socket->GetActive() == 1 && m_packetHandler->GetNetTypeMessageId(p) == NetTypeMessageId::ID_CUSTOM_PACKET)
			{
				if (SDL_LockMutex(m_customPacketLock) == 0)
				{
					m_inactivePackets->push(p);
					SDL_UnlockMutex(m_customPacketLock);
				}
			}
			else
				HandlePacket(p);

			if (SDL_LockMutex(m_dataReceiveLock) == 0)
			{
				*m_totalDataReceived	+= dataReceived;
				*m_currentDataReceived	+= dataReceived;
				SDL_UnlockMutex(m_dataReceiveLock);
			}
			else if (NET_DEBUG > 0)
				DebugLog("Failed to lock datareceive. Error: %s.", LogSeverity::Error, SDL_GetError());


		}
		else if (dataReceived == 0)
		{
			_socket->SetActive(0);
		}
		else
		{
			if (NET_DEBUG > 0)
				DebugLog("Failed to receive message from client.", LogSeverity::Warning);

			_socket->SetActive(0);
		}

	}

	if (SDL_LockMutex(m_connectedClientsLock) == 0)
	{
		m_connectedClients->erase(_socket->GetNetConnection());
		m_connectedClientsNC->erase(std::remove(m_connectedClientsNC->begin(), m_connectedClientsNC->end(), _socket->GetNetConnection()), m_connectedClientsNC->end());

		SAFE_DELETE(_socket);
		SDL_UnlockMutex(m_connectedClientsLock);
	}
	else if (NET_DEBUG > 0)
		DebugLog("Failed to lock connectedClients. Error: %s.", LogSeverity::Error, SDL_GetError());

	delete packetData;
}

void ServerNetwork::ListenForConnections(void)
{
	if (NET_DEBUG > 0)
		DebugLog("Started listen for incoming connections.", LogSeverity::Info);

	while (m_listenSocket->GetActive() != 0)
	{
		NetSleep(50); // 50 ?
		
		if (!m_listenSocket)
			m_listenSocket->SetActive(0);

		ISocket* newConnection = m_listenSocket->Accept();
		if (!newConnection)
			continue;

		newConnection->SetActive(1);
		newConnection->SetNonBlocking(false);
		newConnection->SetNoDelay(true);

		NetConnection nc = newConnection->GetNetConnection();
		unsigned int noConnections = *m_maxConnections + 1;

		if (SDL_LockMutex(m_connectedClientsLock) == 0)
		{
			(*m_connectedClients)[nc] = newConnection;
			noConnections = m_connectedClients->size();
			SDL_UnlockMutex(m_connectedClientsLock);
		}
		else if (NET_DEBUG > 0)
			DebugLog("Failed to lock connectedClients. Error: %s", LogSeverity::Error, SDL_GetError());

		if (noConnections > *m_maxConnections)
		{
			uint64_t id = m_packetHandler->StartPack(ID_SERVER_FULL);
			Packet* p = m_packetHandler->EndPack(id);

			Send(p, newConnection);
			SAFE_DELETE(newConnection);

			if (NET_DEBUG > 0)
				DebugLog("New incoming connection from %s:%d. But the server is full.", LogSeverity::Info, nc.GetIpAddress(), nc.GetPort());

			continue;
		}

		if (NET_DEBUG > 0)
			DebugLog("New incoming connection from %s:%d.", LogSeverity::Info, nc.GetIpAddress(), nc.GetPort());

		(*m_receivePacketThreads)[nc] = std::thread(&ServerNetwork::ReceivePackets, this, newConnection, nc.GetIpAddress());

		if (SDL_LockMutex(m_timeOutLock) == 0)
		{
			(*m_currentTimeOutIntervall)[nc] = 0.f;
			(*m_currentIntervallCounter)[nc] = 0;
			SDL_UnlockMutex(m_timeOutLock);
		}
		else if (NET_DEBUG > 0)
			DebugLog("Failed to lock timeout. Error: %s.", LogSeverity::Error);

	}

	SAFE_DELETE(m_listenSocket);

}

void ServerNetwork::UpdateNetUsage(float& _dt)
{
	*m_usageDataTimer += _dt;

	if (*m_usageDataTimer >= 1.f)
	{
		*m_usageDataTimer = 0;

		if (SDL_LockMutex(m_dataReceiveLock) == 0)
		{
			*m_currentDataReceived = 0;
			SDL_UnlockMutex(m_dataReceiveLock);
		}
		else if (NET_DEBUG > 0)
			DebugLog("Failed to lock datareceive. Error: %s.", LogSeverity::Error, SDL_GetError());


		if (SDL_LockMutex(m_dataSentLock) == 0)
		{
			*m_currentDataSent = 0;
			SDL_UnlockMutex(m_dataSentLock);
		}
		else if (NET_DEBUG > 0)
			DebugLog("Failed to lock dataSent. Error: %s.", LogSeverity::Error, SDL_GetError());
	}


}
void ServerNetwork::UpdateTimeOut(float& _dt)
{
	std::vector<NetConnection> trash;

	if (SDL_LockMutex(m_timeOutLock) == 0)
	{

		for (auto it = m_currentTimeOutIntervall->begin(); it != m_currentTimeOutIntervall->end(); ++it)
		{
			it->second += _dt;
			if (it->second >= *m_maxTimeOutIntervall)
			{
				++(*m_currentIntervallCounter)[it->first];
				it->second = 0;


				if ((*m_currentIntervallCounter)[it->first] >= *m_maxIntervallCounter)
				{
					(*m_currentIntervallCounter)[it->first] = 0;

					trash.push_back(it->first);

					continue;
				}

				uint64_t id = m_packetHandler->StartPack(ID_PING);
				Packet* p = m_packetHandler->EndPack(id);

				NetConnection nc = it->first;
				Send(p, nc);
			}
		}



		SDL_UnlockMutex(m_timeOutLock);
	}
	else if (NET_DEBUG > 0)
		DebugLog("Failed to lock timeout. Error: %s.", LogSeverity::Error, SDL_GetError());


	for (unsigned int i = 0; i < trash.size(); ++i)
		NetConnectionLost(trash[i]);

	if (SDL_LockMutex(m_timeOutLock) == 0)
	{
		for (unsigned int i = 0; i < trash.size(); ++i)
		{
			m_currentIntervallCounter->erase(trash[i]);
			m_currentIntervallCounter->erase(trash[i]);
		}

		SDL_UnlockMutex(m_timeOutLock);
	}
	else if (NET_DEBUG > 0)
		DebugLog("Failed to lock timeout. Error: %s.", LogSeverity::Error, SDL_GetError());


}

void ServerNetwork::ResetNetworkEvents()
{
	Update(0);
	m_onPlayerConnected->clear();
	m_onPlayerDisconnected->clear();
	m_onPlayerTimedOut->clear();
	m_onServerShutdown->clear();

	Clear();
}

void ServerNetwork::SetOnPlayerConnected(NetEvent& _function)
{
	if (NET_DEBUG > 0)
		DebugLog("Hooking function to OnPlayerConnected.", LogSeverity::Info);

	m_onPlayerConnected->push_back(_function);
}

void ServerNetwork::SetOnPlayerDisconnected(NetEvent& _function)
{
	if (NET_DEBUG > 0)
		DebugLog("Hooking function to OnPlayerDisconnected.", LogSeverity::Info);

	m_onPlayerDisconnected->push_back(_function);
}

void ServerNetwork::SetOnPlayerTimedOut(NetEvent& _function)
{
	if (NET_DEBUG > 0)
		DebugLog("Hooking function to OnPlayerTimedOut.", LogSeverity::Info);

	m_onPlayerTimedOut->push_back(_function);
}

void ServerNetwork::SetOnServerShutdown(NetEvent& _function)
{
	if (NET_DEBUG > 0)
		DebugLog("Hooking function to OnServerShutdown.", LogSeverity::Info);

	m_onServerShutdown->push_back(_function);
}


void ServerNetwork::NetPasswordAttempt(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	char type = _packetHandler->GetNetTypeMessageId(_id);
	std::string password = _packetHandler->ReadString(_id);

	if (m_password->compare(password) == 0)
	{
		if (NET_DEBUG > 0)
			DebugLog("Player from %s:%d accepted.", LogSeverity::Info, _connection.GetIpAddress(), _connection.GetPort());

		uint64_t id2 = _packetHandler->StartPack(NetTypeMessageId::ID_CONNECTION_ACCEPTED);
		Packet* newPacket = _packetHandler->EndPack(id2);
		Send(newPacket, _connection);

		if (SDL_LockMutex(m_connectedClientsLock) == 0)
		{
			if ((*m_connectedClients)[_connection])
			{
				(*m_connectedClients)[_connection]->SetActive(2);
				m_connectedClientsNC->push_back(_connection);
			}
			else
			{
				m_connectedClients->erase(_connection);
				if ((*m_receivePacketThreads)[_connection].joinable())
					(*m_receivePacketThreads)[_connection].join();
				return;
			}

			SDL_UnlockMutex(m_connectedClientsLock);
		}
		else if (NET_DEBUG > 0)
			DebugLog("Failed to lock connectedClients. Error: %s", LogSeverity::Error, SDL_GetError());


		HandleInactivePacket();

		uint64_t id3 = _packetHandler->StartPack(NetTypeMessageId::ID_REMOTE_CONNECTION_ACCEPTED);
		_packetHandler->WriteString(id3, _connection.GetIpAddress());
		Packet* broadcastPacket = _packetHandler->EndPack(id3);
		
		Broadcast(broadcastPacket, _connection);

		TriggerEvent(m_onPlayerConnected, _connection, 0);

	}
	else
	{
		if (NET_DEBUG > 0)
			DebugLog("Player from %s:%d tried to connect with invalid password.", LogSeverity::Warning, _connection.GetIpAddress(), _connection.GetPort());

		uint64_t id2 = _packetHandler->StartPack(NetTypeMessageId::ID_PASSWORD_INVALID);
		Packet* newPacket = _packetHandler->EndPack(id2);

		Send(newPacket, _connection);

		if (SDL_LockMutex(m_connectedClientsLock) == 0)
		{
			(*m_connectedClients)[_connection]->ShutdownSocket(1);
			if ((*m_receivePacketThreads)[_connection].joinable())
				(*m_receivePacketThreads)[_connection].join();
			SDL_UnlockMutex(m_connectedClientsLock);
		}
		else if (NET_DEBUG > 0)
			DebugLog("Failed to lock connectedClients.", LogSeverity::Error);

	}

}

void ServerNetwork::NetConnectionLost(NetConnection& _connection)
{
	if (NET_DEBUG > 0)
		DebugLog("Player %s:%d timed out from server.", LogSeverity::Info, _connection.GetIpAddress(), _connection.GetPort());

	if (SDL_LockMutex(m_connectedClientsLock) == 0)
	{
		if(m_connectedClients->find(_connection) != m_connectedClients->end())
			(*m_connectedClients)[_connection]->ShutdownSocket(1);
		SDL_UnlockMutex(m_connectedClientsLock);
	}
	else if (NET_DEBUG > 0)
		DebugLog("Failed to lock connectedClients. Error: %s.", LogSeverity::Error, SDL_GetError());

	uint64_t id = m_packetHandler->StartPack(NetTypeMessageId::ID_REMOTE_CONNECTION_LOST);
	m_packetHandler->WriteString(id, _connection.GetIpAddress());
	Packet* p = m_packetHandler->EndPack(id);

	Broadcast(p, _connection);
	TriggerEvent(m_onPlayerTimedOut, _connection, 0);

	if (SDL_LockMutex(m_timeOutLock) == 0)
	{
		m_currentIntervallCounter->erase(_connection);
		m_currentTimeOutIntervall->erase(_connection);

		SDL_UnlockMutex(m_timeOutLock);
	}
	else if (NET_DEBUG > 0)
		DebugLog("Failed to lock timeOut. Error: %s.", LogSeverity::Error, SDL_GetError());

	if((*m_receivePacketThreads)[_connection].joinable())
		(*m_receivePacketThreads)[_connection].join();
	(*m_receivePacketThreads).erase(_connection);
}

void ServerNetwork::NetConnectionDisconnected(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	if (NET_DEBUG > 0)
		DebugLog("Player %s:%d disconnected from server.", LogSeverity::Info, _connection.GetIpAddress(), _connection.GetPort());

	if (SDL_LockMutex(m_connectedClientsLock) == 0)
	{
		if (m_connectedClients->find(_connection) != m_connectedClients->end())
			(*m_connectedClients)[_connection]->ShutdownSocket(1);
		SDL_UnlockMutex(m_connectedClientsLock);
	}
	else if (NET_DEBUG > 0)
		DebugLog("Failed to lock connectedClients. Error: %s.", LogSeverity::Error, SDL_GetError());

	uint64_t id = m_packetHandler->StartPack(NetTypeMessageId::ID_REMOTE_CONNECTION_DISCONNECTED);
	m_packetHandler->WriteString(id, _connection.GetIpAddress());
	Packet* p = m_packetHandler->EndPack(id);

	Broadcast(p, _connection);
	TriggerEvent(m_onPlayerDisconnected, _connection, 0);

	if (SDL_LockMutex(m_timeOutLock) == 0)
	{
		m_currentIntervallCounter->erase(_connection);
		m_currentTimeOutIntervall->erase(_connection);

		SDL_UnlockMutex(m_timeOutLock);
	}
	else if (NET_DEBUG > 0)
		DebugLog("Failed to lock timeOut. Error: %s.", LogSeverity::Error, SDL_GetError());

	if ((*m_receivePacketThreads)[_connection].joinable())
		(*m_receivePacketThreads)[_connection].join();
	(*m_receivePacketThreads).erase(_connection);
}

void ServerNetwork::NetPing(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	if (NET_DEBUG == 2)
		DebugLog("Received ping from: %s:%d", LogSeverity::Info, _connection.GetIpAddress(), _connection.GetPort());

	uint64_t id = _packetHandler->StartPack(ID_PONG);
	Packet* p = _packetHandler->EndPack(id);
	Send(p, _connection);
}

void ServerNetwork::NetPong(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	if (NET_DEBUG == 2)
		DebugLog("Received pong from: %s:%d", LogSeverity::Info, _connection.GetIpAddress(), _connection.GetPort());
}