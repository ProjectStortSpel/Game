#include "BaseNetwork.h"

BaseNetwork::BaseNetwork()
{
	m_password = "default";
	m_localAddress = "";

	memset(m_packetData, 0, sizeof(m_packetData));
}

BaseNetwork::~BaseNetwork()
{
	for (unsigned int i = 0; i < m_customPackets.size(); ++i)
	{
		auto packet = m_customPackets.front();
		SAFE_DELETE(packet);
		m_customPackets.pop();
	}


	for (unsigned int i = 0; i < m_systemPackets.size(); ++i)
	{
		auto packet = m_systemPackets.front();
		SAFE_DELETE(packet);
		m_systemPackets.pop();
	}
}

void BaseNetwork::AddNetworkHook(std::string _name, NetMessageHook _hook)
{
	m_userFunctions[_name] = _hook;
}

void BaseNetwork::TriggerEvent(NetMessageHook _function, uint64_t _packetId, NetConnection _connection)
{
	if (_function)
	{
		_function(&m_packetHandler, _packetId, _connection);
		m_packetHandler.EndUnpack(_packetId);
	}
}

void BaseNetwork::TriggerEvent(NetEvent _function, NetConnection _connection)
{
	if (_function)
	{
		_function(_connection);
	}
}

//NetMessageHook* BaseNetwork::GetUserFunction(std::string _functionName)
//{
//	if (m_userFunctions.find(_functionName) != m_userFunctions.end())
//		return &m_userFunctions[_functionName];
//
//
//	return 0;
//}
//NetMessageHook* BaseNetwork::GetNetworkFunction(char _functionIdentifier)
//{
//	if (m_networkFunctions.find(_functionIdentifier) != m_networkFunctions.end())
//		return &m_networkFunctions[_functionIdentifier];
//
//	return 0;
//}

void BaseNetwork::HandlePacket(Packet* _packet)
{
	if (!_packet)
		return;

	char type = _packet->Data[0];

	if (type == NetTypeMessageId::ID_CUSTOM_PACKET)
	{
		m_customPacketLock.lock();
		m_customPackets.push(_packet);
		m_customPacketLock.unlock();
	}
	else
	{
		m_systemPacketLock.lock();
		m_systemPackets.push(_packet);
		m_systemPacketLock.unlock();
	}
}

int BaseNetwork::TriggerPacket(void)
{

	m_customPacketLock.lock();
	int size = m_customPackets.size();
	if (size == 0)
	{
		m_customPacketLock.unlock();
		return size;
	}
	Packet* p = m_customPackets.front();
	m_customPackets.pop();
	m_customPacketLock.unlock();

	size--;

	if (p->Length <= 1)
	{
		if (NET_DEBUG)
			printf("Corrupt packet, wrong size.\n");
		return size;
	}

	uint64_t id = m_packetHandler.StartUnpack(p);

	std::string functionName((char*)&p->Data[1]);
	if (m_userFunctions.find(functionName) != m_userFunctions.end())
	{
		m_userFunctions[functionName](&m_packetHandler, id, p->Sender);
	}
	else if (NET_DEBUG)
		printf("Packet \"%s\" not bound to any function.\n", functionName.c_str());

	m_packetHandler.EndUnpack(id);

	return size;
}

void BaseNetwork::Update(void)
{

	m_systemPacketLock.lock();
	int num_sysPackets = m_systemPackets.size();
	m_systemPacketLock.unlock();
	
	Packet *p;
	for (int i = 0; i < num_sysPackets; ++i)
	{
		m_systemPacketLock.lock();
		p = m_systemPackets.front();
		m_systemPackets.pop();
		m_systemPacketLock.unlock();

		char type = p->Data[0];

		uint64_t id = m_packetHandler.StartUnpack(p);

		if (m_networkFunctions.find(type) != m_networkFunctions.end())
		{
			m_networkFunctions[type](&m_packetHandler, id, p->Sender);
		}

		m_packetHandler.EndUnpack((uint64_t)p);
	}
}