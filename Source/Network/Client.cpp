#include "Client.h"

#ifdef WIN32
#else
#include <sys/socket.h>
#endif

Client::Client()
	: BaseNetwork()
{
	m_outgoingPort = 6112;
	m_incomingPort = 0;
	m_remoteAddress = "127.0.0.1";
	m_socketBound = false;
}

Client::~Client()
{
	if (m_listenForPacketsThreadAlive)
		StopListenForPackets();

	SAFE_DELETE(m_socket);
}

bool Client::Connect(const char* _ipAddress, const char* _password, const int _outgoingPort, const int _incomingPort)
{

	m_remoteAddress = _ipAddress;
	m_password		= _password;
	m_outgoingPort	= _outgoingPort;
	m_incomingPort	= _incomingPort;

	return Connect();
}
bool Client::Connect()
{
	m_socket = ISocket::CreateISocket(AF_INET, SOCK_STREAM, 0);

	if (NET_DEBUG)
	{
		printf("Client connecting to server:\n");
		printf("Ip address: \"%s\"\n", m_remoteAddress.c_str());
		printf("Remote Port: \"%i\"\n", m_outgoingPort);
		printf("Local Port: \"%i\"\n", m_incomingPort);
		printf("Password: \"%s\"\n", m_password.c_str());
	}

	if (!m_socketBound)
	{
		m_socket->Bind(m_incomingPort);
		m_socketBound = true;
	}

	return m_socket->Connect(m_remoteAddress.c_str(), m_outgoingPort);
}

void Client::Disconect()
{
	if (NET_DEBUG)
		printf("Client disconnected from server.\n");
	
	StopListenForPackets();

	SAFE_DELETE(m_socket);
	m_socketBound = false;
}
//
void Client::SendToServer(PacketHandler::Packet _packet)
{
	if (m_socketBound)
		m_socket->Send((char*)_packet.Data, _packet.Length, 0);

	if (NET_DEBUG)
		printf("Client sent packet to server.\n");
}

void Client::ReceivePackets()
{
	//RakNet::Packet* packet;
	////m_server->DeallocatePacket(m_packet),
	//unsigned char packetIdentifier;
	//for (packet = m_rakInterface->Receive(); packet; packet = m_rakInterface->Receive())
	//{
	//	packetIdentifier = GetPacketIdentifier(packet);

	//	switch (packetIdentifier)
	//	{
	//	case ID_CONNECTION_REQUEST_ACCEPTED:
	//		// This tells the client they have connected
	//		if (NET_DEBUG)
	//			printf("Client connected to server.\n");

	//		TriggerEvent(m_onConnectedToServer, packetIdentifier, packet->systemAddress);
	//		break;


	//	case ID_DISCONNECTION_NOTIFICATION:
	//		// Disconnected from the server
	//		if (NET_DEBUG)
	//			printf("Client disconnected from server.\n");
	//		TriggerEvent(m_onDisconnectedFromServer, packetIdentifier, packet->systemAddress);
	//		break;
	//	case ID_CONNECTION_LOST:
	//		// Lost connection to the server
	//		if (NET_DEBUG)
	//			printf("Client lost connection from server.\n");
	//		TriggerEvent(m_onTimedOutFromServer, packetIdentifier, packet->systemAddress);
	//		break;


	//	case ID_REMOTE_NEW_INCOMING_CONNECTION: 
	//		// Another user connected to the server
	//		if (NET_DEBUG)
	//			printf("Another client connected to server.\n");
	//		TriggerEvent(m_onRemotePlayerConnected, packetIdentifier, packet->systemAddress);
	//		break;


	//	case ID_REMOTE_CONNECTION_LOST:
	//		// Another user lost connection to the server
	//		if (NET_DEBUG)
	//			printf("Another client lost connection to server.\n");
	//		TriggerEvent(m_onRemotePlayerTimedOut, packetIdentifier, packet->systemAddress);
	//		break;
	//	case ID_REMOTE_DISCONNECTION_NOTIFICATION:
	//		// Another user disconnected from the server
	//		if (NET_DEBUG)
	//			printf("Another client disconnected from server.\n");
	//		TriggerEvent(m_onRemotePlayerDisconnected, packetIdentifier, packet->systemAddress);
	//		break;


	//	case ID_ALREADY_CONNECTED:
	//		// Already connected to the server
	//		if (NET_DEBUG)
	//			printf("Client already connected to server.\n");
	//		TriggerEvent(m_onFailedToConnect, packetIdentifier, packet->systemAddress);
	//		break;
	//	case ID_CONNECTION_BANNED: 
	//		// Banned from the server
	//		if (NET_DEBUG)
	//			printf("Client banned from server.\n");
	//		TriggerEvent(m_onFailedToConnect, packetIdentifier, packet->systemAddress);
	//		break;
	//	case ID_NO_FREE_INCOMING_CONNECTIONS:
	//		// Server is full
	//		if (NET_DEBUG)
	//			printf("Server connecting to is full.\n");
	//		TriggerEvent(m_onFailedToConnect, packetIdentifier, packet->systemAddress);
	//		break;
	//	case ID_CONNECTION_ATTEMPT_FAILED:
	//		// Failed to send a connect request to the server
	//		if (NET_DEBUG)
	//			printf("Failed to connect to server.\n");
	//		TriggerEvent(m_onFailedToConnect, packetIdentifier, packet->systemAddress);
	//		break;
	//	case ID_INVALID_PASSWORD:
	//		// Incorrect password to the server
	//		if (NET_DEBUG)
	//			printf("Invalid password to server.\n");
	//		TriggerEvent(m_onFailedToConnect, packetIdentifier, packet->systemAddress);
	//		break;
	//	case ID_INCOMPATIBLE_PROTOCOL_VERSION:
	//		// Incompatible protocol version (IPV4/IPV6 ?)
	//		if (NET_DEBUG)
	//			printf("Incompatible protocol version (IPV4/IPV6).\n");
	//		TriggerEvent(m_onFailedToConnect, packetIdentifier, packet->systemAddress);
	//		break;


	//	case ID_CONNECTED_PING:
	//	case ID_UNCONNECTED_PING:
	//		if (NET_DEBUG)
	//			printf("Ping from %s.\n", packet->systemAddress.ToString(true));
	//		break;
	//	case ID_USER_PACKET:
	//	{
	//		if (NET_DEBUG)
	//			printf("Recieved \"ID_USER_PACKET\" from server %s.\n", packet->systemAddress.ToString(true));

	//		PacketHandler::Packet* p = new PacketHandler::Packet();
	//		p->Data = new unsigned char[packet->length];
	//		memcpy(p->Data, &packet->data[0], packet->length);

	//		p->Length = packet->length;
	//		p->Sender = &m_connectionMap[packet->systemAddress];

	//		m_packetLock.lock();
	//		m_packets.push(p);
	//		m_packetLock.unlock();

	//		break;
	//	}
	//	default:
	//		break;
	//	}

	//	m_rakInterface->DeallocatePacket(packet);
	//}
}

void Client::SetOnConnectedToServer(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnConnectedToServer.\n");

	m_onConnectedToServer = _function;
}

void Client::SetOnDisconnectedFromServer(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnDisconnectedFromServer.\n");

	m_onDisconnectedFromServer = _function;
}

void Client::SetOnTimedOutFromServer(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnTimedOutFromServer.\n");

	m_onTimedOutFromServer = _function;
}

void Client::SetOnFailedToConnect(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnFailedToConnect.\n");

	m_onFailedToConnect = _function;
}

void Client::SetOnRemotePlayerConnected(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnRemotePlayerConnected.\n");

	m_onRemotePlayerConnected = _function;
}

void Client::SetOnRemotePlayerDisconnected(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnRemotePlayerDisconnected.\n");

	m_onRemotePlayerDisconnected = _function;
}

void Client::SetOnRemotePlayerTimedOut(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnRemotePlayerTimedOut.\n");

	m_onRemotePlayerTimedOut = _function;
}
