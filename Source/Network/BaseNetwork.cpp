#include "BaseNetwork.h"

#ifdef WIN32
#else
#include <sys/time.h>
#endif

using namespace Network;

BaseNetwork::BaseNetwork()
{
	m_password = "default";
	m_localAddress = "";

	m_maxTimeOutIntervall = 10.0f;
	m_maxIntervallCounter = 3;

	m_totalDataReceived = 0;
	m_totalDataSent = 0;

	m_currentDataReceived = 0;
	m_currentDataSent = 0;

	m_usageDataTimer = 0;

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

void BaseNetwork::AddNetworkHook(char* _name, NetMessageHook _hook)
{
	m_userFunctions[_name] = _hook;
	return;
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
	size_t size = m_customPackets.size();
	if (size == 0)
	{
		m_customPacketLock.unlock();
		return (int)size;
	}
	Packet* p = m_customPackets.front();
	m_customPackets.pop();
	m_customPacketLock.unlock();

	size--;

	if (p->Length <= 1)
	{
		if (NET_DEBUG)
			printf("Corrupt packet, wrong size.\n");
		return (int)size;
	}

	uint64_t id = m_packetHandler.StartUnpack(p);

	if (m_userFunctions.find((char*)&p->Data[1]) != m_userFunctions.end())
	{
		m_userFunctions[(char*)&p->Data[1]](&m_packetHandler, id, p->Sender);
	}
	else if (NET_DEBUG)
		printf("Packet \"%s\" not bound to any function.\n", (char*)&p->Data[1]);

	m_packetHandler.EndUnpack(id);

	return (int)size;
}

void BaseNetwork::Update(float _dt)
{

	UpdateNetUsage(_dt);
	UpdateTimeOut(_dt);

	m_systemPacketLock.lock();
	size_t num_sysPackets = m_systemPackets.size();
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

unsigned int BaseNetwork::GetMillisecondsTime()
{
#ifdef WIN32
	return (unsigned int)GetTickCount();
#else
	struct timeval tv;
	if (gettimeofday(&tv, 0) != 0) return 0;
	return (unsigned int)((tv.tv_sec * 1000ul) + (tv.tv_usec / 1000ul));
#endif
}