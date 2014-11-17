#include "Client.h"
#include <RakNet/MessageIdentifiers.h>



Client::Client()
{
	m_client = RakNet::RakPeerInterface::GetInstance();
}


Client::~Client()
{
}


void Client::Connect()
{

	m_client->AllowConnectionResponseIPMigration(false);

	// Connecting the client is very simple.  0 means we don't care about
	// a connectionValidationInteger, and false for low priority threads
	RakNet::SocketDescriptor socketDescriptor(m_clientPort, 0);
	socketDescriptor.socketFamily = AF_INET;
	m_client->Startup(8, &socketDescriptor, 1);
	m_client->SetOccasionalPing(true);

	RakNet::ConnectionAttemptResult car = m_client->Connect(m_ipAddress.c_str(), m_port, m_password.c_str(), (int)strlen(m_password.c_str()));
	
	printf("\nMy IP addresses:\n");
	unsigned int i;
	for (i = 0; i < m_client->GetNumberOfAddresses(); i++)
	{
		printf("%i. %s\n", i + 1, m_client->GetLocalIP(i));
	}

	printf("My GUID is %s\n", m_client->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS).ToString());

	m_thread = std::thread(&Client::Run, this);

}

void Client::Disconect()
{
}

void Client::Run()
{
	while (1)
	{
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

		switch (packetIdentifier)
		{
		case ID_DISCONNECTION_NOTIFICATION:
			// Connection lost normally
			printf("ID_DISCONNECTION_NOTIFICATION\n");
			break;
		case ID_ALREADY_CONNECTED:
			// Connection lost normally
			printf("ID_ALREADY_CONNECTED with guid %" PRINTF_64_BIT_MODIFIER "u\n", packet->guid);
			break;
		case ID_INCOMPATIBLE_PROTOCOL_VERSION:
			printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
			break;
		case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
			printf("ID_REMOTE_DISCONNECTION_NOTIFICATION\n");
			break;
		case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
			printf("ID_REMOTE_CONNECTION_LOST\n");
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
			printf("ID_REMOTE_NEW_INCOMING_CONNECTION\n");
			break;
		case ID_CONNECTION_BANNED: // Banned from this server
			printf("We are banned from this server.\n");
			break;
		case ID_CONNECTION_ATTEMPT_FAILED:
			printf("Connection attempt failed\n");
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			// Sorry, the server is full.  I don't do anything here but
			// A real app should tell the user
			printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
			break;

		case ID_INVALID_PASSWORD:
			printf("ID_INVALID_PASSWORD\n");
			break;

		case ID_CONNECTION_LOST:
			// Couldn't deliver a reliable packet - i.e. the other system was abnormally
			// terminated
			printf("ID_CONNECTION_LOST\n");
			break;

		case ID_CONNECTION_REQUEST_ACCEPTED:
			// This tells the client they have connected
			printf("ID_CONNECTION_REQUEST_ACCEPTED to %s with GUID %s\n", packet->systemAddress.ToString(true), packet->guid.ToString());
			printf("My external address is %s\n", m_client->GetExternalID(packet->systemAddress).ToString(true));
			break;
		case ID_CONNECTED_PING:
		case ID_UNCONNECTED_PING:
			printf("Ping from %s\n", packet->systemAddress.ToString(true));
			break;
		default:
			m_packetLock.lock();
			m_packets.push(packet);
			m_packetLock.unlock();
		}


	}
}

RakNet::Packet* Client::GetPacket()
{
	//RecivePackets();

	RakNet::Packet* p = NULL;

	if (!m_packets.empty())
	{
		RakNet::Packet* p = m_packets.front();
		m_client->DeallocatePacket(m_packets.front());
		m_packets.pop();
	}

	return p;
}

void Client::SetOnUserConnect(std::function<void()> _function)
{
	m_onUserConnect = _function;
}

void Client::SetOnUserDisconnect(std::function<void()> _function)
{
	m_onUserDisconnect = _function;
}

void Client::SetOnUserTimeOut(std::function<void()> _function)
{
	m_onUserTimeOut = _function;
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
