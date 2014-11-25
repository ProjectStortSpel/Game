#include "Server.h"
#include <RakNet/MessageIdentifiers.h>
#include <algorithm>

Server::Server()
	: BaseNetwork(), m_listenForConnectionsThreadAlive(false)
{
	m_incomingPort = 5357;
	m_maxConnections = 8;
}


Server::~Server()
{
}


void Server::Start()
{
	m_listenSocket->Bind(m_incomingPort);


	if (NET_DEBUG)
	{
		printf("Starting server:\n");
		printf("Ip address: \"INSERT IP HERE\"\n");
		printf("Port: \"%i\"\n", m_incomingPort);
		printf("Password: \"%s\"\n", m_password.c_str());
		printf("Max connections: \"%i\"\n", m_maxConnections);
	}

	if (!m_listenSocket->Listen(SOMAXCONN))
	{
		printf("ERROR!\n");
	}


	m_listenForConnectionsThreadAlive = true;
	m_newConnectionsThread = std::thread(&Server::ListenForConnections, this);
}

void Server::Stop()
{
	if (m_listenForPacketsThreadAlive)
		StopListenForPackets();

	if (m_listenForConnectionsThreadAlive)
	{
		m_newConnectionsThread.join();
		m_listenForConnectionsThreadAlive = false;
	}

	//Shutdown
	m_listenSocket = 0;
}

void Server::Broadcast(PacketHandler::Packet _packet, NetConnection* _exclude)
{
	//RakNet::SystemAddress exclude = RakNet::UNASSIGNED_SYSTEM_ADDRESS;

	//if (_exclude)
	//{
	//	if (m_addressMap.find(*_exclude) != m_addressMap.end())
	//	{
	//		exclude = m_addressMap[*_exclude];
	//	}
	//}	

	//m_rakInterface->Send((char*)_packet.Data, _packet.Length, HIGH_PRIORITY, RELIABLE_ORDERED, 0, exclude, true);
}

void Server::Send(PacketHandler::Packet _packet, NetConnection _connection)
{
	//if (m_addressMap.find(_connection) != m_addressMap.end())
	//{
	//	m_rakInterface->Send((char*)_packet.Data, _packet.Length, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_addressMap[_connection], false);
	//}	
}

void Server::ReceivePackets()
{
	//RakNet::Packet* packet;
	////m_server->DeallocatePacket(m_packet),
	//unsigned char packetIdentifier;
	//for (packet = m_rakInterface->Receive(); packet; packet = m_rakInterface->Receive())
	//{
	//	packetIdentifier = GetPacketIdentifier(packet);

	//	switch (packetIdentifier)
	//	{
	//	case ID_NEW_INCOMING_CONNECTION:
	//		// User connected to server
	//	{
	//		if (NET_DEBUG)
	//			printf("New client connected from %s.\n", packet->systemAddress.ToString(true));

	//		NetConnection c;
	//		c = packet->systemAddress.ToString(true, ':');
	//		m_connections.push_back(c);
	//		m_connectionMap[packet->systemAddress] = c;
	//		m_addressMap[c] = packet->systemAddress;
	//		
	//		TriggerEvent(m_onPlayerConnected, packetIdentifier, packet->systemAddress);
	//		break;

	//	}
	//		

	//	case ID_CONNECTION_LOST:
	//		// User lost connection to server
	//		if (NET_DEBUG)
	//			printf("Client lost connection to server.\n");
	//		{
	//			NetConnection c = m_connectionMap[packet->systemAddress];
	//			m_connectionMap.erase(packet->systemAddress);
	//			m_addressMap.erase(c);

	//			//m_connections.erase(std::remove(m_connections.begin(), m_connections.end(), c), m_connections.end());
	//			m_connections.erase(std::remove<std::vector<NetConnection>::iterator, NetConnection>(m_connections.begin(), m_connections.end(), c), m_connections.end());
	//			//m_connections.erase(std::find(m_connections.begin(), m_connections.end(), c));
	//			TriggerEvent(m_onPlayerDisconnected, packetIdentifier, packet->systemAddress);
	//			break;
	//		}
	//		break;
	//	case ID_DISCONNECTION_NOTIFICATION:
	//		if (NET_DEBUG)
	//			printf("Client disconnected from server.\n");
	//		// User disconnected from server
	//	{
	//		NetConnection c = m_connectionMap[packet->systemAddress];
	//		m_connectionMap.erase(packet->systemAddress);
	//		m_addressMap.erase(c);
	//		
	//		//m_connections.erase(std::remove(m_connections.begin(), m_connections.end(), c), m_connections.end());
	//		
	//		m_connections.erase(std::remove<std::vector<NetConnection>::iterator, NetConnection>(m_connections.begin(), m_connections.end(), c), m_connections.end());
	//		//m_connections.erase(std::find(m_connections.begin(), m_connections.end(), c));
	//		TriggerEvent(m_onPlayerDisconnected, packetIdentifier, packet->systemAddress);
	//		break;
	//	}		

	//	case ID_INCOMPATIBLE_PROTOCOL_VERSION:
	//		if (NET_DEBUG)
	//			printf("Incompatible protocl version - Server.\n");
	//		break;

	//	case ID_CONNECTED_PING:
	//	case ID_UNCONNECTED_PING:
	//		if (NET_DEBUG)
	//			printf("Ping from %s\n", packet->systemAddress.ToString(true));
	//		break;

	//	case ID_USER_PACKET:
	//	{
	//		if (NET_DEBUG)
	//			printf("Recieved user message from \"%s\"\n", packet->systemAddress.ToString(true));

	//		PacketHandler::Packet* p = new PacketHandler::Packet();
	//		p->Data = new unsigned char[packet->length];
	//		memcpy(p->Data, &packet->data[0], packet->length);

	//		p->Length = packet->length;

	//		if (m_connectionMap.find(packet->systemAddress) != m_connectionMap.end())
	//			p->Sender = &m_connectionMap[packet->systemAddress];
	//		else
	//			p->Sender = NULL;

	//		m_packetLock.lock();
	//		m_packets.push(p);
	//		m_packetLock.unlock();

	//		break;
	//	}
	//	default:
	//		break;
	//	}
	//}
}


void Server::ListenForConnections()
{
	while (m_listenForConnectionsThreadAlive)
	{
		ISocket* newConnection = m_listenSocket->Accept();
		if (!newConnection)
			continue;

		if (m_connectedClients.size() < m_maxConnections)
			m_connectedClients.emplace_back(newConnection);
		else
			printf("KILL CONNECTION!\n");
	}
}

void Server::KickClient(NetConnection* _connection, const char* _reason)
{
	//std::string address = *_connection;
	//if (m_addressMap.find(address) != m_addressMap.end())
	//{

	//	if (NET_DEBUG)
	//		printf("%s was kicked from the server. Reason: %s.\n", address.c_str(), _reason);

	//	char* packet = new char[16];

	//	packet[0] = ID_USER_PACKET;
	//	packet[1] = 'K';
	//	packet[2] = 'i';
	//	packet[3] = 'c';
	//	packet[4] = 'k';
	//	packet[5] = 'C';
	//	packet[6] = 'l';
	//	packet[7] = 'i';
	//	packet[8] = 'e';
	//	packet[9] = 'n';
	//	packet[10] = 't';
	//	packet[11] = '\0';
	//	packet[12] = '1';
	//	packet[13] = '2';
	//	packet[14] = '3';
	//	packet[15] = '\0';

	//	m_rakInterface->Send(packet, 16, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_addressMap[address], false);
	//	m_rakInterface->CloseConnection(m_addressMap[address], true);
	//}
	//else
	//{
	//	if (NET_DEBUG)
	//		printf("%s is not connected the the server.\n", address.c_str());
	//}
}

void Server::BanClient(NetConnection* _connection, const char* _reason, unsigned int _milliseconds)
{
	//std::string address = *_connection;

	//if (NET_DEBUG)
	//	printf("%s added to the banlist for %i milliseconds.\n", address.c_str(), _milliseconds);

	//m_rakInterface->AddToBanList(address.c_str(), _milliseconds);
}

void Server::UnbanClient(NetConnection* _connection)
{
	//std::string address = *_connection;

	//if (m_rakInterface->IsBanned(address.c_str()))
	//{
	//	if (NET_DEBUG)
	//		printf("%s was removed from the banlist.\n", address.c_str());

	//	m_rakInterface->RemoveFromBanList(address.c_str());
	//}
	//else
	//{
	//	if (NET_DEBUG)
	//		printf("%s is not on the banlist.\n", address.c_str());
	//}
}

void Server::SetOnPlayerConnected(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnPlayerConnected.\n");

	m_onPlayerConnected = _function;
}

void Server::SetOnPlayerDisconnected(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnPlayerDisconnected.\n");

	m_onPlayerDisconnected = _function;
}

void Server::SetOnPlayerTimedOut(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnPlayerTimedOut.\n");

	m_onPlayerTimedOut = _function;
}