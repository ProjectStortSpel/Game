#include "PacketHandler.h"
#include "NetTypeMessageID.h"
#include "BaseNetwork.h"

#ifdef WIN32
#else
#include <arpa/inet.h>
#endif

using namespace Network;

PacketHandler::PacketHandler()
{
	m_packetSendInfoMap		= new std::map<uint64_t, PacketSendInfo*>();
	m_packetReceiveInfoMap	= new std::map<uint64_t, PacketReceiveInfo*>();

	m_sendLock		= SDL_CreateMutex();
	m_receiveLock	= SDL_CreateMutex();

}

PacketHandler::~PacketHandler()
{
	if (SDL_LockMutex(m_sendLock) == 0)
	{
		for (auto iterator = m_packetSendInfoMap->begin(); iterator != m_packetSendInfoMap->end(); ++iterator)
			SAFE_DELETE(iterator->second);

		SAFE_DELETE(m_packetSendInfoMap);

		SDL_UnlockMutex(m_sendLock);
	}
	else if (NET_DEBUG > 0)
		DebugLog("Failed to lock send mutex. Error: %s.", LogSeverity::Error, SDL_GetError());
	SDL_DestroyMutex(m_sendLock);



	if (SDL_LockMutex(m_receiveLock) == 0)
	{
		for (auto iterator = m_packetReceiveInfoMap->begin(); iterator != m_packetReceiveInfoMap->end(); ++iterator)
		{
			SAFE_DELETE(iterator->second->PacketData);
			SAFE_DELETE(iterator->second);
		}

		SAFE_DELETE(m_packetReceiveInfoMap);

		SDL_UnlockMutex(m_receiveLock);
	}
	else if (NET_DEBUG > 0)
		DebugLog("Failed to lock receive mutex. Error: %s.", LogSeverity::Error, SDL_GetError());
	SDL_DestroyMutex(m_receiveLock);
}

PacketHandler::PacketSendInfo* PacketHandler::GetPacketSendInfo(uint64_t _id)
{
	PacketHandler::PacketSendInfo* result = 0;

	if (SDL_LockMutex(m_sendLock) == 0)
	{

		if ( m_packetSendInfoMap->find( _id ) != m_packetSendInfoMap->end( ) )
		{
			result = m_packetSendInfoMap->at( _id );
		}
		SDL_UnlockMutex(m_sendLock);

	}
	else if (NET_DEBUG > 0)
		DebugLog("Failed to lock send mutex. Error: %s", LogSeverity::Error, SDL_GetError());

	return result;
}

PacketHandler::PacketReceiveInfo* PacketHandler::GetPacketReceiveInfo(uint64_t _id)
{
	PacketHandler::PacketReceiveInfo* result = 0;

	if (SDL_LockMutex(m_receiveLock) == 0)
	{

		if (m_packetReceiveInfoMap->find(_id) != m_packetReceiveInfoMap->end())
			result = m_packetReceiveInfoMap->at(_id);
		SDL_UnlockMutex(m_receiveLock);

	}
	else if (NET_DEBUG > 0)
		DebugLog("Failed to lock receive mutex. Error: %s", LogSeverity::Error, SDL_GetError());

	return result;

}

char PacketHandler::GetNetTypeMessageId(Packet* _packet)
{
	return _packet->Data[0];
}

char PacketHandler::GetNetTypeMessageId(uint64_t _id)
{
	PacketReceiveInfo* pri = GetPacketReceiveInfo(_id);

	if (pri)
		return pri->PacketData->Data[0];
	else if(NET_DEBUG > 0)
		DebugLog("Tried to get NetTypeMessageId from invalid packet id.", LogSeverity::Error);

	return -1;
}

uint64_t PacketHandler::StartPack(const char* _functionName)
{
	// THIS IS PROBLEM!
	PacketSendInfo* psi = new PacketSendInfo();
	if (psi)
	{
		uint64_t id = (uint64_t)psi;

		if (SDL_LockMutex(m_sendLock) == 0)
		{
			
			(*m_packetSendInfoMap)[id] = psi;
			SDL_UnlockMutex(m_sendLock);

			psi->Position = psi->Data;

			WriteByte(id, ID_CUSTOM_PACKET);
			WriteString(id, _functionName);

		}
		else if (NET_DEBUG > 0)
			DebugLog("Failed to lock send mutex. Error: %s", LogSeverity::Error, SDL_GetError());

		return id;
	}
	else if (NET_DEBUG > 0)
		DebugLog("Tried to start pack on invalid packet id.", LogSeverity::Error);

	return -1;
}

uint64_t PacketHandler::StartPack(char _identifier)
{
	// THIS IS PROBLEM!
	PacketSendInfo* psi = new PacketSendInfo();
	if (psi)
	{
		uint64_t id = (uint64_t)psi;

		if (SDL_LockMutex(m_sendLock) == 0)
		{
			(*m_packetSendInfoMap)[id] = psi;
			SDL_UnlockMutex(m_sendLock);

			psi->Position = psi->Data;
			WriteByte(id, _identifier);
		}
		else if(NET_DEBUG > 0)
			DebugLog("Failed to lock send mutex. Error: %s", LogSeverity::Error, SDL_GetError());

		return id;
	}
	else if(NET_DEBUG > 0)
		DebugLog("Tried to start pack on invalid packet id.", LogSeverity::Error);
	
	return -1;
}

Packet* PacketHandler::EndPack(uint64_t _id)
{
	PacketSendInfo* psi = GetPacketSendInfo(_id);
	
	if (psi)
	{
		unsigned short length = (unsigned short)(psi->Position - psi->Data);

		Packet* p = new Packet();
		p->Data = new char[length];
		memcpy(p->Data, psi->Data, length);
		*p->Length = length;

		// remove psi?
		if (SDL_LockMutex(m_sendLock) == 0)
		{
			// remove but dont deallocate
			//might work
			//PacketSendInfo* to_be_deleted = m_packetSendInfoMap->at( _id );
			
			//might work
			SAFE_DELETE(psi);
			m_packetSendInfoMap->erase(_id);
			SDL_UnlockMutex(m_sendLock);
		}
		else if(NET_DEBUG > 0)
			DebugLog("Failed to lock send mutex. Error: %s", LogSeverity::Error, SDL_GetError());

		return p;
	}
	else if(NET_DEBUG > 0)
		DebugLog("Tried to end pack on invalid packet id.", LogSeverity::Error);

	return 0;
}

uint64_t PacketHandler::StartUnpack(Packet* _packet)
{
	PacketReceiveInfo* pri = new PacketReceiveInfo();
	if (pri)
	{
		pri->PacketData = _packet;
		pri->Position = pri->PacketData->Data;
		uint64_t id = (uint64_t)pri;

		if (SDL_LockMutex(m_receiveLock) == 0)
		{
			(*m_packetReceiveInfoMap)[id] = pri;
			SDL_UnlockMutex(m_receiveLock);
		}
		else if (NET_DEBUG > 0)
			DebugLog("Failed to lock receive mutex. Error: %s", LogSeverity::Error, SDL_GetError());

		return id;
	}
	else if(NET_DEBUG > 0)
		DebugLog("Tried to end pack on invalid packet id.", LogSeverity::Error);

	return -1;
}

void PacketHandler::EndUnpack(uint64_t _id)
{
	PacketReceiveInfo* pri = GetPacketReceiveInfo(_id);

	if (pri)
	{

		if (SDL_LockMutex(m_receiveLock) == 0)
		{
			m_packetReceiveInfoMap->erase(_id);
			SAFE_DELETE(pri);
			SDL_UnlockMutex(m_receiveLock);
		}
		else if (NET_DEBUG > 0)
			DebugLog("Failed to lock receive mutex. Error: %s", LogSeverity::Error, SDL_GetError());

	}
	else if (NET_DEBUG > 0)
		DebugLog("Tried to end unpack on invalid packet id.", LogSeverity::Error);
}

void PacketHandler::WriteByte(uint64_t _id, const unsigned char _byte)
{
	PacketSendInfo* psi = GetPacketSendInfo(_id);
	if (psi)
	{
		if (!IsOutOfBounds(psi->Data, psi->Position + sizeof(int), MAX_PACKET_SIZE))
		{
			memcpy(psi->Position, &_byte, sizeof(_byte));
			psi->Position += sizeof(_byte);
		}
	}
	else if (NET_DEBUG > 0)
		DebugLog("Tried to write byte to invalid packet id", LogSeverity::Error);
}

void PacketHandler::WriteBytes(uint64_t _id, const unsigned char* _bytes, unsigned short size)
{
	PacketSendInfo* psi = GetPacketSendInfo(_id);
	if (psi)
	{
		if (!IsOutOfBounds(psi->Data, psi->Position + size * sizeof(unsigned char), MAX_PACKET_SIZE))
		{
			memcpy(psi->Position, _bytes, size * sizeof(unsigned char));
			psi->Position += size * sizeof(unsigned char);
		}
	}
}

void PacketHandler::WriteFloat(uint64_t _id, const float _float)
{
	PacketSendInfo* psi = GetPacketSendInfo(_id);
	if (psi)
	{
		if (!IsOutOfBounds(psi->Data, psi->Position + sizeof(int), MAX_PACKET_SIZE))
		{
			memcpy(psi->Position, &_float, sizeof(_float));
			psi->Position += sizeof(_float);
		}
	}
	else if (NET_DEBUG > 0)
		DebugLog("Tried to write float to invalid packet id", LogSeverity::Error);
}
void PacketHandler::WriteShort(uint64_t _id, const short _short)
{
	PacketSendInfo* psi = GetPacketSendInfo(_id);
	if (psi)
	{
		if (!IsOutOfBounds(psi->Data, psi->Position + sizeof(int), MAX_PACKET_SIZE))
		{
			short i = htons(_short);
			memcpy(psi->Position, &i, sizeof(i));
			psi->Position += sizeof(i);
		}
	}
	else if (NET_DEBUG > 0)
		DebugLog("Tried to write short to invalid packet id", LogSeverity::Error);
}
void PacketHandler::WriteInt(uint64_t _id, const int _int)
{
	PacketSendInfo* psi = GetPacketSendInfo(_id);
	if (psi)
	{
		if (!IsOutOfBounds(psi->Data, psi->Position + sizeof(int), MAX_PACKET_SIZE))
		{
			int i = htonl(_int);
			memcpy(psi->Position, &i, sizeof(i));
			psi->Position += sizeof(i);
		}
	}
	else if (NET_DEBUG > 0)
		DebugLog("Tried to write int to invalid packet id", LogSeverity::Error);
}

void PacketHandler::WriteInt64(uint64_t _id, const uint64_t _int)
{
	PacketSendInfo* psi = GetPacketSendInfo(_id);
	if (psi)
	{
		if (!IsOutOfBounds(psi->Data, psi->Position + sizeof(uint64_t), MAX_PACKET_SIZE))
		{
			uint64_t i = Network::hton_ll(_int);
			memcpy(psi->Position, &i, sizeof(i));
			psi->Position += sizeof(i);
		}
	}
	else if (NET_DEBUG > 0)
		DebugLog("Tried to write uint64_t to invalid packet id", LogSeverity::Error);
}

void PacketHandler::WriteString(uint64_t _id, const char* _string)
{
	PacketSendInfo* psi = GetPacketSendInfo(_id);
	if (psi)
	{
		size_t length = strlen(_string) + 1;
		if (!IsOutOfBounds(psi->Data, psi->Position + length, MAX_PACKET_SIZE))
		{
			memcpy((char*)psi->Position, _string, length);
			psi->Position += length;
		}
	}
	else if(NET_DEBUG > 0)
		DebugLog("Tried to write string to invalid packet id", LogSeverity::Error);
}

char PacketHandler::ReadByte(uint64_t _id)
{
	char var = 0;

	PacketReceiveInfo* pri = GetPacketReceiveInfo(_id);
	if (pri)
	{
		if (!IsOutOfBounds(pri->PacketData->Data, pri->Position + sizeof(char), *pri->PacketData->Length))
		{
			memcpy(&var, pri->Position, sizeof(char));
			pri->Position += sizeof(char);
		}
	}
	else if (NET_DEBUG > 0)
		DebugLog("Tried to read char from invalid packet id", LogSeverity::Error);

	return var;
}

char* PacketHandler::ReadBytes(uint64_t _id, int size)
{
	char* var = NULL;
	PacketReceiveInfo* pri = GetPacketReceiveInfo(_id);
	if (pri)
	{
		if (!IsOutOfBounds(pri->PacketData->Data, pri->Position + size * sizeof(unsigned char), *pri->PacketData->Length))
		{
			var = pri->Position;
			pri->Position += size * sizeof(unsigned char);
		}
	}
	return var;
}

short PacketHandler::ReadShort(uint64_t _id)
{
	short var = 0;

	PacketReceiveInfo* pri = GetPacketReceiveInfo(_id);
	if (pri)
	{
		if (!IsOutOfBounds(pri->PacketData->Data, pri->Position + sizeof(short), *pri->PacketData->Length))
		{
			memcpy(&var, pri->Position, sizeof(short));
			var = ntohs(var);
			pri->Position += sizeof(short);
		}
	}
	else if (NET_DEBUG > 0)
		DebugLog("Tried to read short from invalid packet id", LogSeverity::Error);

	return var;
}

int PacketHandler::ReadInt(uint64_t _id)
{
	int var = 0;

	PacketReceiveInfo* pri = GetPacketReceiveInfo(_id);
	if (pri)
	{
		if (!IsOutOfBounds(pri->PacketData->Data, pri->Position + sizeof(int), *pri->PacketData->Length))
		{
			memcpy(&var, pri->Position, sizeof(int));
			var = ntohl(var);
			pri->Position += sizeof(int);
		}
	}
	else if (NET_DEBUG > 0)
		DebugLog("Tried to read int from invalid packet id", LogSeverity::Error);

	return var;
}

uint64_t PacketHandler::ReadInt64(uint64_t _id)
{
	uint64_t var = 0;

	PacketReceiveInfo* pri = GetPacketReceiveInfo(_id);
	if (pri)
	{
		if (!IsOutOfBounds(pri->PacketData->Data, pri->Position + sizeof(uint64_t), *pri->PacketData->Length))
		{
			memcpy(&var, pri->Position, sizeof(uint64_t));
			var = Network::ntoh_ll(var);
			pri->Position += sizeof(uint64_t);
		}
	}
	else if(NET_DEBUG > 0)
		DebugLog("Tried to read uint64_t from invalid packet id", LogSeverity::Error);

	return var;
}

char* PacketHandler::ReadString(uint64_t _id)
{
	char* var = 0;
	PacketReceiveInfo* pri = GetPacketReceiveInfo(_id);
	if (pri)
	{
		size_t length = strlen((char*)pri->Position) + 1;

		if (!IsOutOfBounds(pri->PacketData->Data, pri->Position + length, *pri->PacketData->Length))
		{
			var = (char*)pri->Position;
			pri->Position += length;
		}

	}
	else if (NET_DEBUG > 0)
		DebugLog("Tried to read string from invalid packet id", LogSeverity::Error);

	return var;

}

float PacketHandler::ReadFloat(uint64_t _id)
{
	float var = 0.f;
	PacketReceiveInfo* pri = GetPacketReceiveInfo(_id);
	if (pri)
	{
		if (!IsOutOfBounds(pri->PacketData->Data, pri->Position + sizeof(float), *pri->PacketData->Length))
		{
			memcpy(&var, pri->Position, sizeof(float));
			pri->Position += sizeof(float);
		}
	}
	else if (NET_DEBUG > 0)
		DebugLog("Tried to read float from invalid packet id", LogSeverity::Error);

	return var;
}


bool PacketHandler::IsOutOfBounds(char* _begin, char* _position, unsigned short _length)
{

	if (_position - _begin > _length)
	{
		if (NET_DEBUG > 0)
			DebugLog("Position out of bounds while reading/writing packet.", LogSeverity::Error);
		return true;
	}
	return false;
}