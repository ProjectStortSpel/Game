#include "Server.h"
#include <RakNet/MessageIdentifiers.h>

Server::Server()
	: BaseNetwork()
{
	m_incomingPort = 5357;
	m_maxConnections = 8;
}


Server::~Server()
{
}


void Server::Start()
{
	if (NET_DEBUG)
	{
		printf("Starting server:\n");
		printf("Ip address: \"%s\"\n", m_rakInterface->GetLocalIP(0));
		printf("Port: \"%i\"\n", m_incomingPort);
		printf("Password: \"%s\"\n", m_password.c_str());
		printf("Max connections: \"%i\"\n", m_maxConnections);
	}

	m_rakInterface->SetIncomingPassword(m_password.c_str(), (int)strlen(m_password.c_str()));
	m_rakInterface->SetTimeoutTime(30000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);

	RakNet::SystemAddress clientID = RakNet::UNASSIGNED_SYSTEM_ADDRESS;


	RakNet::SocketDescriptor socketDescriptors[2];
	socketDescriptors[0].port = m_incomingPort;
	socketDescriptors[0].socketFamily = AF_INET; // Test out IPV4
	socketDescriptors[1].port = m_incomingPort;
	socketDescriptors[1].socketFamily = AF_INET6; // Test out IPV6

	m_rakInterface->SetMaximumIncomingConnections(m_maxConnections);
	
	RakNet::StartupResult startResult = m_rakInterface->Startup(m_maxConnections, socketDescriptors, 2);
	if (startResult == RakNet::RAKNET_STARTED)
	{
		if (NET_DEBUG)
			printf("\nServer started with IPV6!\n");
	}
	else
	{
		startResult = m_rakInterface->Startup(m_maxConnections, socketDescriptors, 1);
		if (startResult == RakNet::RAKNET_STARTED)
		{
			if (NET_DEBUG)
				printf("\nServer started with IPV4!\n");
		}
		else
		{
			if (NET_DEBUG)
				printf("\nServer failed to start. Exiting\n");
			exit(1);
		}
	}

	m_rakInterface->SetOccasionalPing(true);
	m_rakInterface->SetUnreliableTimeout(1000);

	StartListen();
}

void Server::Stop()
{
	if (m_receiveThreadAlive)
		StopListen();

	m_rakInterface->Shutdown(300);
	RakNet::RakPeerInterface::DestroyInstance(m_rakInterface);
}

void Server::Broadcast(PacketHandler::Packet _packet)
{
	m_rakInterface->Send((char*)_packet.Data, _packet.Length, HIGH_PRIORITY, RELIABLE_ORDERED, 0, _packet.Sender, true);
}

void Server::ReceivePackets()
{
	RakNet::Packet* packet;
	//m_server->DeallocatePacket(m_packet),
	unsigned char packetIdentifier;
	for (packet = m_rakInterface->Receive(); packet; packet = m_rakInterface->Receive())
	{
		packetIdentifier = GetPacketIdentifier(packet);

		switch (packetIdentifier)
		{
		case ID_NEW_INCOMING_CONNECTION:
			// User connected to server
			if (NET_DEBUG)
				printf("New client connected.\n");
			TriggerEvent(m_onPlayerConnected, packetIdentifier);
			break;

		case ID_CONNECTION_LOST:
			// User lost connection to server
			if (NET_DEBUG)
				printf("Client lost connection to server.\n");
			TriggerEvent(m_onPlayerDisconnected, packetIdentifier);
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			if (NET_DEBUG)
				printf("Client disconnected from server.\n");
			// User disconnected from server
			TriggerEvent(m_onPlayerDisconnected, packetIdentifier);
			break;

		case ID_INCOMPATIBLE_PROTOCOL_VERSION:
			if (NET_DEBUG)
				printf("Incompatible protocl version - Server.\n");
			break;

		case ID_CONNECTED_PING:
		case ID_UNCONNECTED_PING:
			if (NET_DEBUG)
				printf("Ping from %s\n", packet->systemAddress.ToString(true));
			break;

		case ID_USER_PACKET:
		{
			if (NET_DEBUG)
				printf("Recieved user message from \"%s\"", packet->systemAddress.ToString(true));

			PacketHandler::Packet* p = new PacketHandler::Packet();
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
	}
}