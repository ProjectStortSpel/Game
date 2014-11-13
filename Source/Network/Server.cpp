#include "stdafx.h"
#include "Server.h"
#include <RakNet\MessageIdentifiers.h>

#include <mutex>


Server::Server()
{
	m_server = RakNet::RakPeerInterface::GetInstance();
}


Server::~Server()
{
}


ProjectStortSpel::StartResult Server::Connect()
{
	m_server->SetIncomingPassword(m_password.c_str(), (int)strlen(m_password.c_str()));
	m_server->SetTimeoutTime(30000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);

	RakNet::SystemAddress clientID = RakNet::UNASSIGNED_SYSTEM_ADDRESS;


	RakNet::SocketDescriptor socketDescriptors[2];
	socketDescriptors[0].port = m_port;
	socketDescriptors[0].socketFamily = AF_INET; // Test out IPV4
	socketDescriptors[1].port = m_port;
	socketDescriptors[1].socketFamily = AF_INET6; // Test out IPV6
	bool b = m_server->Startup(4, socketDescriptors, 2) == RakNet::RAKNET_STARTED;
	m_server->SetMaximumIncomingConnections(4);
	if (!b)
	{
		printf("Failed to start dual IPV4 and IPV6 ports. Trying IPV4 only.\n");

		// Try again, but leave out IPV6
		b = m_server->Startup(4, socketDescriptors, 1) == RakNet::RAKNET_STARTED;
		if (!b)
		{
			puts("Server failed to start.  Terminating.");
			exit(1);
		}
	}
	m_server->SetOccasionalPing(true);
	m_server->SetUnreliableTimeout(1000);

	DataStructures::List< RakNet::RakNetSocket2* > sockets;
	m_server->GetSockets(sockets);

	printf("Socket addresses used by RakNet:\n");
	for (unsigned int i = 0; i < sockets.Size(); i++)
	{
		printf("%i. %s\n", i + 1, sockets[i]->GetBoundAddress().ToString(true));
	}

	printf("\nMy IP addresses:\n");
	for (unsigned int i = 0; i < m_server->GetNumberOfAddresses(); i++)
	{
		RakNet::SystemAddress sa = m_server->GetInternalID(RakNet::UNASSIGNED_SYSTEM_ADDRESS, i);
		printf("%i. %s (LAN=%i)\n", i + 1, sa.ToString(false), sa.IsLANAddress());
	}

	printf("\nMy GUID is %s\n", m_server->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS).ToString());

	m_thread = std::thread(&Server::Run, this);

	return ProjectStortSpel::UNKNOWN;
}
void Server::Disconect()
{
}


void Server::Run()
{
	while (1)
	{
		RecivePackets();
	}
}

unsigned char GetPacketIdentifier(RakNet::Packet *p);
void Server::RecivePackets()
{
	RakNet::Packet* packet;
	//m_server->DeallocatePacket(m_packet),
	unsigned char packetIdentifier;
	for (packet = m_server->Receive(); packet; packet = m_server->Receive())
	{
		packetIdentifier = GetPacketIdentifier(packet);

		switch (packetIdentifier)
		{
		case ID_DISCONNECTION_NOTIFICATION:
			// Connection lost normally
			printf("ID_DISCONNECTION_NOTIFICATION from %s\n", packet->systemAddress.ToString(true));;
			break;


		case ID_NEW_INCOMING_CONNECTION:
			// Somebody connected.  We have their IP now
			printf("ID_NEW_INCOMING_CONNECTION from %s with GUID %s\n", packet->systemAddress.ToString(true), packet->guid.ToString());
			//clientID = p->systemAddress; // Record the player ID of the client

			printf("Remote internal IDs:\n");
			for (int index = 0; index < MAXIMUM_NUMBER_OF_INTERNAL_IDS; index++)
			{
				RakNet::SystemAddress internalId = m_server->GetInternalID(packet->systemAddress, index);
				if (internalId != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
				{
					printf("%i. %s\n", index + 1, internalId.ToString(true));
				}
			}

			break;

		case ID_INCOMPATIBLE_PROTOCOL_VERSION:
			printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
			break;

		case ID_CONNECTED_PING:
		case ID_UNCONNECTED_PING:
			printf("Ping from %s\n", packet->systemAddress.ToString(true));
			break;

		case ID_CONNECTION_LOST:
			// Couldn't deliver a reliable packet - i.e. the other system was abnormally
			// terminated
			printf("ID_CONNECTION_LOST from %s\n", packet->systemAddress.ToString(true));;
			break;

		default:
			m_packetLock.lock();
			m_packets.push(packet);
			m_packetLock.unlock();
		}

		//if (packetIdentifer == Server_Specific_Message
		//		HandleMessage
		//else
		

		
	}
}

RakNet::Packet* Server::GetPacket()
{
	//RecivePackets();

	RakNet::Packet* p = NULL;

	if (!m_packets.empty())
	{
		RakNet::Packet* p = m_packets.front();
		m_server->DeallocatePacket(m_packets.front());
		m_packets.pop();
	}
	
	return p;
}

void Server::SetOnUserConnect(std::function<void()> _function)
{
	m_onUserConnect = _function;
}

void Server::SetOnUserDisconnect(std::function<void()> _function)
{
	m_onUserDisconnect = _function;
}

void Server::SetOnUserTimeOut(std::function<void()> _function)
{
	m_onUserTimeOut = _function;
}

unsigned char Server::GetPacketIdentifier(RakNet::Packet *p)
{
	if (p == 0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		RakAssert(p->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
		return (unsigned char)p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
	}
	else
		return (unsigned char)p->data[0];
}
