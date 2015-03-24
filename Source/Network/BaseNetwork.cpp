#include "BaseNetwork.h"
#include "NetTypeMessageID.h"

#ifdef WIN32
#else
#include <sys/time.h>
#endif

using namespace Network;

BaseNetwork::BaseNetwork()
{
	m_packetHandler = new PacketHandler();

	m_localAddress = new std::string("");
	m_password = new std::string("default");

	m_incomingPort = new unsigned int(6112);

	m_totalDataReceived = new int(0);
	m_totalDataSent = new int(0);

	m_currentDataReceived = new int(0);
	m_currentDataSent = new int(0);

	m_usageDataTimer = new float(0.f);

	/* CHANGE THESE NUMBERS */
	/*	maxTimeOutIntervall is the number between each PING message
		maxIntervallCounter is the number of times the system is allowed to miss
		when the maxIntervallCounter is reached the player will be counted as disconnected (timed out)
	*/
	m_maxTimeOutIntervall = new float(5); // Indicator of the time between each PING message is sent
	m_maxIntervallCounter = new int(2);	  // Counter for how many max allowed misses (if you didn't receive any PONG for X times)

	m_customPackets = new std::queue<Packet*>();
	m_systemPackets = new std::queue<Packet*>();

	m_systemFunctions = new std::map < char, NetMessageHook >();
	m_customFunctions = new std::map < std::string, NetMessageHook >();

	m_customPacketLock = SDL_CreateMutex();
	m_systemPacketLock = SDL_CreateMutex();
}

BaseNetwork::~BaseNetwork()
{
	SAFE_DELETE(m_packetHandler);

	SAFE_DELETE(m_localAddress);
	SAFE_DELETE(m_password);

	SAFE_DELETE(m_incomingPort);

	SAFE_DELETE(m_totalDataReceived);
	SAFE_DELETE(m_totalDataSent);

	SAFE_DELETE(m_currentDataReceived);
	SAFE_DELETE(m_currentDataSent);

	SAFE_DELETE(m_usageDataTimer);

	SAFE_DELETE(m_maxTimeOutIntervall);
	SAFE_DELETE(m_maxIntervallCounter);

	if (SDL_LockMutex(m_customPacketLock) == 0)
	{
		for (unsigned int i = 0; i < m_customPackets->size(); ++i)
		{
			auto packet = m_customPackets->front();
			SAFE_DELETE(packet);
			m_customPackets->pop();
		}
		
		SAFE_DELETE(m_customPackets);

		SDL_UnlockMutex(m_customPacketLock);

	}
	SDL_DestroyMutex(m_customPacketLock);
	
	if (SDL_LockMutex(m_systemPacketLock) == 0)
	{
		for (unsigned int i = 0; i < m_systemPackets->size(); ++i)
		{
			auto packet = m_systemPackets->front();
			SAFE_DELETE(packet);
			m_systemPackets->pop();
		}

		SAFE_DELETE(m_systemPackets);
		SDL_UnlockMutex(m_systemPacketLock);
	}
	SDL_DestroyMutex(m_systemPacketLock);


	m_systemFunctions->clear();
	m_customFunctions->clear();

	SAFE_DELETE(m_systemFunctions);
	SAFE_DELETE(m_customFunctions);

}

void BaseNetwork::AddNetworkHook(const char* _name, NetMessageHook& _hook)
{
	(*m_customFunctions)[_name] = _hook;
	if (NET_DEBUG > 0)
		DebugLog("Custom function \'%s\' bound.", LogSeverity::Info, _name);
}

void BaseNetwork::Clear()
{
	if (SDL_LockMutex(m_customPacketLock) == 0)
	{
		for (unsigned int i = 0; i < m_customPackets->size(); ++i)
		{
			auto packet = m_customPackets->front();
			SAFE_DELETE(packet);
			m_customPackets->pop();
		}
		SDL_UnlockMutex(m_customPacketLock);

		if (NET_DEBUG > 0)
			DebugLog("Cleared custompackets.", LogSeverity::Info);

	}
	else if (NET_DEBUG > 0)
		DebugLog("Failed to lock custom packet. Error: %s", LogSeverity::Error, SDL_GetError());

	if (SDL_LockMutex(m_systemPacketLock) == 0)
	{
		for (unsigned int i = 0; i < m_systemPackets->size(); ++i)
		{
			auto packet = m_systemPackets->front();
			SAFE_DELETE(packet);
			m_systemPackets->pop();
		}
		SDL_UnlockMutex(m_systemPacketLock);

		if (NET_DEBUG > 0)
			DebugLog("Cleared systempackets.", LogSeverity::Info);

	}
	else if (NET_DEBUG > 0)
		DebugLog("Failed to lock system packet. Error: %s", LogSeverity::Error, SDL_GetError());

	m_customFunctions->clear();

	if (NET_DEBUG > 0)
		DebugLog("Cleared custom functions.", LogSeverity::Info);
}

void BaseNetwork::Update(float _dt)
{
	UpdateNetUsage(_dt);
	UpdateTimeOut(_dt);

	
	size_t noSysPackets = 0;
	if (SDL_LockMutex(m_systemPacketLock) == 0)
	{
		noSysPackets = m_systemPackets->size();
		SDL_UnlockMutex(m_systemPacketLock);
	}
	else if(NET_DEBUG > 0)
		DebugLog("Failed to lock system packet lock. Error: %s.", LogSeverity::Error, SDL_GetError());


	Packet* p = 0;
	for (unsigned int i = 0; i < noSysPackets; ++i)
	{
		if (SDL_LockMutex(m_systemPacketLock) == 0)
		{
			p = m_systemPackets->front();
			m_systemPackets->pop();
			SDL_UnlockMutex(m_systemPacketLock);
		}
		else if (NET_DEBUG > 0)
			DebugLog("Failed to lock systemPacket. Error: %s", LogSeverity::Error, SDL_GetError());

		uint64_t id = m_packetHandler->StartUnpack(p);
		char type = m_packetHandler->ReadByte(id);

		if (m_systemFunctions->find(type) != m_systemFunctions->end())
			(*m_systemFunctions)[type](m_packetHandler, id, *p->Sender);
		else if (NET_DEBUG > 0)
		{
			NetTypeMessageId tmp = (NetTypeMessageId)type;
			DebugLog("System packet \'%s\' not bound to any function", LogSeverity::Warning, m_enumStrings[type].c_str());
		}

		SAFE_DELETE(p);
		m_packetHandler->EndUnpack(id);
	}

}

void BaseNetwork::HandlePacket(Packet* _packet)
{
	if (!_packet)
		return;

	char systemType = _packet->Data[0];

	if (systemType == NetTypeMessageId::ID_CUSTOM_PACKET)
	{
		if (SDL_LockMutex(m_customPacketLock) == 0)
		{
			m_customPackets->push(_packet);
			SDL_UnlockMutex(m_customPacketLock);
		}
		else if (NET_DEBUG > 0)
			DebugLog("Failed to lock custom packet lock. Error: %s.", LogSeverity::Error, SDL_GetError());
	}
	else
	{
		if (SDL_LockMutex(m_systemPacketLock) == 0)
		{
			m_systemPackets->push(_packet);
			SDL_UnlockMutex(m_systemPacketLock);
		}
		else if (NET_DEBUG > 0)
			DebugLog("Failed to lock custom packet lock. Error: %s.", LogSeverity::Error, SDL_GetError());
	}
}


int BaseNetwork::PopAndExecutePacket(void)
{
	if (SDL_LockMutex(m_customPacketLock) == 0)
	{
		size_t noCustomPackets = m_customPackets->size();
		if (noCustomPackets == 0)
		{
			SDL_UnlockMutex(m_customPacketLock);
			return 0;
		}

		Packet* p = m_customPackets->front();
		m_customPackets->pop();
		SDL_UnlockMutex(m_customPacketLock);
		noCustomPackets--;

		if (*p->Length <= 1)
		{
			if (NET_DEBUG > 0)
				DebugLog("Corrupt custom packet. Invalid size %d.", LogSeverity::Error, *p->Length);

			SAFE_DELETE(p);
			return noCustomPackets;
		}

		uint64_t id = m_packetHandler->StartUnpack(p);
		m_packetHandler->ReadByte(id);
		char* functionName = m_packetHandler->ReadString(id);

		if (m_customFunctions->find(functionName) != m_customFunctions->end())
			(*m_customFunctions)[functionName](m_packetHandler, id, *p->Sender);
		else if (NET_DEBUG > 0)
			DebugLog("Custom packet not bound to any function. Expected: %s.", LogSeverity::Warning, functionName);

		m_packetHandler->EndUnpack(id);

		SAFE_DELETE(p);
		return noCustomPackets;
	}
	else if(NET_DEBUG > 0)
		DebugLog("Failed to lock custom packet mutex. Error: %s.", LogSeverity::Error, SDL_GetError());

	return -1;
}

void BaseNetwork::TriggerEvent(std::vector<NetEvent>* _event, NetConnection& _connection, const char* _message)
{
	for (int i = 0; i < _event->size(); ++i)
		(*_event)[i](_connection, _message);
}

unsigned int BaseNetwork::GetMillisecondsTime(void)
{
#ifdef WIN32
	return (unsigned int)GetTickCount();
#else
	struct timeval tv;
	if (gettimeofday(&tv, 0) != 0) return 0;
	return (unsigned int)((tv.tv_sec * 1000ul) + (tv.tv_usec / 1000ul));
#endif
}