#include "BaseNetwork.h"

#include <thread>
#include "Network/NetTypeMessageID.h"


BaseNetwork::BaseNetwork()
{
#ifdef WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	ISocket::Initialize();
	m_password = "localhest";
	m_packets = std::queue<Packet*>();
}

BaseNetwork::~BaseNetwork()
{
}

Packet* BaseNetwork::GetPacket()
{
	Packet* p = 0;

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

NetMessageHook* BaseNetwork::GetNetworkFunction(NetTypeMessageId _function)
{
	if (m_networkFunctionMap.find(_function) != m_networkFunctionMap.end())
	{
		return &m_networkFunctionMap[_function];
	}

	return 0;
}


void BaseNetwork::TriggerEvent(NetEvent _function, NetConnection _connection)
{
	if (_function)
		_function(_connection);
}