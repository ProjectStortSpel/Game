#include "BaseNetwork.h"

#include <thread>

BaseNetwork::BaseNetwork()
	:m_receiveThreadAlive(false)
{
	m_rakInterface = RakNet::RakPeerInterface::GetInstance();
	m_localAddress = m_rakInterface->GetLocalIP(0);
	m_password = "localhest";

	m_packets = std::queue<PacketHandler::Packet*>();
}

BaseNetwork::~BaseNetwork()
{
	m_rakInterface->Shutdown(300);
	RakNet::RakPeerInterface::DestroyInstance(m_rakInterface);
}

void BaseNetwork::StartListen()
{
	m_receiveThreadAlive = true;
	m_thread = std::thread(&BaseNetwork::Run, this);

	if (NET_DEBUG)
		printf("Started listen on new thread.\n\n");
}

void BaseNetwork::StopListen()
{
	if (NET_DEBUG)
		printf("Trying to stop listen. Stopping thread.\n");

	m_receiveThreadAlive = false;
	m_thread.join();

	if (NET_DEBUG)
		printf("Thread stopped.\n");
}

void BaseNetwork::Run()
{
	while (m_receiveThreadAlive)
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

void BaseNetwork::SetOnPlayerConnected(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnPlayerConnected.\n");

	m_onPlayerConnected = _function;
}

void BaseNetwork::SetOnPlayerDisconnected(NetEvent _function)
{
	if (NET_DEBUG)
		printf("Hooking function to OnPlayerDisconnected.\n");

	m_onPlayerDisconnected = _function;
}

unsigned char BaseNetwork::GetPacketIdentifier(RakNet::Packet *p)
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

void BaseNetwork::TriggerEvent(NetEvent _function, unsigned char _identifier, RakNet::SystemAddress _address)
{


	if (_function)
		_function(_identifier, &m_connectionMap[_address]);
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