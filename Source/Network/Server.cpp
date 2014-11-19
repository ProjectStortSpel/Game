#include "Server.h"
#include <RakNet/MessageIdentifiers.h>

Server::Server()
	: BaseNetwork()
{
	m_incomingPort = 5357;
}


Server::~Server()
{
}


void Server::Start()
{
	m_rakInterface->SetIncomingPassword(m_password.c_str(), (int)strlen(m_password.c_str()));
	m_rakInterface->SetTimeoutTime(30000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);

	RakNet::SystemAddress clientID = RakNet::UNASSIGNED_SYSTEM_ADDRESS;


	RakNet::SocketDescriptor socketDescriptors[2];
	socketDescriptors[0].port = m_incomingPort;
	socketDescriptors[0].socketFamily = AF_INET; // Test out IPV4
	socketDescriptors[1].port = m_incomingPort;
	socketDescriptors[1].socketFamily = AF_INET6; // Test out IPV6

	m_rakInterface->SetMaximumIncomingConnections(MAX_PLAYERS);
	
	bool b = m_rakInterface->Startup(MAX_PLAYERS, socketDescriptors, 2) == RakNet::RAKNET_STARTED;
	if (!b)
	{
		b = m_rakInterface->Startup(MAX_PLAYERS, socketDescriptors, 1) == RakNet::RAKNET_STARTED;
		if (!b)
			exit(1);
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

void Server::Broadcast(PacketHandler::Packet _packet, NetConnection* _exclude)
{
	RakNet::SystemAddress exclude = RakNet::UNASSIGNED_SYSTEM_ADDRESS;

	if (_exclude)
	{
		if (m_addressMap.find(*_exclude) != m_addressMap.end())
		{
			exclude = m_addressMap[*_exclude];
		}
	}	

	m_rakInterface->Send((char*)_packet.Data, _packet.Length, HIGH_PRIORITY, RELIABLE_ORDERED, 0, exclude, true);
}

void Server::Send(PacketHandler::Packet _packet, NetConnection _connection)
{
	if (m_addressMap.find(_connection) != m_addressMap.end())
	{
		m_rakInterface->Send((char*)_packet.Data, _packet.Length, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_addressMap[_connection], false);
	}	
}

void Server::RecivePackets()
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
		{
			NetConnection c;
			c = packet->systemAddress.ToString(true, ':');
			m_connections.push_back(c);
			m_connectionMap[packet->systemAddress] = c;
			m_addressMap[c] = packet->systemAddress;
			
			TriggerEvent(m_onPlayerConnected, packetIdentifier, packet->systemAddress);
			break;

		}
			

		case ID_CONNECTION_LOST:
			// User lost connection to server
		case ID_DISCONNECTION_NOTIFICATION:
			// User disconnected from server
		{
			NetConnection c = m_connectionMap[packet->systemAddress];
			m_connectionMap.erase(packet->systemAddress);
			m_addressMap.erase(c);

			m_connections.erase(std::remove(m_connections.begin(), m_connections.end(), c), m_connections.end());

			TriggerEvent(m_onPlayerDisconnected, packetIdentifier, packet->systemAddress);
			break;
		}		

		case ID_INCOMPATIBLE_PROTOCOL_VERSION:
			printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
			break;

		case ID_CONNECTED_PING:
		case ID_UNCONNECTED_PING:
			printf("Ping from %s\n", packet->systemAddress.ToString(true));
			break;

		case ID_USER_PACKET:
		{
			PacketHandler::Packet* p = new PacketHandler::Packet();
			// Couldn't deliver a reliable packet - i.e. the k system was abnormally
			// terminated
			printf("ID_USER_PACKET from %s\n", packet->systemAddress.ToString(true));
			p->Data = new unsigned char[packet->length];
			memcpy(p->Data, &packet->data[0], packet->length);

			p->Length = packet->length;

			if (m_connectionMap.find(packet->systemAddress) != m_connectionMap.end())
				p->Sender = &m_connectionMap[packet->systemAddress];
			else
				p->Sender = NULL;

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