#include "BaseNetwork.h"

BaseNetwork::BaseNetwork()
{
	m_password = "default";
	m_localAddress = "";

	memset(m_packetData, 0, sizeof(m_packetData));
}

BaseNetwork::~BaseNetwork()
{
	for (unsigned int i = 0; i < m_packets.size(); ++i)
	{
		auto packet = m_packets.front();
		SAFE_DELETE(packet);
		m_packets.pop();
	}
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

NetMessageHook* BaseNetwork::GetUserFunction(std::string _functionName)
{
	if (m_userFunctions.find(_functionName) != m_userFunctions.end())
		return &m_userFunctions[_functionName];


	return 0;
}
NetMessageHook* BaseNetwork::GetNetworkFunction(char _functionIdentifier)
{
	if (m_networkFunctions.find(_functionIdentifier) != m_networkFunctions.end())
		return &m_networkFunctions[_functionIdentifier];

	return 0;
}

void BaseNetwork::HandlePacket(Packet* _packet)
{
	if (!_packet)
		return;

	char type = _packet->Data[0];

	if (type == NetTypeMessageId::ID_CUSTOM_PACKET)
	{
		m_packetLock.lock();
		m_packets.push(_packet);
		m_packetLock.unlock();
	}
	else
	{
		uint64_t id = m_packetHandler.StartUnpack(_packet);

		if (m_networkFunctions.find(type) != m_networkFunctions.end())
		{
			m_networkFunctions[type](&m_packetHandler, id, _packet->Sender);
		}

		m_packetHandler.EndUnpack((uint64_t)_packet);
	}
}

int BaseNetwork::TriggerPacket(void)
{
	int size = m_packets.size();

	if (size == 0)
		return size;

	Packet* p = m_packets.front();
	m_packets.pop();
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