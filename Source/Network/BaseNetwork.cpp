#include "BaseNetwork.h"

#include <thread>
#include "Network/NetTypeMessageID.h"

BaseNetwork::BaseNetwork()
	:m_listenForPacketsThreadAlive(false)
{
#ifdef WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	ISocket::Initialize();
	m_password = "localhest";
	m_packets = std::queue<PacketHandler::Packet*>();
}

BaseNetwork::~BaseNetwork()
{
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

void BaseNetwork::TriggerEvent(NetEvent _function, NetConnection _connection)
{
	if (_function)
		_function(_connection);
}