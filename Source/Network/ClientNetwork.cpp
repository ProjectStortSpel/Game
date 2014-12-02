#include "ClientNetwork.h"

#ifdef WIN32
#else
#include <sys/socket.h>
#endif

ClientNetwork::ClientNetwork()
	: BaseNetwork()
{
	m_outgoingPort = 5357;
	m_incomingPort = 0;
	m_remoteAddress = "127.0.0.1";
	m_socketBound = false;
	m_receivePacketsThreadAlive = false;
	m_socket = 0;

	m_networkFunctions[NetTypeMessageId::ID_PASSWORD_INVALID] = std::bind(&ClientNetwork::NetPasswordInvalid, this, NetworkHookPlaceholders);
	m_networkFunctions[NetTypeMessageId::ID_CONNECTION_ACCEPTED] = std::bind(&ClientNetwork::NetConnectionAccepted, this, NetworkHookPlaceholders);
	m_networkFunctions[NetTypeMessageId::ID_SERVER_FULL] = std::bind(&ClientNetwork::NetConnectionServerFull, this, NetworkHookPlaceholders);

	m_networkFunctions[NetTypeMessageId::ID_CONNECTION_LOST] = std::bind(&ClientNetwork::NetConnectionLost, this, NetworkHookPlaceholders);
	m_networkFunctions[NetTypeMessageId::ID_CONNECTION_DISCONNECTED] = std::bind(&ClientNetwork::NetConnectionDisconnected, this, NetworkHookPlaceholders);

	m_networkFunctions[NetTypeMessageId::ID_CONNECTION_KICKED] = std::bind(&ClientNetwork::NetConnectionKicked, this, NetworkHookPlaceholders);
	m_networkFunctions[NetTypeMessageId::ID_CONNECTION_BANNED] = std::bind(&ClientNetwork::NetConnectionBanned, this, NetworkHookPlaceholders);

	m_networkFunctions[NetTypeMessageId::ID_REMOTE_CONNECTION_ACCEPTED] = std::bind(&ClientNetwork::NetRemoteConnectionAccepted, this, NetworkHookPlaceholders);

	m_networkFunctions[NetTypeMessageId::ID_REMOTE_CONNECTION_LOST] = std::bind(&ClientNetwork::NetRemoteConnectionLost, this, NetworkHookPlaceholders);
	m_networkFunctions[NetTypeMessageId::ID_REMOTE_CONNECTION_DISCONNECTED] = std::bind(&ClientNetwork::NetRemoteConnectionDisconnected, this, NetworkHookPlaceholders);

	m_networkFunctions[NetTypeMessageId::ID_REMOTE_CONNECTION_KICKED] = std::bind(&ClientNetwork::NetRemoteConnectionKicked, this, NetworkHookPlaceholders);
	m_networkFunctions[NetTypeMessageId::ID_REMOTE_CONNECTION_BANNED] = std::bind(&ClientNetwork::NetRemoteConnectionBanned, this, NetworkHookPlaceholders);

}

ClientNetwork::~ClientNetwork()
{
	Disconnect();
}

bool ClientNetwork::Connect(const char* _ipAddress, const char* _password, const int _outgoing, const int _incomingPort)
{
	m_remoteAddress = _ipAddress;
	m_password = _password;
	m_outgoingPort = _outgoing;
	m_incomingPort = _incomingPort;

	return Connect();
}
bool ClientNetwork::Connect()
{
	if (NET_DEBUG)
	{
		printf("Client connecting to server:\n");
		printf("Ip address: \"%s\"\n", m_remoteAddress.c_str());
		printf("Remote Port: \"%i\"\n", m_outgoingPort);
		printf("Local Port: \"%i\"\n", m_incomingPort);
		printf("Password: \"%s\"\n", m_password.c_str());
	}

	m_socket = ISocket::CreateSocket();

	if (!m_socketBound)
	{
		m_socket->Bind(m_incomingPort);
		m_socketBound = true;
	}

	bool connected = false;
	for (int i = 0; i < 5; ++i)
	{
		connected = m_socket->Connect(m_remoteAddress.c_str(), m_outgoingPort);
		if (connected)
			break;

		NetSleep(1500);
	}

	if (!connected)
	{
		if (m_onFailedToConnect)
		{
			m_onFailedToConnect(NetConnection(m_remoteAddress, m_outgoingPort));
		}
		return false;
	}
	
	uint64_t id = m_packetHandler.StartPack(NetTypeMessageId::ID_PASSWORD_ATTEMPT);
	m_packetHandler.WriteString(id, m_password.c_str());
	auto packet = m_packetHandler.EndPack(id);
	Send(packet);

	m_receivePacketsThreadAlive = true;
	m_receivePacketsThread = std::thread(&ClientNetwork::ReceivePackets, this);

	return true;
}
void ClientNetwork::Disconnect()
{
	uint64_t id = m_packetHandler.StartPack(ID_CONNECTION_DISCONNECTED);
	Packet* packet = m_packetHandler.EndPack(id);
	Send(packet);

	m_receivePacketsThreadAlive = false;

	if (m_receivePacketsThread.joinable())	
		m_receivePacketsThread.join();

	SAFE_DELETE(m_socket);
	m_socketBound = 0;

	if (NET_DEBUG)
		printf("Client disconnected/shutdown.\n");
}

void ClientNetwork::ReceivePackets()
{
	// On its on thread
	while (m_receivePacketsThreadAlive)
	{
		int result = m_socket->Receive(m_packetData, MAX_PACKET_SIZE);

		if (result > 0)
		{
			unsigned short packetSize = result;

			if (NET_DEBUG)
				printf("Received message with length \"%i\" from server.\n", packetSize);

			Packet* p = new Packet();
			p->Data = new unsigned char[packetSize];
			p->Length = packetSize;
			p->Sender = m_socket->GetNetConnection();
			memcpy(p->Data, m_packetData, packetSize);

			HandlePacket(p);

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
	if (!m_socketBound)
	{
		if (NET_DEBUG)
			printf("Not connected to server.\n");
		return;
	}

	m_socket->Send((char*)_packet->Data, _packet->Length);
	SAFE_DELETE(_packet);
}

void ClientNetwork::NetPasswordInvalid(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection)
{
	if (NET_DEBUG)
		printf("Password \"%s\" invalid, connection refused.\n", m_password.c_str());

	//Disconnect();
	m_receivePacketsThreadAlive = false;

	SAFE_DELETE(m_socket);
	m_socketBound = 0;

	if (m_onPasswordInvalid)
		m_onPasswordInvalid(_connection);

}
void ClientNetwork::NetConnectionAccepted(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection)
{
	if (NET_DEBUG)
		printf("Password accepted, connection accepted.\n");

	if (m_onConnectedToServer)
		m_onConnectedToServer(_connection);
}

void ClientNetwork::NetConnectionServerFull(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection)
{
	if (NET_DEBUG)
		printf("Disconnected. Server is full.\n");

	//Disconnect();
	m_receivePacketsThreadAlive = false;

	SAFE_DELETE(m_socket);
	m_socketBound = 0;

	if (m_onServerFull)
		m_onServerFull(_connection);

	if (m_receivePacketsThread.joinable())
		m_receivePacketsThread.join();
}

void ClientNetwork::NetConnectionLost(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection)
{
	if (NET_DEBUG)
		printf("Disconnected. Connection timed out.\n");

	//Disconnect();
	m_receivePacketsThreadAlive = false;

	SAFE_DELETE(m_socket);
	m_socketBound = 0;

	if (m_onTimedOutFromServer)
		m_onTimedOutFromServer(_connection);

	if (m_receivePacketsThread.joinable())
		m_receivePacketsThread.join();
}

void ClientNetwork::NetConnectionDisconnected(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection)
{
	if (NET_DEBUG)
		printf("Disconnected. Server shutdown.\n");

	//Disconnect();
	m_receivePacketsThreadAlive = false;

	SAFE_DELETE(m_socket);
	m_socketBound = 0;

	if (m_onDisconnectedFromServer)
		m_onDisconnectedFromServer(_connection);

	if (m_receivePacketsThread.joinable())
		m_receivePacketsThread.join();
}

void ClientNetwork::NetConnectionKicked(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection)
{
	if (NET_DEBUG)
		printf("Disconnected. You were kicked.\n");

	//Disconnect();
	m_receivePacketsThreadAlive = false;

	SAFE_DELETE(m_socket);
	m_socketBound = 0;

	if (m_onKickedFromServer)
		m_onKickedFromServer(_connection);

	if (m_receivePacketsThread.joinable())
		m_receivePacketsThread.join();
}

void ClientNetwork::NetConnectionBanned(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection)
{
	if (NET_DEBUG)
		printf("Disconnected. You were banned.\n");

	//Disconnect();
	m_receivePacketsThreadAlive = false;

	SAFE_DELETE(m_socket);
	m_socketBound = 0;

	if (m_onBannedFromServer)
		m_onBannedFromServer(_connection);

	if (m_receivePacketsThread.joinable())
		m_receivePacketsThread.join();
}

//Remote
void ClientNetwork::NetRemoteConnectionAccepted(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection)
{
	std::string name = _packetHandler->ReadString(_id);
	if (NET_DEBUG)
		printf("%s connected to the server.\n", name.c_str());

	if (m_onRemotePlayerConnected)
		m_onRemotePlayerConnected(_connection);
}


void ClientNetwork::NetRemoteConnectionLost(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection)
{
	std::string name = _packetHandler->ReadString(_id);
	if (NET_DEBUG)
		printf("%s timed out to the server.\n", name.c_str());

	if (m_onRemotePlayerTimedOut)
		m_onRemotePlayerTimedOut(_connection);
}

void ClientNetwork::NetRemoteConnectionDisconnected(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection)
{
	std::string name = _packetHandler->ReadString(_id);
	if (NET_DEBUG)
		printf("%s disconnected from the server.\n", name.c_str());

	if (m_onRemotePlayerDisconnected)
		m_onRemotePlayerDisconnected(_connection);
}


void ClientNetwork::NetRemoteConnectionKicked(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection)
{
	std::string name = _packetHandler->ReadString(_id);
	if (NET_DEBUG)
		printf("%s was kicked from the server.\n", name.c_str());

	if (m_onRemotePlayerKicked)
		m_onRemotePlayerKicked(_connection);
}

void ClientNetwork::NetRemoteConnectionBanned(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection)
{
	std::string name = _packetHandler->ReadString(_id);
	if (NET_DEBUG)
		printf("%s was banned from the server.\n", name.c_str());

	if (m_onRemotePlayerBanned)
		m_onRemotePlayerBanned(_connection);
}



void ClientNetwork::SetOnConnectedToServer(NetEvent _function)
{
	if (NET_DEBUG)	
		printf("Hooking function to OnConnectedToServer.\n");

	m_onConnectedToServer = _function;
}

void ClientNetwork::SetOnDisconnectedFromServer(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnDisconnectedFromServer.\n");

	m_onDisconnectedFromServer = _function;
}

void ClientNetwork::SetOnTimedOutFromServer(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnTimedOutFromServer.\n");

	m_onTimedOutFromServer = _function;
}

void ClientNetwork::SetOnFailedToConnect(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnFailedToConnect.\n");

	m_onFailedToConnect = _function;
}

void ClientNetwork::SetOnPasswordInvalid(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnPasswordInvalid.\n");

	m_onPasswordInvalid = _function;
}

void ClientNetwork::SetOnKickedFromServer(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnKickedFromServer.\n");

	m_onKickedFromServer = _function;
}

void ClientNetwork::SetOnBannedFromServer(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnBannedFromServer.\n");

	m_onBannedFromServer = _function;
}

void ClientNetwork::SetOnServerFull(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnServerFull.\n");

	m_onServerFull = _function;
}

void ClientNetwork::SetOnRemotePlayerConnected(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnRemotePlayerConnected.\n");

	m_onRemotePlayerConnected = _function;
}

void ClientNetwork::SetOnRemotePlayerDisconnected(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnRemotePlayerDisconnected.\n");

	m_onRemotePlayerDisconnected = _function;
}

void ClientNetwork::SetOnRemotePlayerTimedOut(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnRemotePlayerTimedOut.\n");

	m_onRemotePlayerTimedOut = _function;
}

void ClientNetwork::SetOnRemotePlayerKicked(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnRemotePlayerKicked.\n");

	m_onRemotePlayerKicked = _function;
}

void ClientNetwork::SetOnRemotePlayerBanned(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnRemotePlayerBanned.\n");

	m_onRemotePlayerBanned = _function;
}