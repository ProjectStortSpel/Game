#include "Client.h"
#include <RakNet/MessageIdentifiers.h>

Client::Client()
	:m_threadAlive(false)
{
	m_client = RakNet::RakPeerInterface::GetInstance();
}

Client::~Client()
{
	/*PacketHandler packet;

	packet.StartPack("hest");
	packet.WriteByte('p');
	packet.WriteFloat(0.5f);
	packet.WriteInt(1337);
	packet.WriteString("HEST HEST HEST2");
	auto p = packet.EndPack();

	packet.StartUnPack(p);
	auto c = packet.ReadByte();
	auto f = packet.ReadFloat();
	auto i = packet.ReadInt();
	auto s = packet.ReadString();
	auto fel = packet.ReadFloat();*/

}

void Client::Connect(const char* _ipAddress, const char* _password, const int _port, const int _clientPort)
{
	m_ipAddress = _ipAddress;
	m_password = _password;
	m_port = _port;
	m_clientPort = _clientPort;

	Connect();
}
void Client::Connect()
{
	// Disallow connection responses from any IP.
	// Usable when connecting to a server with multiple IP addresses.
	m_client->AllowConnectionResponseIPMigration(false);

	// Connecting the client is very simple.  0 means we don't care about
	// a connectionValidationInteger, and false for low priority threads

	// Describes the local socket used when connecting.
	// socketFamily describes if we should use IPV4 or IPV6 (AF_INET is IPV4)
	RakNet::SocketDescriptor socketDescriptor(m_clientPort, 0);
	socketDescriptor.socketFamily = AF_INET;

	// Starts the network thread
	m_client->Startup(8, &socketDescriptor, 1);
	// Send an occasional ping to the server to check for response
	m_client->SetOccasionalPing(true);

	// Connect to the server
	RakNet::ConnectionAttemptResult car = m_client->Connect(m_ipAddress.c_str(), m_port, m_password.c_str(), (int)strlen(m_password.c_str()));
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
	if (m_threadAlive)
		StopListen();

	m_client->Shutdown(300);
	RakNet::RakPeerInterface::DestroyInstance(m_client);
}

void Client::Send(PacketHandler::Packet _packet)
{
	m_client->Send((char*)_packet.Data, _packet.Length, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Client::StartListen()
{
	m_threadAlive = true;
	m_thread = std::thread(&Client::Run, this);
}

void Client::StopListen()
{
	m_threadAlive = false;
	m_thread.join();
}

void Client::Run()
{
	while (m_threadAlive)
	{
		NetSleep(30);
		RecivePackets();
	}
}

void Client::RecivePackets()
{
	RakNet::Packet* packet;
	//m_server->DeallocatePacket(m_packet),
	unsigned char packetIdentifier;
	for (packet = m_client->Receive(); packet; packet = m_client->Receive())
	{
		packetIdentifier = GetPacketIdentifier(packet);

		PacketHandler::Packet p;
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
			// Couldn't deliver a reliable packet - i.e. the k system was abnormally
			// terminated
			printf("ID_USER_PACKET from %s\n", packet->systemAddress.ToString(true));
			p.Data = &packet->data[0];

			p.Length = packet->length;

			m_packetLock.lock();
			m_packets.push(p);
			m_packetLock.unlock();

			break;
		default:
			break;
		}


	}
}

PacketHandler::Packet test2;
PacketHandler::Packet* Client::GetPacket()
{
	//RecivePackets();

	PacketHandler::Packet* p = NULL;

	if (!m_packets.empty())
	{
		test2 = m_packets.front();
		p = &test2;
		m_packets.pop();
	}

	return p;
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

void Client::SetOnPlayerConnected(NetEvent _function)
{
	m_onPlayerConnected = _function;
}

void Client::SetOnPlayerDisconnected(NetEvent _function)
{
	m_onPlayerDisconnected = _function;
}

unsigned char Client::GetPacketIdentifier(RakNet::Packet *p)
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

void Client::TriggerEvent(NetEvent _function, unsigned char _identifier)
{
	if (_function)
		_function(_identifier);
}