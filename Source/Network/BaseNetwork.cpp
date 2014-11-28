#include "BaseNetwork.h"

BaseNetwork::BaseNetwork()
{
	m_password = "default";
	m_localAddress = "";

	memset(m_packetData, 0, sizeof(m_packetData));
}

BaseNetwork::~BaseNetwork()
{
	for (int i = 0; i < m_packets.size(); ++i)
	{
		auto packet = m_packets.front();
		SAFE_DELETE(packet);
		m_packets.pop();
	}
}


void BaseNetwork::TriggerEvent(NetMessageHook _function, NetConnection _connection)
{
	//if (_function)
	//	_function(m_packetHandler, _connection);
}

std::function<void(PacketHandler* _packetHandler, Packet* _packet)>* BaseNetwork::GetUserFunction(std::string _functionName)
{
	if (m_userFunctions.find(_functionName) != m_userFunctions.end())
		return &m_userFunctions[_functionName];


	return 0;
}
std::function<void(PacketHandler* _packetHandler, Packet* _packet)>* BaseNetwork::GetNetworkFunction(char _functionIdentifier)
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
		std::string functionName((char*)&_packet->Data[1]);
		if (m_userFunctions.find(functionName) != m_userFunctions.end())
		{
			m_userFunctions[functionName](&m_packetHandler, _packet);
		}

	}
	else
	{
		if (m_networkFunctions.find(type) != m_networkFunctions.end())
		{
			m_networkFunctions[type](&m_packetHandler, _packet);
		}
	}

	SAFE_DELETE(_packet);
}