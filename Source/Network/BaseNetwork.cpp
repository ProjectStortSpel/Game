#include "Network/BaseNetwork.h"

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