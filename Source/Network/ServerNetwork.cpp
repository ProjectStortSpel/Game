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
	m_listenSocket = ISocket::CreateSocket();

	m_connectedClients			= new std::map<NetConnection, ISocket*>();
	m_receivePacketThreads		= new std::map < NetConnection, std::thread >();
	m_currentTimeOutIntervall	= new std::map < NetConnection, float >();
	m_currentIntervallCounter	= new std::map < NetConnection, int >();
	m_connectedClientsNC		= new std::vector<NetConnection>();

	m_connectedClientsLock	= SDL_CreateMutex();
	m_dataSentLock			= SDL_CreateMutex();
	m_dataReceiveLock = SDL_CreateMutex();
	m_timeOutLock			= SDL_CreateMutex();
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
	m_listenSocket->Bind(*m_incomingPort);
	m_listenSocket->SetNoDelay(true);

	if (NET_DEBUG > 0)
		DebugLog("Starting server.", LogSeverity::Info);

	if (!m_listenSocket->Listen(128))
		return *m_running;

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

	uint64_t id = m_packetHandler->StartPack(NetTypeMessageId::ID_CONNECTION_DISCONNECTED);
	Packet* p = m_packetHandler->EndPack(id);
	Broadcast(p);

	NetSleep(10);

	if (SDL_LockMutex(m_connectedClientsLock) == 0)
	{
		for (auto it = m_connectedClients->begin(); it != m_connectedClients->end(); ++it)
			it->second->ShutdownSocket(1);

		SDL_UnlockMutex(m_connectedClientsLock);
	}
	else
		DebugLog("Failed to lock connectedClients. Error: %s.", LogSeverity::Error, SDL_GetError());

	for (auto it = m_receivePacketThreads->begin(); it != m_receivePacketThreads->end(); ++it)
		it->second.join();
	m_receivePacketThreads->clear();

	m_listenSocket->ShutdownSocket(2);
	m_listenThread->join();
	SAFE_DELETE(m_listenSocket);

	for (auto it = m_connectedClients->begin(); it != m_connectedClients->end(); ++it)
		SAFE_DELETE(m_connectedClients);
	m_connectedClients->clear();

	m_currentIntervallCounter->clear();
	m_currentTimeOutIntervall->clear();

	TriggerEvent(m_onServerShutdown, NetConnection(), 0);

	*m_running = false;

	return !*m_running;
}

void ServerNetwork::Broadcast(Packet* _packet, const NetConnection& _exclude)
{
	float bytesSent = 0;

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

void ServerNetwork::Send(Packet* _packet, NetConnection& _receiver)
{
	float bytesSent = 0;

	if (SDL_LockMutex(m_connectedClientsLock) == 0)
	{

		auto result = m_connectedClients->find(_receiver);

		if (result == m_connectedClients->end() || result->second->GetActive() > 1) // Could be a problem, check and make sure
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

	SAFE_DELETE(_packet);
}

void ServerNetwork::ReceivePackets(ISocket* _socket)
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