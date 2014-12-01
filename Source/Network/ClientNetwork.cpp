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
		return false;
	
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

}
void ClientNetwork::NetConnectionAccepted(PacketHandler* _packetHandler, uint64_t _id, NetConnection _connection)
{
	if (NET_DEBUG)
		printf("Password accepted, connection accepted.\n");

	uint64_t id = _packetHandler->StartPack("localhest");

	_packetHandler->WriteByte(id, '3');
	_packetHandler->WriteShort(id, 1337);
	_packetHandler->WriteInt(id, 555);
	_packetHandler->WriteFloat(id, 3.1415);
	_packetHandler->WriteString(id, "test med mellanslag!?");
	_packetHandler->WriteByte(id, '9');

	Packet* packet = _packetHandler->EndPack(id);

	Send(packet);

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