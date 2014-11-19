#include "BaseNetwork.h"

#include <thread>

BaseNetwork::BaseNetwork()
	:m_receiveThreadAlive(false)
{
	m_rakInterface = RakNet::RakPeerInterface::GetInstance();
	m_localAddress = m_rakInterface->GetLocalIP(0);
	m_password = "localhest";
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
}

void BaseNetwork::StopListen()
{
	m_receiveThreadAlive = false;
	m_thread.join();
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
	m_onPlayerConnected = _function;
}

void BaseNetwork::SetOnPlayerDisconnected(NetEvent _function)
{
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

void BaseNetwork::TriggerEvent(NetEvent _function, unsigned char _identifier)
{
	if (_function)
		_function(_identifier);
}