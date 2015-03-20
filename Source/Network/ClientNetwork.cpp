#include "ClientNetwork.h"

#include "NetTypeMessageID.h"

#ifdef WIN32
#else
#include <sys/socket.h>
#endif

using namespace Network;

ClientNetwork::ClientNetwork()
	: BaseNetwork()
{

	m_outgoingPort = new int(6112);
	m_remoteAddress = new std::string("127.0.0.1");
	m_connected = new bool(false);

	m_ping = new float(0);
	m_sendTime = new float(0);
	m_receiveTime = new float(0);

	m_onConnectedToServer = new std::vector<NetEvent>();
	m_onDisconnectedFromServer = new std::vector<NetEvent>();
	m_onTimedOutFromServer = new std::vector<NetEvent>();
	m_onFailedToConnect = new std::vector<NetEvent>();
	m_onPasswordInvalid = new std::vector<NetEvent>();
	m_onKickedFromServer = new std::vector<NetEvent>();
	m_onBannedFromServer = new std::vector<NetEvent>();
	m_onServerFull = new std::vector<NetEvent>();

	m_onRemotePlayerConnected = new std::vector<NetEvent>();
	m_onRemotePlayerDisconnected = new std::vector<NetEvent>();
	m_onRemotePlayerTimedOut = new std::vector<NetEvent>();
	m_onRemotePlayerKicked = new std::vector<NetEvent>();
	m_onRemotePlayerBanned = new std::vector<NetEvent>();

	m_currentTimeOutIntervall = new float(0);
	m_currentIntervallCounter = new int(0);

	m_receiveThread = new std::thread();
	m_socket = 0;

	(*m_systemFunctions)[NetTypeMessageId::ID_PASSWORD_INVALID] = std::bind(&ClientNetwork::NetPasswordInvalid, this, NetworkHookPlaceholders);
	(*m_systemFunctions)[NetTypeMessageId::ID_CONNECTION_ACCEPTED] = std::bind(&ClientNetwork::NetConnectionAccepted, this, NetworkHookPlaceholders);
	(*m_systemFunctions)[NetTypeMessageId::ID_SERVER_FULL] = std::bind(&ClientNetwork::NetConnectionServerFull, this, NetworkHookPlaceholders);

	(*m_systemFunctions)[NetTypeMessageId::ID_CONNECTION_DISCONNECTED] = std::bind(&ClientNetwork::NetConnectionDisconnected, this, NetworkHookPlaceholders);

	(*m_systemFunctions)[NetTypeMessageId::ID_CONNECTION_KICKED] = std::bind(&ClientNetwork::NetConnectionKicked, this, NetworkHookPlaceholders);
	(*m_systemFunctions)[NetTypeMessageId::ID_CONNECTION_BANNED] = std::bind(&ClientNetwork::NetConnectionBanned, this, NetworkHookPlaceholders);

	(*m_systemFunctions)[NetTypeMessageId::ID_PING] = std::bind(&ClientNetwork::NetPing, this, NetworkHookPlaceholders);
	(*m_systemFunctions)[NetTypeMessageId::ID_PONG] = std::bind(&ClientNetwork::NetPong, this, NetworkHookPlaceholders);

	(*m_systemFunctions)[NetTypeMessageId::ID_REMOTE_CONNECTION_ACCEPTED] = std::bind(&ClientNetwork::NetRemoteConnectionAccepted, this, NetworkHookPlaceholders);

	(*m_systemFunctions)[NetTypeMessageId::ID_REMOTE_CONNECTION_LOST] = std::bind(&ClientNetwork::NetRemoteConnectionLost, this, NetworkHookPlaceholders);
	(*m_systemFunctions)[NetTypeMessageId::ID_REMOTE_CONNECTION_DISCONNECTED] = std::bind(&ClientNetwork::NetRemoteConnectionDisconnected, this, NetworkHookPlaceholders);

	(*m_systemFunctions)[NetTypeMessageId::ID_REMOTE_CONNECTION_KICKED] = std::bind(&ClientNetwork::NetRemoteConnectionKicked, this, NetworkHookPlaceholders);
	(*m_systemFunctions)[NetTypeMessageId::ID_REMOTE_CONNECTION_BANNED] = std::bind(&ClientNetwork::NetRemoteConnectionBanned, this, NetworkHookPlaceholders);

	memset(m_packetData, 0, sizeof(m_packetData));



}

ClientNetwork::~ClientNetwork()
{
	Disconnect();

	SAFE_DELETE(m_outgoingPort);
	SAFE_DELETE(m_remoteAddress);
	SAFE_DELETE(m_connected);

	SAFE_DELETE(m_ping);
	SAFE_DELETE(m_sendTime);
	SAFE_DELETE(m_receiveTime);

	SAFE_DELETE(m_onConnectedToServer);
	SAFE_DELETE(m_onDisconnectedFromServer);
	SAFE_DELETE(m_onTimedOutFromServer);
	SAFE_DELETE(m_onFailedToConnect);
	SAFE_DELETE(m_onPasswordInvalid);
	SAFE_DELETE(m_onKickedFromServer);
	SAFE_DELETE(m_onBannedFromServer);
	SAFE_DELETE(m_onServerFull);

	SAFE_DELETE(m_onRemotePlayerConnected);
	SAFE_DELETE(m_onRemotePlayerDisconnected);
	SAFE_DELETE(m_onRemotePlayerTimedOut);
	SAFE_DELETE(m_onRemotePlayerKicked);
	SAFE_DELETE(m_onRemotePlayerBanned);

	SAFE_DELETE(m_currentTimeOutIntervall);
	SAFE_DELETE(m_currentIntervallCounter);
	
	SAFE_DELETE(m_receiveThread);
}

bool ClientNetwork::Connect(const char* _ipAddress, const char* _password, const int& _outgoing, const int& _incomingPort)
{
	*m_remoteAddress = _ipAddress;
	*m_password = _password;
	*m_outgoingPort = _outgoing;
	*m_incomingPort = _incomingPort;

	return Connect();
}


bool ClientNetwork::Connect()
{
	if (NET_DEBUG > 0)
		DebugLog("Client connecting to server: %s:%d", LogSeverity::Info, m_remoteAddress->c_str(), *m_outgoingPort);

	if (*m_connected)
		Disconnect();

	SAFE_DELETE(m_socket);
	m_socket = ISocket::CreateSocket();
	m_socket->Bind(*m_incomingPort);

	*m_connected = m_socket->Connect(m_remoteAddress->c_str(), *m_outgoingPort);

	if (!*m_connected)
	{
		NetConnection nc = NetConnection(m_remoteAddress->c_str(), *m_outgoingPort);
		TriggerEvent(m_onFailedToConnect, nc, 0);
		return false;
	}

	*m_connected = true;
	m_socket->SetActive(1);

	uint64_t id = m_packetHandler->StartPack(NetTypeMessageId::ID_PASSWORD_ATTEMPT);
	m_packetHandler->WriteString(id, m_password->c_str());
	Packet* p = m_packetHandler->EndPack(id);

	Send(p);

	*m_receiveThread = std::thread(&ClientNetwork::ReceivePackets, this, m_remoteAddress->c_str());

	return true;
}

void ClientNetwork::Disconnect()
{
	if (!*m_connected)
	{
		DebugLog("Tried to disconnect while not connected.", LogSeverity::Warning);
		return;
	}

	uint64_t id = m_packetHandler->StartPack(ID_CONNECTION_DISCONNECTED);
	Packet* packet = m_packetHandler->EndPack(id);
	Send(packet);

	m_socket->ShutdownSocket(1);

	NetSleep(10);

	if(m_receiveThread->joinable())
		m_receiveThread->join();

	*m_connected = false;

	NetConnection nc = m_socket->GetNetConnection();
	TriggerEvent(m_onDisconnectedFromServer, nc, 0);

	SAFE_DELETE(m_socket);
}

void ClientNetwork::ReceivePackets(const std::string _name)
{
	short dataReceived;
	bool threadRunning = true;

	while (m_socket->GetActive() > 0)
	{
		dataReceived = m_socket->Receive(m_packetData, MAX_PACKET_SIZE);

		if (dataReceived > 0)
		{
			*m_currentIntervallCounter = 0;
			*m_currentTimeOutIntervall = 0.0f;

			Packet* p = new Packet();
			p->Data = new unsigned char[dataReceived];
			*p->Length = dataReceived;
			*p->Sender = m_socket->GetNetConnection();
			memcpy(p->Data, m_packetData, dataReceived);

			HandlePacket(p);

			*m_totalDataReceived += dataReceived;
			*m_currentDataReceived += dataReceived;

		}
		else if (dataReceived == 0)
		{
			m_socket->SetActive(0);
		}
		else
		{
			m_socket->SetActive(0);
		}
	}

	

}

void ClientNetwork::Send(Packet* _packet)
{
	if (!*m_connected)
	{
		if (NET_DEBUG > 0)
			DebugLog("Tried to send a message while not connected to server.", LogSeverity::Warning);

		SAFE_DELETE(_packet);
		return;
	}

	if (m_packetHandler->GetNetTypeMessageId(_packet) == NetTypeMessageId::ID_CUSTOM_PACKET && m_socket->GetActive() != 2)
	{
		m_inactivePackets->push(_packet);
		return;
	}


	int bytesSent = m_socket->Send((char*)_packet->Data, *_packet->Length);

	if (bytesSent > 0)
	{
		if (m_packetHandler->GetNetTypeMessageId(_packet) == ID_PING)
			*m_sendTime = GetMillisecondsTime();

		*m_totalDataSent += bytesSent;
		*m_currentDataSent += bytesSent;
	}
	else if (bytesSent == 0)
	{
	}
	else
	{
		if (NET_DEBUG > 0)
			DebugLog("Failed to send message to server.", LogSeverity::Warning);
	}

	SAFE_DELETE(_packet);

}

void ClientNetwork::UpdateNetUsage(float& _dt)
{
	*m_usageDataTimer += _dt;
	if (*m_usageDataTimer >= 1.f)
	{
		*m_usageDataTimer = 0;
		*m_currentDataReceived = 0;
		*m_currentDataSent = 0;
	}
}

void ClientNetwork::UpdateTimeOut(float& _dt)
{
	if (!m_socket)
		return;

	*m_currentTimeOutIntervall += _dt;

	if (*m_currentTimeOutIntervall >= *m_maxTimeOutIntervall)
	{
		++*m_currentIntervallCounter;
		*m_currentTimeOutIntervall = 0;

		if (*m_currentIntervallCounter >= *m_maxIntervallCounter)
		{
			*m_currentIntervallCounter = 0;
			NetConnection nc = m_socket->GetNetConnection();
			NetConnectionLost(nc);
			return;
		}
		uint64_t id = m_packetHandler->StartPack(ID_PING);
		Packet* p = m_packetHandler->EndPack(id);

		Send(p);
	}
}

void ClientNetwork::ResetNetworkEvents()
{
	Update(0);
	m_onConnectedToServer->clear();
	m_onDisconnectedFromServer->clear();
	m_onTimedOutFromServer->clear();
	m_onFailedToConnect->clear();
	m_onPasswordInvalid->clear();
	m_onKickedFromServer->clear();
	m_onBannedFromServer->clear();
	m_onServerFull->clear();
	m_onRemotePlayerConnected->clear();
	m_onRemotePlayerDisconnected->clear();
	m_onRemotePlayerTimedOut->clear();
	m_onRemotePlayerKicked->clear();
	m_onRemotePlayerBanned->clear();

	Clear();
}

void ClientNetwork::NetPasswordInvalid(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	if (NET_DEBUG > 0)
		DebugLog("Invalid password. Connection refused to %s:%d.", LogSeverity::Info, _connection.GetIpAddress(), _connection.GetPort());

	m_socket->ShutdownSocket(1);
	//m_socket->SetActive(0);

	TriggerEvent(m_onPasswordInvalid, _connection, 0);

}

void ClientNetwork::NetConnectionAccepted(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	if (NET_DEBUG > 0)
		DebugLog("Password accepted. Connected to %s:%d.", LogSeverity::Info, _connection.GetIpAddress(), _connection.GetPort());

	m_socket->SetActive(2);

	for (int i = 0; i < m_inactivePackets->size(); ++i)
	{
		Send(m_inactivePackets->front());
		m_inactivePackets->pop();
	}

	TriggerEvent(m_onConnectedToServer, _connection, 0);

}

void ClientNetwork::NetConnectionServerFull(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	if (NET_DEBUG > 0)
		DebugLog("Server full. Unable to connect to %s:%d.", LogSeverity::Info, _connection.GetIpAddress(), _connection.GetPort());

	m_socket->ShutdownSocket(1);
	//m_socket->SetActive(0);

	TriggerEvent(m_onServerFull, _connection, 0);

}

void ClientNetwork::NetConnectionLost(NetConnection& _connection)
{
	if (NET_DEBUG > 0)
		DebugLog("Connection lost. Disconnect from %s:%d.", LogSeverity::Info, _connection.GetIpAddress(), _connection.GetPort());

	m_socket->ShutdownSocket(1);

	NetSleep(10);

	if (m_receiveThread->joinable())
		m_receiveThread->join();

	*m_connected = false;
	TriggerEvent(m_onTimedOutFromServer, _connection, 0);

	SAFE_DELETE(m_socket);
}

void ClientNetwork::NetConnectionDisconnected(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	if (NET_DEBUG > 0)
		DebugLog("Disconnected from %s:%d.", LogSeverity::Info, _connection.GetIpAddress(), _connection.GetPort());

	m_socket->ShutdownSocket(1);
	//m_socket->SetActive(0);

	TriggerEvent(m_onDisconnectedFromServer, _connection, 0);
}

void ClientNetwork::NetConnectionKicked(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	if (NET_DEBUG > 0)
		DebugLog("Kicked from %s:%d. Disconnected", LogSeverity::Info, _connection.GetIpAddress(), _connection.GetPort());

	m_socket->ShutdownSocket(1);
	//m_socket->SetActive(0);

	TriggerEvent(m_onKickedFromServer, _connection, 0);
}

void ClientNetwork::NetConnectionBanned(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	if (NET_DEBUG > 0)
		DebugLog("Banned from %s:%d. Disconnected", LogSeverity::Info, _connection.GetIpAddress(), _connection.GetPort());

	m_socket->ShutdownSocket(1);
	//m_socket->SetActive(0);

	TriggerEvent(m_onBannedFromServer, _connection, 0);
}


void ClientNetwork::NetPing(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	if (NET_DEBUG == 2)
		DebugLog("Received ping from %s:%d.", LogSeverity::Info, _connection.GetIpAddress(), _connection.GetPort());

	uint64_t id = _packetHandler->StartPack(ID_PONG);
	Packet* p = _packetHandler->EndPack(id);
	Send(p);

}

void ClientNetwork::NetPong(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	if (NET_DEBUG == 2)
		DebugLog("Received pong from %s:%d.", LogSeverity::Info, _connection.GetIpAddress(), _connection.GetPort());

	*m_receiveTime = GetMillisecondsTime();
	*m_ping = *m_receiveTime - *m_sendTime;

}


void ClientNetwork::NetRemoteConnectionAccepted(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	char* name = _packetHandler->ReadString(_id);
	if (NET_DEBUG > 0)
		DebugLog("%s connected to the server.", LogSeverity::Info, name);

	TriggerEvent(m_onRemotePlayerConnected, _connection, 0);
}

void ClientNetwork::NetRemoteConnectionLost(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	char* name = _packetHandler->ReadString(_id);
	if (NET_DEBUG > 0)
		DebugLog("%s timed out from the server.", LogSeverity::Info, name);

	TriggerEvent(m_onRemotePlayerTimedOut, _connection, 0);
}

void ClientNetwork::NetRemoteConnectionDisconnected(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	char* name = _packetHandler->ReadString(_id);
	if (NET_DEBUG > 0)
		DebugLog("%s disconnected from the server.", LogSeverity::Info, name);

	TriggerEvent(m_onRemotePlayerDisconnected, _connection, 0);
}

void ClientNetwork::NetRemoteConnectionKicked(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	char* name = _packetHandler->ReadString(_id);

	if (NET_DEBUG > 0)
		DebugLog("%s was kicked from the server.", LogSeverity::Info, name);

	TriggerEvent(m_onRemotePlayerKicked, _connection, 0);
}

void ClientNetwork::NetRemoteConnectionBanned(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	char* name = _packetHandler->ReadString(_id);

	if (NET_DEBUG > 0)
		DebugLog("%s was banned from the server.", LogSeverity::Info, name);

	TriggerEvent(m_onRemotePlayerBanned, _connection, 0);
}


void ClientNetwork::SetOnConnectedToServer(NetEvent& _function)
{
	if(NET_DEBUG > 0)
		DebugLog("Hooking function to OnConnectedToServer.", LogSeverity::Info);

	m_onConnectedToServer->push_back(_function);
}

void ClientNetwork::SetOnDisconnectedFromServer(NetEvent& _function)
{
	if (NET_DEBUG > 0)
		DebugLog("Hooking function to OnDisconnectedFromServer.", LogSeverity::Info);

	m_onDisconnectedFromServer->push_back(_function);
}

void ClientNetwork::SetOnTimedOutFromServer(NetEvent& _function)
{
	if (NET_DEBUG > 0)
		DebugLog("Hooking function to OnTimedOutFromServer.", LogSeverity::Info);

	m_onTimedOutFromServer->push_back(_function);
}

void ClientNetwork::SetOnFailedToConnect(NetEvent& _function)
{
	if (NET_DEBUG > 0)
		DebugLog("Hooking function to OnFailedToConnect.", LogSeverity::Info);

	m_onFailedToConnect->push_back(_function);
}

void ClientNetwork::SetOnPasswordInvalid(NetEvent& _function)
{
	if (NET_DEBUG > 0)
		DebugLog("Hooking function to OnPasswordInvalid.", LogSeverity::Info);

	m_onPasswordInvalid->push_back(_function);
}

void ClientNetwork::SetOnKickedFromServer(NetEvent& _function)
{
	if (NET_DEBUG > 0)
		DebugLog("Hooking function to OnKickedFromServer.", LogSeverity::Info);

	m_onKickedFromServer->push_back(_function);
}

void ClientNetwork::SetOnBannedFromServer(NetEvent& _function)
{
	if (NET_DEBUG > 0)
		DebugLog("Hooking function to OnBannedFromServer.", LogSeverity::Info);

	m_onBannedFromServer->push_back(_function);
}

void ClientNetwork::SetOnServerFull(NetEvent& _function)
{
	if (NET_DEBUG > 0)
		DebugLog("Hooking function to OnServerFull.", LogSeverity::Info);

	m_onServerFull->push_back(_function);
}

void ClientNetwork::SetOnRemotePlayerConnected(NetEvent& _function)
{
	if (NET_DEBUG > 0)
		DebugLog("Hooking function to OnRemotePlayerConnected.", LogSeverity::Info);

	m_onRemotePlayerConnected->push_back(_function);
}

void ClientNetwork::SetOnRemotePlayerDisconnected(NetEvent& _function)
{
	if (NET_DEBUG > 0)
		DebugLog("Hooking function to OnRemotePlayerDisconnected.", LogSeverity::Info);

	m_onRemotePlayerDisconnected->push_back(_function);
}

void ClientNetwork::SetOnRemotePlayerTimedOut(NetEvent& _function)
{
	if (NET_DEBUG > 0)
		DebugLog("Hooking function to OnRemotePlayerTimedOut.", LogSeverity::Info);

	m_onRemotePlayerTimedOut->push_back(_function);
}

void ClientNetwork::SetOnRemotePlayerKicked(NetEvent& _function)
{
	if (NET_DEBUG > 0)
		DebugLog("Hooking function to OnRemotePlayerKicked.", LogSeverity::Info);

	m_onRemotePlayerKicked->push_back(_function);
}

void ClientNetwork::SetOnRemotePlayerBanned(NetEvent& _function)
{
	if (NET_DEBUG > 0)
		DebugLog("Hooking function to OnRemotePlayerBanned.", LogSeverity::Info);

	m_onRemotePlayerBanned->push_back(_function);
}
