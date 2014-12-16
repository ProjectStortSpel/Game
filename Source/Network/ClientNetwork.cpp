#include "ClientNetwork.h"

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
	m_socketBound = new bool(false);

	m_receivePacketsThread = new std::thread();
	m_receivePacketsThreadAlive = new bool(false);

	m_ping = new float(0.0f);
	m_sendTime = new float();
	m_receiveTime = new float();

	m_currentTimeOutIntervall = new float();
	m_currentIntervallCounter = new int();

	*m_incomingPort = 0;
	m_socket = 0;
	m_connected = new bool(false);

	*m_maxTimeOutIntervall = 100.0f;
	*m_maxIntervallCounter = 300;

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

	(*m_networkFunctions)[NetTypeMessageId::ID_PASSWORD_INVALID] = std::bind(&ClientNetwork::NetPasswordInvalid, this, NetworkHookPlaceholders);
	(*m_networkFunctions)[NetTypeMessageId::ID_CONNECTION_ACCEPTED] = std::bind(&ClientNetwork::NetConnectionAccepted, this, NetworkHookPlaceholders);
	(*m_networkFunctions)[NetTypeMessageId::ID_SERVER_FULL] = std::bind(&ClientNetwork::NetConnectionServerFull, this, NetworkHookPlaceholders);

	(*m_networkFunctions)[NetTypeMessageId::ID_CONNECTION_DISCONNECTED] = std::bind(&ClientNetwork::NetConnectionDisconnected, this, NetworkHookPlaceholders);

	(*m_networkFunctions)[NetTypeMessageId::ID_CONNECTION_KICKED] = std::bind(&ClientNetwork::NetConnectionKicked, this, NetworkHookPlaceholders);
	(*m_networkFunctions)[NetTypeMessageId::ID_CONNECTION_BANNED] = std::bind(&ClientNetwork::NetConnectionBanned, this, NetworkHookPlaceholders);

	(*m_networkFunctions)[NetTypeMessageId::ID_PING] = std::bind(&ClientNetwork::NetPing, this, NetworkHookPlaceholders);
	(*m_networkFunctions)[NetTypeMessageId::ID_PONG] = std::bind(&ClientNetwork::NetPong, this, NetworkHookPlaceholders);

	(*m_networkFunctions)[NetTypeMessageId::ID_REMOTE_CONNECTION_ACCEPTED] = std::bind(&ClientNetwork::NetRemoteConnectionAccepted, this, NetworkHookPlaceholders);

	(*m_networkFunctions)[NetTypeMessageId::ID_REMOTE_CONNECTION_LOST] = std::bind(&ClientNetwork::NetRemoteConnectionLost, this, NetworkHookPlaceholders);
	(*m_networkFunctions)[NetTypeMessageId::ID_REMOTE_CONNECTION_DISCONNECTED] = std::bind(&ClientNetwork::NetRemoteConnectionDisconnected, this, NetworkHookPlaceholders);

	(*m_networkFunctions)[NetTypeMessageId::ID_REMOTE_CONNECTION_KICKED] = std::bind(&ClientNetwork::NetRemoteConnectionKicked, this, NetworkHookPlaceholders);
	(*m_networkFunctions)[NetTypeMessageId::ID_REMOTE_CONNECTION_BANNED] = std::bind(&ClientNetwork::NetRemoteConnectionBanned, this, NetworkHookPlaceholders);

}

ClientNetwork::~ClientNetwork()
{
	Disconnect();

	SAFE_DELETE(m_remoteAddress);
	SAFE_DELETE(m_outgoingPort);
	SAFE_DELETE(m_socketBound);
	SAFE_DELETE(m_receivePacketsThread);
	SAFE_DELETE(m_receivePacketsThreadAlive);
	SAFE_DELETE(m_ping);
	SAFE_DELETE(m_sendTime);
	SAFE_DELETE(m_receiveTime);
	SAFE_DELETE(m_currentTimeOutIntervall);
	SAFE_DELETE(m_currentIntervallCounter);
	SAFE_DELETE(m_connected);
	
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
	if (NET_DEBUG)
	{
		printf("Client connecting to server:\n");
		printf("Ip address: \"%s\"\n", m_remoteAddress->c_str());
		printf("Remote Port: \"%i\"\n", *m_outgoingPort);
		printf("Local Port: \"%i\"\n", *m_incomingPort);
		printf("Password: \"%s\"\n", m_password->c_str());
	}

	m_socket = ISocket::CreateSocket();

	if (!*m_socketBound)
	{
		m_socket->Bind(*m_incomingPort);
		*m_socketBound = true;
	}

	if (!m_socket->SetNoDelay(true))
		printf("Failed to set no delay CLIENT\n");

	bool connected = false;
	//for (int i = 0; i < 5; ++i)
	//{
		connected = m_socket->Connect(m_remoteAddress->c_str(), *m_outgoingPort);
		//if (connected)
		//	break;

		//NetSleep(1500);
	//}

	if (!connected)
	{
		NetConnection nc = NetConnection(m_remoteAddress->c_str(), *m_outgoingPort);
		TriggerEvent(m_onFailedToConnect, nc, 0);
		return false;
	}

	*m_connected = true;
	m_socket->SetNonBlocking(false);

	uint64_t id = m_packetHandler->StartPack(NetTypeMessageId::ID_PASSWORD_ATTEMPT);
	m_packetHandler->WriteString(id, m_password->c_str());
	auto packet = m_packetHandler->EndPack(id);
	Send(packet);

	*m_receivePacketsThreadAlive = true;
	*m_receivePacketsThread = std::thread(&ClientNetwork::ReceivePackets, this);

	return true;
}

void ClientNetwork::Disconnect()
{
	if (*m_socketBound)
	{
		uint64_t id = m_packetHandler->StartPack(ID_CONNECTION_DISCONNECTED);
		Packet* packet = m_packetHandler->EndPack(id);
		Send(packet);
	}

	*m_receivePacketsThreadAlive = false;

	if (m_receivePacketsThread->joinable())	
		m_receivePacketsThread->join();

	if (m_socket)
	{
		NetConnection nc = m_socket->GetNetConnection();
		TriggerEvent(m_onDisconnectedFromServer, nc, 0);

		delete m_socket;
		m_socket = 0;
	}
	*m_socketBound = 0;

	if (NET_DEBUG)
		printf("Client disconnected/shutdown.\n");

	*m_connected = false;
}

void ClientNetwork::ReceivePackets()
{
	// On its on thread
	while (*m_receivePacketsThreadAlive)
	{
		int result = m_socket->Receive(m_packetData, MAX_PACKET_SIZE);

		if (result > 0)
		{
			unsigned short packetSize = result;

			if (NET_DEBUG)
				printf("Received message with length \"%i\" from server.\n", packetSize);

			*m_currentIntervallCounter = 0;
			*m_currentTimeOutIntervall = 0.0f;

			Packet* p = new Packet();
			p->Data = new unsigned char[packetSize];
			*p->Length = packetSize;
			*p->Sender = m_socket->GetNetConnection();
			memcpy(p->Data, m_packetData, packetSize);

			HandlePacket(p);

			*m_totalDataReceived += packetSize;
			*m_currentDataReceived += packetSize;

		}
		else if (result == 0)
		{
			// server shutdown graceful
		}
		else
		{
			// Failed to receive packets
		}

	}
}

void ClientNetwork::Send(Packet* _packet)
{
	if (!*m_socketBound)
	{
		if (NET_DEBUG)
			printf("Not connected to server.\n");
		return;
	}

	float bytesSent = m_socket->Send((char*)_packet->Data, *_packet->Length);
	if (bytesSent != -1)
	{
		if (_packet->Data[0] == ID_PING)
			*m_sendTime = GetMillisecondsTime();

		*m_totalDataSent += bytesSent;
		*m_currentDataSent += bytesSent;
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

void ClientNetwork::NetPasswordInvalid(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	if (NET_DEBUG)
		printf("Password \"%s\" invalid, connection refused.\n", m_password->c_str());

	//Disconnect();
	*m_receivePacketsThreadAlive = false;

	SAFE_DELETE(m_socket);
	*m_socketBound = 0;

	TriggerEvent(m_onPasswordInvalid, _connection, 0);
}

void ClientNetwork::NetConnectionAccepted(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	if (NET_DEBUG)
		printf("Password accepted, connection accepted.\n");

	TriggerEvent(m_onConnectedToServer, _connection, 0);
}

void ClientNetwork::NetConnectionServerFull(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	if (NET_DEBUG)
		printf("Disconnected. Server is full.\n");

	//Disconnect();
	*m_receivePacketsThreadAlive = false;

	SAFE_DELETE(m_socket);
	*m_socketBound = 0;

	TriggerEvent(m_onServerFull, _connection, 0);

	if (m_receivePacketsThread->joinable())
		m_receivePacketsThread->join();
}

void ClientNetwork::NetConnectionLost(NetConnection& _connection)
{
	if (NET_DEBUG)
		printf("Disconnected. Connection timed out.\n");

	//Disconnect();
	*m_receivePacketsThreadAlive = false;

	SAFE_DELETE(m_socket);
	*m_socketBound = 0;

	TriggerEvent(m_onTimedOutFromServer, _connection, 0);

	if (m_receivePacketsThread->joinable())
		m_receivePacketsThread->join();
}

void ClientNetwork::NetConnectionDisconnected(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	if (NET_DEBUG)
		printf("Disconnected. Server shutdown.\n");

	//Disconnect();
	*m_receivePacketsThreadAlive = false;

	SAFE_DELETE(m_socket);
	*m_socketBound = 0;

	TriggerEvent(m_onDisconnectedFromServer, _connection, 0);

	if (m_receivePacketsThread->joinable())
		m_receivePacketsThread->join();
}


void ClientNetwork::NetConnectionKicked(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	if (NET_DEBUG)
		printf("Disconnected. You were kicked.\n");

	char* message = _packetHandler->ReadString(_id);

	TriggerEvent(m_onKickedFromServer, _connection, message);

	*m_receivePacketsThreadAlive = false;

	SAFE_DELETE(m_socket);
	*m_socketBound = 0;

	

	if (m_receivePacketsThread->joinable())
		m_receivePacketsThread->join();
}

void ClientNetwork::NetConnectionBanned(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	if (NET_DEBUG)
		printf("Disconnected. You were banned.\n");

	char* message = _packetHandler->ReadString(_id);

	//Disconnect();
	*m_receivePacketsThreadAlive = false;

	SAFE_DELETE(m_socket);
	*m_socketBound = 0;

	TriggerEvent(m_onBannedFromServer, _connection, message);

	if (m_receivePacketsThread->joinable())
		m_receivePacketsThread->join();
}


void ClientNetwork::NetPing(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	if (NET_DEBUG)
		printf("Ping from: %s:%d\n", _connection.GetIpAddress(), _connection.GetPort());

	uint64_t id = _packetHandler->StartPack(ID_PONG);
	Packet* p = _packetHandler->EndPack(id);
	Send(p);
}

void ClientNetwork::NetPong(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	if (NET_DEBUG)
		printf("Pong from: %s:%d\n", _connection.GetIpAddress(), _connection.GetPort());

	*m_receiveTime = GetMillisecondsTime();
	*m_ping = *m_receiveTime - *m_sendTime;
}


void ClientNetwork::NetRemoteConnectionAccepted(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	char* name = _packetHandler->ReadString(_id);
	if (NET_DEBUG)
		printf("%s connected to the server.\n", name);

	TriggerEvent(m_onRemotePlayerConnected, _connection, 0);
}

void ClientNetwork::NetRemoteConnectionLost(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	char* name = _packetHandler->ReadString(_id);
	if (NET_DEBUG)
		printf("%s timed out to the server.\n", name);

	TriggerEvent(m_onRemotePlayerTimedOut, _connection, 0);
}

void ClientNetwork::NetRemoteConnectionDisconnected(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	char* name = _packetHandler->ReadString(_id);
	if (NET_DEBUG)
		printf("%s disconnected from the server.\n", name);

	TriggerEvent(m_onRemotePlayerDisconnected, _connection, 0);
}

void ClientNetwork::NetRemoteConnectionKicked(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	char* name = _packetHandler->ReadString(_id);

	if (NET_DEBUG)
		printf("%s was kicked from the server.\n", name);

	TriggerEvent(m_onRemotePlayerKicked, _connection, 0);
}

void ClientNetwork::NetRemoteConnectionBanned(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection)
{
	char* name = _packetHandler->ReadString(_id);

	if (NET_DEBUG)
		printf("%s was banned from the server.\n", name);

	TriggerEvent(m_onRemotePlayerBanned, _connection, 0);
}


void ClientNetwork::SetOnConnectedToServer(NetEvent& _function)
{
	if (NET_DEBUG)	
		printf("Hooking function to OnConnectedToServer.\n");

	m_onConnectedToServer->push_back(_function);
}

void ClientNetwork::SetOnDisconnectedFromServer(NetEvent& _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnDisconnectedFromServer.\n");

	m_onDisconnectedFromServer->push_back(_function);
}

void ClientNetwork::SetOnTimedOutFromServer(NetEvent& _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnTimedOutFromServer.\n");

	m_onTimedOutFromServer->push_back(_function);
}

void ClientNetwork::SetOnFailedToConnect(NetEvent& _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnFailedToConnect.\n");

	m_onFailedToConnect->push_back(_function);
}

void ClientNetwork::SetOnPasswordInvalid(NetEvent& _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnPasswordInvalid.\n");

	m_onPasswordInvalid->push_back(_function);
}

void ClientNetwork::SetOnKickedFromServer(NetEvent& _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnKickedFromServer.\n");

	m_onKickedFromServer->push_back(_function);
}

void ClientNetwork::SetOnBannedFromServer(NetEvent& _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnBannedFromServer.\n");

	m_onBannedFromServer->push_back(_function);
}

void ClientNetwork::SetOnServerFull(NetEvent& _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnServerFull.\n");

	m_onServerFull->push_back(_function);
}

void ClientNetwork::SetOnRemotePlayerConnected(NetEvent& _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnRemotePlayerConnected.\n");

	m_onRemotePlayerConnected->push_back(_function);
}

void ClientNetwork::SetOnRemotePlayerDisconnected(NetEvent& _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnRemotePlayerDisconnected.\n");

	m_onRemotePlayerDisconnected->push_back(_function);
}

void ClientNetwork::SetOnRemotePlayerTimedOut(NetEvent& _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnRemotePlayerTimedOut.\n");

	m_onRemotePlayerTimedOut->push_back(_function);
}

void ClientNetwork::SetOnRemotePlayerKicked(NetEvent& _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnRemotePlayerKicked.\n");

	m_onRemotePlayerKicked->push_back(_function);
}

void ClientNetwork::SetOnRemotePlayerBanned(NetEvent& _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnRemotePlayerBanned.\n");

	m_onRemotePlayerBanned->push_back(_function);
}