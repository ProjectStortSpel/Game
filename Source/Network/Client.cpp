#include "Client.h"
#include <RakNet/MessageIdentifiers.h>

Client::Client()
	: BaseNetwork()
{
	m_outgoingPort = 5357;
	m_incomingPort = 5358;
	m_remoteAddress = "127.0.0.1";
}

Client::~Client()
{
}

void Client::Connect(const char* _ipAddress, const char* _password, const int _outgoingPort, const int _incomingPort)
{

	m_remoteAddress = _ipAddress;
	m_password = _password;
	m_outgoingPort = _outgoingPort;
	m_incomingPort = _incomingPort;

	Connect();
}
void Client::Connect()
{
	if (NET_DEBUG)
	{
		printf("Client connecting to server:\n");
		printf("Ip address: \"%s\"\n", m_remoteAddress.c_str());
		printf("Remote Port: \"%i\"\n", m_outgoingPort);
		printf("Local Port: \"%i\"\n", m_incomingPort);
		printf("Password: \"%s\"\n", m_password.c_str());
	}

	// Disallow connection responses from any IP.
	// Usable when connecting to a server with multiple IP addresses.
	m_rakInterface->AllowConnectionResponseIPMigration(false);

	// Connecting the client is very simple.  0 means we don't care about
	// a connectionValidationInteger, and false for low priority threads

	// Describes the local socket used when connecting.
	// socketFamily describes if we should use IPV4 or IPV6 (AF_INET is IPV4)
	RakNet::SocketDescriptor socketDescriptor(m_incomingPort, 0);
	socketDescriptor.socketFamily = AF_INET;

	// Starts the network thread
	m_rakInterface->Startup(1, &socketDescriptor, 1);
	// Send an occasional ping to the server to check for response
	m_rakInterface->SetOccasionalPing(true);

	// Connect to the server
	RakNet::ConnectionAttemptResult car = m_rakInterface->Connect(m_remoteAddress.c_str(), m_outgoingPort, m_password.c_str(), (int)strlen(m_password.c_str()));
	if (car == RakNet::CONNECTION_ATTEMPT_STARTED)
	{
		if (NET_DEBUG)
			printf("Client started to connect.\n");

		StartListen();
	}
	else
		printf("Unable to start connecting.\n");
}

void Client::Disconect()
{
	if (NET_DEBUG)
		printf("Client disconnected from server.\n");

	if (m_receiveThreadAlive)
		StopListen();
	m_rakInterface->Shutdown(300);
	RakNet::RakPeerInterface::DestroyInstance(m_rakInterface);
}

void Client::SendToServer(PacketHandler::Packet _packet)
{
	m_rakInterface->Send((char*)_packet.Data, _packet.Length, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

	if (NET_DEBUG)
		printf("Client sent packet to server.\n");
}

void Client::ReceivePackets()
{
	RakNet::Packet* packet;
	//m_server->DeallocatePacket(m_packet),
	unsigned char packetIdentifier;
	for (packet = m_rakInterface->Receive(); packet; packet = m_rakInterface->Receive())
	{
		packetIdentifier = GetPacketIdentifier(packet);

		switch (packetIdentifier)
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
			// This tells the client they have connected
			if (NET_DEBUG)
				printf("Client connected to server.\n");

			TriggerEvent(m_onConnectedToServer, packetIdentifier, packet->systemAddress);
			break;


		case ID_DISCONNECTION_NOTIFICATION:
			// Disconnected from the server
			if (NET_DEBUG)
				printf("Client disconnected from server.\n");
			TriggerEvent(m_onDisconnectedFromServer, packetIdentifier, packet->systemAddress);
			break;
		case ID_CONNECTION_LOST:
			// Lost connection to the server
			if (NET_DEBUG)
				printf("Client lost connection from server.\n");
			TriggerEvent(m_onDisconnectedFromServer, packetIdentifier, packet->systemAddress);
			break;


		case ID_REMOTE_NEW_INCOMING_CONNECTION: 
			// Another user connected to the server
			if (NET_DEBUG)
				printf("Another client connected to server.\n");
			TriggerEvent(m_onPlayerConnected, packetIdentifier, packet->systemAddress);
			break;


		case ID_REMOTE_CONNECTION_LOST:
			// Another user lost connection to the server
			if (NET_DEBUG)
				printf("Another client lost connection to server.\n");
			TriggerEvent(m_onPlayerDisconnected, packetIdentifier, packet->systemAddress);
			break;
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			// Another user disconnected from the server
			if (NET_DEBUG)
				printf("Another client disconnected from server.\n");
			TriggerEvent(m_onPlayerDisconnected, packetIdentifier, packet->systemAddress);
			break;


		case ID_ALREADY_CONNECTED:
			// Already connected to the server
			if (NET_DEBUG)
				printf("Client already connected to server.\n");
			TriggerEvent(m_onFailedToConnect, packetIdentifier, packet->systemAddress);
			break;
		case ID_CONNECTION_BANNED: 
			// Banned from the server
			if (NET_DEBUG)
				printf("Client banned from server.\n");
			TriggerEvent(m_onFailedToConnect, packetIdentifier, packet->systemAddress);
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			// Server is full
			if (NET_DEBUG)
				printf("Server connecting to is full.\n");
			TriggerEvent(m_onFailedToConnect, packetIdentifier, packet->systemAddress);
			break;
		case ID_CONNECTION_ATTEMPT_FAILED:
			// Failed to send a connect request to the server
			if (NET_DEBUG)
				printf("Failed to connect to server.\n");
			TriggerEvent(m_onFailedToConnect, packetIdentifier, packet->systemAddress);
			break;
		case ID_INVALID_PASSWORD:
			// Incorrect password to the server
			if (NET_DEBUG)
				printf("Invalid password to server.\n");
			TriggerEvent(m_onFailedToConnect, packetIdentifier, packet->systemAddress);
			break;
		case ID_INCOMPATIBLE_PROTOCOL_VERSION:
			// Incompatible protocol version (IPV4/IPV6 ?)
			if (NET_DEBUG)
				printf("Incompatible protocol version (IPV4/IPV6).\n");
			TriggerEvent(m_onFailedToConnect, packetIdentifier, packet->systemAddress);
			break;


		case ID_CONNECTED_PING:
		case ID_UNCONNECTED_PING:
			if (NET_DEBUG)
				printf("Ping from %s.\n", packet->systemAddress.ToString(true));
			break;
		case ID_USER_PACKET:
		{
			if (NET_DEBUG)
				printf("Recieved user message from server.\n");

			PacketHandler::Packet* p = new PacketHandler::Packet();
			printf("ID_USER_PACKET from %s.\n", packet->systemAddress.ToString(true));
			p->Data = new unsigned char[packet->length];
			memcpy(p->Data, &packet->data[0], packet->length);

			p->Length = packet->length;
			p->Sender = &m_connectionMap[packet->systemAddress];

			m_packetLock.lock();
			m_packets.push(p);
			m_packetLock.unlock();

			break;
		}
		default:
			break;
		}

		m_rakInterface->DeallocatePacket(packet);
	}
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

void Client::SetOnFailedToConnect(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnFailedToConnect.\n");

	m_onFailedToConnect = _function;
}