#include "BaseNetwork.h"

#ifdef WIN32
#else
#include <sys/time.h>
#endif

using namespace Network;

BaseNetwork::BaseNetwork()
{
	m_userFunctions = new std::map < std::string, NetMessageHook >();
	m_networkFunctions = new std::map < char, NetMessageHook >();

	m_systemPackets = new std::queue<Packet*>();
	m_customPackets = new std::queue<Packet*>();
	m_systemPacketLock = new std::mutex();
	m_customPacketLock = new std::mutex();

	m_localAddress = new std::string("");
	m_password = new std::string("default");
	m_incomingPort = new unsigned int();

	m_packetHandler = new PacketHandler();

	m_maxTimeOutIntervall = new float(10.0f);
	m_maxIntervallCounter = new int(3);

	m_totalDataReceived = new float(0);
	m_totalDataSent = new float(0);

	m_currentDataReceived = new float(0);
	m_currentDataSent = new float(0);
	m_usageDataTimer = new float(0);



	memset(m_packetData, 0, sizeof(m_packetData));
}

BaseNetwork::~BaseNetwork()
{
	for (unsigned int i = 0; i < m_customPackets->size(); ++i)
	{
		auto packet = m_customPackets->front();
		SAFE_DELETE(packet);
		m_customPackets->pop();
	}


	for (unsigned int i = 0; i < m_systemPackets->size(); ++i)
	{
		auto packet = m_systemPackets->front();
		SAFE_DELETE(packet);
		m_systemPackets->pop();
	}


	SAFE_DELETE(m_userFunctions);
	SAFE_DELETE(m_networkFunctions);

	SAFE_DELETE(m_systemPackets);
	SAFE_DELETE(m_customPackets);
	SAFE_DELETE(m_systemPacketLock);
	SAFE_DELETE(m_customPacketLock);

	SAFE_DELETE(m_localAddress);
	SAFE_DELETE(m_password);
	SAFE_DELETE(m_incomingPort);

	SAFE_DELETE(m_packetHandler);

	SAFE_DELETE(m_maxTimeOutIntervall);
	SAFE_DELETE(m_maxIntervallCounter);

	SAFE_DELETE(m_totalDataReceived);
	SAFE_DELETE(m_totalDataSent);

	SAFE_DELETE(m_currentDataReceived);
	SAFE_DELETE(m_currentDataSent);
	SAFE_DELETE(m_usageDataTimer);

}

void BaseNetwork::AddNetworkHook(char* _name, NetMessageHook& _hook)
{
	(*m_userFunctions)[_name] = _hook;
	return;
}

void BaseNetwork::TriggerEvent(std::vector<NetEvent>* _event, NetConnection& _connection, const char* _message)
{
	for (int i = 0; i < _event->size(); ++i)
		(*_event)[i](_connection, _message);
}

void BaseNetwork::HandlePacket(Packet* _packet)
{
	if (!_packet)
		return;

	char type = _packet->Data[0];

	if (type == NetTypeMessageId::ID_CUSTOM_PACKET)
	{
		m_customPacketLock->lock();
		m_customPackets->push(_packet);
		m_customPacketLock->unlock();
	}
	else
	{
		m_systemPacketLock->lock();
		m_systemPackets->push(_packet);
		m_systemPacketLock->unlock();
	}
}

int BaseNetwork::PopAndExecutePacket(void)
{

	m_customPacketLock->lock();
	size_t size = m_customPackets->size();
	if (size == 0)
	{
		m_customPacketLock->unlock();
		return (int)size;
	}
	Packet* p = m_customPackets->front();
	m_customPackets->pop();
	m_customPacketLock->unlock();

	size--;

	if (*p->Length <= 1)
	{
		if (NET_DEBUG)
			printf("Corrupt packet, wrong size.\n");
		return (int)size;
	}

	uint64_t id = m_packetHandler->StartUnpack(p);

	if (m_userFunctions->find((char*)&p->Data[1]) != m_userFunctions->end())
	{
		(*m_userFunctions)[(char*)&p->Data[1]](m_packetHandler, id, *p->Sender);
	}
	else if (NET_DEBUG)
		printf("Packet \"%s\" not bound to any function.\n", (char*)&p->Data[1]);

	m_packetHandler->EndUnpack(id);

	return (int)size;
}

void BaseNetwork::Update(float _dt)
{

	UpdateNetUsage(_dt);
	UpdateTimeOut(_dt);

	m_systemPacketLock->lock();
	size_t num_sysPackets = m_systemPackets->size();
	m_systemPacketLock->unlock();
	
	Packet *p;
	for (int i = 0; i < num_sysPackets; ++i)
	{
		m_systemPacketLock->lock();
		p = m_systemPackets->front();
		m_systemPackets->pop();
		m_systemPacketLock->unlock();

		char type = p->Data[0];

		uint64_t id = m_packetHandler->StartUnpack(p);

		if (m_networkFunctions->find(type) != m_networkFunctions->end())
		{
			(*m_networkFunctions)[type](m_packetHandler, id, *p->Sender);
		}

		m_packetHandler->EndUnpack((uint64_t)p);
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