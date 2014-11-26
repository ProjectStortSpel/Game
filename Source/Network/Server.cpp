#include "Server.h"
#include <algorithm>

#ifdef WIN32
#else
#include <sys/socket.h>
#endif

Server::Server()
	: BaseNetwork(), m_listenForConnectionsThreadAlive(false)
{
	m_incomingPort = 6112;
	m_maxConnections = 8;


	m_networkFunctionMap[ID_REMOTE_CONNECTION_KICKED] = std::bind(&Server::NetConnectionAccepted, this, std::placeholders::_1, std::placeholders::_2);
}


Server::~Server()
{
	Stop();

	ISocket::Shutdown();
}


void Server::Start()
{
	m_listenSocket = ISocket::CreateISocket(AF_INET, SOCK_STREAM, 0);
	m_listenSocket->Bind(m_incomingPort);


	if (NET_DEBUG)
	{
		printf("Starting server:\n");
		printf("Ip address: \"INSERT IP HERE\"\n");
		printf("Port: \"%i\"\n", m_incomingPort);
		printf("Password: \"%s\"\n", m_password.c_str());
		printf("Max connections: \"%i\"\n", m_maxConnections);
	}

	if (!m_listenSocket->Listen(128))
	{
		printf("ERROR!\n");
	}


	m_listenForConnectionsThreadAlive = true;
	m_newConnectionsThread = std::thread(&Server::ListenForConnections, this);
}

void Server::Stop()
{
	for (int i = 0; i < m_receivePacketsActive.size(); ++i)
		m_receivePacketsActive[i] = false;
	
	for (int i = 0; i < m_receivePacketThreads.size(); ++i)
		m_receivePacketThreads[i].join();

	if (m_listenForConnectionsThreadAlive)
	{
		m_newConnectionsThread.join();
		m_listenForConnectionsThreadAlive = false;
	}

	if (m_listenSocket)
	{
		SAFE_DELETE(m_listenSocket);
	}

	for (auto it = m_connectionClients.begin(); it != m_connectionClients.end(); ++it)
	{
			SAFE_DELETE(it->second);
	}

	m_connectionClients.clear();
	

	m_listenSocket = 0;
}

void Server::Broadcast(Packet _packet, NetConnection* _exclude)
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

void Server::Send(Packet _packet, NetConnection _connection)
{
	//if (m_addressMap.find(_connection) != m_addressMap.end())
	//{
	//	m_rakInterface->Send((char*)_packet.Data, _packet.Length, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_addressMap[_connection], false);
	//}	
}

void Server::ReceivePackets(ISocket* _socket, int _id)
{

	while (m_receivePacketsActive[_id])
	{

		int result = _socket->Recv(m_packetData, MAX_PACKET_SIZE, 0);
		if (result > 0)
		{
			unsigned int packetSize = result;

			if (NET_DEBUG)
				printf("Recieved message with length from server with length: %i.\n", packetSize);

			Packet* p = new Packet();
			p->Data = new unsigned char[packetSize];
			p->Length = packetSize;
			p->Sender = _socket->GetNetConnection();
			memcpy(p->Data, m_packetData, packetSize);

			m_packetLock.lock();
			m_packets.push(p);
			m_packetLock.unlock();

		}
		else if (result == 0)
		{
			// Connection shut down graceful
		}
		else
		{
			//if (NET_DEBUG)
			//	printf("ReveivePackets failed. Error Code: %d\n.", WSAGetLastError());
		}
	}

	if (NET_DEBUG)
		printf("Thread stopped!\n");

}

void Server::ListenForConnections()
{
	if(NET_DEBUG)
		printf("Start listen for incoming connections.\n");

	while (m_listenForConnectionsThreadAlive)
	{
		ISocket* newConnection = m_listenSocket->Accept();
		if (!newConnection)
			continue;

		NetConnection nc;
		nc.IpAddress = newConnection->GetRemoteIpAddress();
		nc.Port		 = newConnection->GetRemotePort();

		m_connectionClients[nc] = newConnection;

		if (NET_DEBUG)
			printf("New incoming connection from %s:%d\n", nc.IpAddress.c_str(), nc.Port);

		m_receivePacketsActive.push_back(true);
		m_receivePacketThreads.push_back(std::thread(&Server::ReceivePackets, this, newConnection, m_receivePacketThreads.size()));

		char* hest = new char[10];
		//NetSleep(5000);
		newConnection->Recv(hest, 10, 0);

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

void Server::NetConnectionAccepted(PacketHandler* _packetHandler, NetConnection _netConnection)
{
	printf("PEW PEW PEW PEWP EWP PEWPEWPE PEWP EWPEWP EPEWPEWPE PEWPEW PEWPE PEW!!!!!11!!!!!!!1!!oneeoneoneoneoneoen!!1!!!!1!!1.\n");
}