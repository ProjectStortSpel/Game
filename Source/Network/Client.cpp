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

void Client::Connect(const char* _ipAddress, const char* _password, const int _port, const int _clientPort)
{
	m_remoteAddress = _ipAddress;
	m_password = _password;
	m_outgoingPort = _port;
	m_incomingPort = _clientPort;

	Connect();
}
void Client::Connect()
{
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
	m_rakInterface->Startup(8, &socketDescriptor, 1);
	// Send an occasional ping to the server to check for response
	m_rakInterface->SetOccasionalPing(true);

	// Connect to the server
	RakNet::ConnectionAttemptResult car = m_rakInterface->Connect(m_remoteAddress.c_str(), m_outgoingPort, m_password.c_str(), (int)strlen(m_password.c_str()));
	if (car == RakNet::CONNECTION_ATTEMPT_STARTED)
	{
		// Start a new thread to listen to packets on
		//m_thread = std::thread(&Client::Run, this);
		//Run();
	}
	else
		printf("Unable to start connecting.\n");
}

void Client::Disconect()
{
	if (m_receiveThreadAlive)
		StopListen();

	m_rakInterface->Shutdown(300);
	RakNet::RakPeerInterface::DestroyInstance(m_rakInterface);
}

void Client::SendToServer(PacketHandler::Packet _packet)
{
	m_rakInterface->Send((char*)_packet.Data, _packet.Length, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Client::RecivePackets()
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
			TriggerEvent(m_onConnectedToServer, packetIdentifier);
			break;


		case ID_DISCONNECTION_NOTIFICATION:
			// Disconnected from the server
			TriggerEvent(m_onDisconnectedFromServer, packetIdentifier);
			break;
		case ID_CONNECTION_LOST:
			// Lost connection to the server
			TriggerEvent(m_onDisconnectedFromServer, packetIdentifier);
			break;


		case ID_REMOTE_NEW_INCOMING_CONNECTION: 
			// Another user connected to the server
			TriggerEvent(m_onPlayerConnected, packetIdentifier);
			break;


		case ID_REMOTE_CONNECTION_LOST:
			// Another user lost connection to the server
			TriggerEvent(m_onPlayerDisconnected, packetIdentifier);
			break;
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			// Another user disconnected from the server
			TriggerEvent(m_onPlayerDisconnected, packetIdentifier);
			break;


		case ID_ALREADY_CONNECTED:
			// Already connected to the server
			TriggerEvent(m_onFailedToConnect, packetIdentifier);
			break;
		case ID_CONNECTION_BANNED: 
			// Banned from the server
			TriggerEvent(m_onFailedToConnect, packetIdentifier);
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			// Server is full
			TriggerEvent(m_onFailedToConnect, packetIdentifier);
			break;
		case ID_CONNECTION_ATTEMPT_FAILED:
			// Failed to send a connect request to the server
			TriggerEvent(m_onFailedToConnect, packetIdentifier);
			break;
		case ID_INVALID_PASSWORD:
			// Incorrect password to the server
			TriggerEvent(m_onFailedToConnect, packetIdentifier);
			break;
		case ID_INCOMPATIBLE_PROTOCOL_VERSION:
			// Incompatible protocol version (IPV4/IPV6 ?)
			TriggerEvent(m_onFailedToConnect, packetIdentifier);
			break;


		case ID_CONNECTED_PING:
		case ID_UNCONNECTED_PING:
			printf("Ping from %s\n", packet->systemAddress.ToString(true));
			break;
		case ID_USER_PACKET:
		{
			PacketHandler::Packet* p = new PacketHandler::Packet();
			printf("ID_USER_PACKET from %s\n", packet->systemAddress.ToString(true));
			p->Data = new unsigned char[packet->length];
			memcpy(p->Data, &packet->data[0], packet->length);

			p->Length = packet->length;
			p->Sender = packet->systemAddress;

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
	m_onConnectedToServer = _function;
}

void Client::SetOnDisconnectedFromServer(NetEvent _function)
{
	m_onDisconnectedFromServer = _function;
}

void Client::SetOnFailedToConnect(NetEvent _function)
{
	m_onFailedToConnect = _function;
}