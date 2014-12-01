#include "PacketHandler.h"
#include "NetTypeMessageID.h"
#include "BaseNetwork.h"

PacketHandler::PacketHandler()
{
}

PacketHandler::~PacketHandler()
{
	m_sendMutex.lock();
	for (auto iterator = m_packetSendInfoMap.begin(); iterator != m_packetSendInfoMap.end(); iterator++)
	{
		delete iterator->second;
	}
	m_packetSendInfoMap.clear();
	m_sendMutex.unlock();

	m_receiveMutex.lock();
	for (auto iterator = m_packetReceiveInfoMap.begin(); iterator != m_packetReceiveInfoMap.end(); iterator++)
	{
		delete iterator->second->PacketData;
		delete iterator->second;
	}
	m_packetReceiveInfoMap.clear();
	m_receiveMutex.unlock();
}

PacketHandler::PacketSendInfo* PacketHandler::GetPacketSendInfo(uint64_t _id)
{
	PacketHandler::PacketSendInfo* result = 0;
	m_sendMutex.lock();
	if (m_packetSendInfoMap.find(_id) != m_packetSendInfoMap.end())
	{
		result = m_packetSendInfoMap[_id];
	}
	m_sendMutex.unlock();
	return result;
}

PacketHandler::PacketReceiveInfo* PacketHandler::GetPacketReceiveInfo(uint64_t _id)
{
	PacketHandler::PacketReceiveInfo* result = 0;
	m_receiveMutex.lock();
	if (m_packetReceiveInfoMap.find(_id) != m_packetReceiveInfoMap.end())
	{
		result = m_packetReceiveInfoMap[_id];
	}
	m_receiveMutex.unlock();
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
	{
		return pri->PacketData->Data[0];
	}
	else
	{
		return -1;
	}

}

uint64_t PacketHandler::StartPack(const char* _functionName)
{
	PacketSendInfo* psi = new PacketSendInfo();
	uint64_t id = (uint64_t)psi;
	
	m_sendMutex.lock();
	m_packetSendInfoMap[id] = psi;
	m_sendMutex.unlock();

	psi->Position = psi->Data;

	WriteByte(id, ID_CUSTOM_PACKET);
	WriteString(id, _functionName);
	return id;
}

uint64_t PacketHandler::StartPack(char _identifier)
{
	PacketSendInfo* psi = new PacketSendInfo();
	uint64_t id = (uint64_t)psi;

	m_sendMutex.lock();
	m_packetSendInfoMap[id] = psi;
	m_sendMutex.unlock();

	psi->Position = psi->Data;
	WriteByte(id, _identifier);
	return id;
}

Packet* PacketHandler::EndPack(uint64_t _id)
{
	PacketSendInfo* psi = GetPacketSendInfo(_id);

	unsigned short length = psi->Position - psi->Data;

	Packet* p = new Packet();
	p->Data = new unsigned char[length];
	memcpy(p->Data, psi->Data, length);
	p->Length = length;

	m_sendMutex.lock();
	m_packetSendInfoMap.erase(_id);
	m_sendMutex.unlock();
	delete psi;

	return p;
}

uint64_t PacketHandler::StartUnpack(Packet* _packet)
{
	PacketReceiveInfo* pri = new PacketReceiveInfo();
	pri->PacketData = _packet;
	pri->Position = pri->PacketData->Data;
	uint64_t id = (uint64_t)pri;

	m_receiveMutex.lock();
	m_packetReceiveInfoMap[id] = pri;
	m_receiveMutex.unlock();

	char type = ReadByte(id);

	if (type == NetTypeMessageId::ID_CUSTOM_PACKET)
		ReadString(id);

	return id;
}

void PacketHandler::EndUnpack(uint64_t _id)
{
	PacketReceiveInfo* pri = GetPacketReceiveInfo(_id);

	if (pri)
	{
		m_receiveMutex.lock();
		m_packetReceiveInfoMap.erase(_id);
		m_receiveMutex.unlock();
		delete pri->PacketData;
		delete pri;
	}
}

void PacketHandler::WriteByte(uint64_t _id, const unsigned char _byte)
{
	PacketSendInfo* psi = GetPacketSendInfo(_id);
	if (psi)
	{
		if (!IsOutOfBounds(psi->Data, psi->Position + sizeof(unsigned char), MAX_PACKET_SIZE))
		{
			memcpy(psi->Position, &_byte, sizeof(_byte));
			psi->Position += sizeof(_byte);
		}
	}	
}
void PacketHandler::WriteFloat(uint64_t _id, const float _float)
{
	PacketSendInfo* psi = GetPacketSendInfo(_id);
	if (psi)
	{
		if (!IsOutOfBounds(psi->Data, psi->Position + sizeof(float), MAX_PACKET_SIZE))
		{
			memcpy(psi->Position, &_float, sizeof(_float));
			psi->Position += sizeof(_float);
		}
	}
}
void PacketHandler::WriteShort(uint64_t _id, const short _short)
{
	PacketSendInfo* psi = GetPacketSendInfo(_id);
	if (psi)
	{
		if (!IsOutOfBounds(psi->Data, psi->Position + sizeof(int), MAX_PACKET_SIZE))
		{
			memcpy(psi->Position, &_short, sizeof(_short));
			psi->Position += sizeof(_short);
		}
	}
}
void PacketHandler::WriteInt(uint64_t _id, const int _int)
{
	PacketSendInfo* psi = GetPacketSendInfo(_id);
	if (psi)
	{
		if (!IsOutOfBounds(psi->Data, psi->Position + sizeof(int), MAX_PACKET_SIZE))
		{
			memcpy(psi->Position, &_int, sizeof(_int));
			psi->Position += sizeof(_int);
		}
	}
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
}

char PacketHandler::ReadByte(uint64_t _id)
{
	char var = 0;

	PacketReceiveInfo* pri = GetPacketReceiveInfo(_id);
	if (pri)
	{
		if (!IsOutOfBounds(pri->PacketData->Data, pri->Position + sizeof(char), pri->PacketData->Length))
		{
			memcpy(&var, pri->Position, sizeof(char));
			pri->Position += sizeof(char);
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
		if (!IsOutOfBounds(pri->PacketData->Data, pri->Position + sizeof(short), pri->PacketData->Length))
		{
			memcpy(&var, pri->Position, sizeof(short));
			pri->Position += sizeof(short);
		}
	}
	return var;
}

int PacketHandler::ReadInt(uint64_t _id)
{
	int var = 0;

	PacketReceiveInfo* pri = GetPacketReceiveInfo(_id);
	if (pri)
	{
		if (!IsOutOfBounds(pri->PacketData->Data, pri->Position + sizeof(int), pri->PacketData->Length))
		{
			memcpy(&var, pri->Position, sizeof(int));
			pri->Position += sizeof(int);
		}
	}
	return var;
}

char* PacketHandler::ReadString(uint64_t _id)
{
	char* var = "";
	PacketReceiveInfo* pri = GetPacketReceiveInfo(_id);
	if (pri)
	{
		size_t length = strlen((char*)pri->Position) + 1;
		var = new char[length];

		if (!IsOutOfBounds(pri->PacketData->Data, pri->Position + length, pri->PacketData->Length))
		{
			memcpy(var, (char*)pri->Position, length);
			//strcpy_s(var, length, (char*)m_userPacketReceiveInfo.Position);
			pri->Position += length;
		}
	}
	return var;
}

float PacketHandler::ReadFloat(uint64_t _id)
{
	float var = 0.f;
	PacketReceiveInfo* pri = GetPacketReceiveInfo(_id);
	if (pri)
	{
		if (!IsOutOfBounds(pri->PacketData->Data, pri->Position + sizeof(float), pri->PacketData->Length))
		{
			memcpy(&var, pri->Position, sizeof(float));
			pri->Position += sizeof(float);
		}
	}

	return var;
}







bool PacketHandler::IsOutOfBounds(unsigned char* _begin, unsigned char* _position, unsigned short _length)
{
	if (_position - _begin > _length)
	{
		if (NET_DEBUG)
		{
			printf("Out of bounds while reading/writing packet.\n");
		}
		return true;
	}
	else
		return false;
}