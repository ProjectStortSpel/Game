#include "BaseNetwork.h"

#include <thread>
#include "Network/NetTypeMessageID.h"

BaseNetwork::BaseNetwork()
	:m_listenForPacketsThreadAlive(false)
{
#ifdef WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	m_listenSocket = ISocket::CreateISocket(AF_INET, SOCK_STREAM, 0);
	m_password = "localhest";
	m_packets = std::queue<PacketHandler::Packet*>();
}

BaseNetwork::~BaseNetwork()
{
	if (m_listenSocket)
	{
		// Shutdown
		m_listenSocket = 0;
	}

	StopListenForPackets();
}

void BaseNetwork::StartListenForPackets()
{
	m_listenForPacketsThreadAlive = true;
	m_receivePacketThread = std::thread(&BaseNetwork::ListenForPackets, this);

	if (NET_DEBUG)
		printf("Started listen on new thread.\n\n");
}

void BaseNetwork::StopListenForPackets()
{
	if (NET_DEBUG)
		printf("Trying to stop listen. Stopping thread.\n");
	if (m_listenForPacketsThreadAlive)
	{
		m_listenForPacketsThreadAlive = false;
		m_receivePacketThread.join();
	}
	if (NET_DEBUG)
		printf("Thread stopped.\n");
}

void BaseNetwork::ListenForPackets()
{
	while (m_listenForPacketsThreadAlive)
	{
		NetSleep(30);
		ReceivePackets();
	}
}

PacketHandler::Packet* BaseNetwork::GetPacket()
{
	PacketHandler::Packet* p = 0;

	if (!m_packets.empty())
	{
		p = m_packets.front();
		m_packets.pop();
	}

	return p;
}

//unsigned char BaseNetwork::GetPacketIdentifier(RakNet::Packet *p)
//{
//	if (p == 0)
//		return 255;
//
//	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
//	{
//		RakAssert(p->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
//		return (unsigned char)p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
//	}
//	else
//		return (unsigned char)p->data[0];
//}

void BaseNetwork::TriggerEvent(NetEvent _function, unsigned char _identifier)// , RakNet::SystemAddress _address)
{

	if (_function)
	{
	//	_function(_identifier, &m_connectionMap[_address]);
	}
	else if (NET_DEBUG)
	{
		printf("Event ");

		switch (_identifier)
		{
			case ID_NEW_INCOMING_CONNECTION:
				printf("\"ID_NEW_INCOMING_CONNECTION\" not bound\n");
				break;
			case ID_CONNECTION_LOST:
				printf("\"ID_CONNECTION_LOST\" not bound\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				printf("\"ID_DISCONNECTION_NOTIFICATION\" not bound\n");
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				printf("\"ID_CONNECTION_REQUEST_ACCEPTED\" not bound\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("\"ID_REMOTE_NEW_INCOMING_CONNECTION\" not bound\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("\"ID_REMOTE_CONNECTION_LOST\" not bound\n");
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("\"ID_REMOTE_DISCONNECTION_NOTIFICATION\" not bound\n");
				break;
			case ID_ALREADY_CONNECTED:
				printf("\"ID_ALREADY_CONNECTED\" not bound\n");
				break;
			case ID_CONNECTION_BANNED:
				printf("\"ID_CONNECTION_BANNED\" not bound\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("\"ID_NO_FREE_INCOMING_CONNECTIONS\" not bound\n");
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				printf("\"ID_CONNECTION_ATTEMPT_FAILED\" not bound\n");
				break;
			case ID_INVALID_PASSWORD:
				printf("\"ID_INVALID_PASSWORD\" not bound\n");
				break;
			default:
				printf("\"UNHANDLED EVENT\" not bound\n");
				break;
		}
	}
}