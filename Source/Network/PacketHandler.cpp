#include "PacketHandler.h"
#include <RakNet/BitStream.h>

PacketHandler::PacketHandler()
	: m_packetSend(0), m_positionSend(0), m_positionReceive(0)
{
	m_packetSend = new char[MAX_PACKET_SIZE];
}


PacketHandler::~PacketHandler()
{
	if (m_packetSend)
	{
		delete m_packetSend;
		m_packetSend = 0;
	}

	m_positionSend = 0;
	m_positionReceive = 0;
}

void PacketHandler::StartPack(const char* _functionName)
{
	m_positionSend = m_packetSend;
	WriteString(_functionName);
}

void PacketHandler::StartUnPack(Packet _packet)
{
	m_packetReceive = _packet;
	m_positionReceive = m_packetReceive.Data;

	auto hest = ReadString();
}

PacketHandler::Packet PacketHandler::EndPack()
{
	unsigned short length = m_positionSend - m_packetSend;

	return Packet(m_packetSend, length);
}

void PacketHandler::WriteByte(const char _byte)
{
	if (!IsOutOfBounds(m_packetSend, m_positionSend + sizeof(char), MAX_PACKET_SIZE))
	{
		memcpy(m_positionSend, &_byte, sizeof(_byte));
		m_positionSend += sizeof(_byte);
	}
}

void PacketHandler::WriteFloat(const float _float)
{
	if (!IsOutOfBounds(m_packetSend, m_positionSend + sizeof(float), MAX_PACKET_SIZE))
	{
		memcpy(m_positionSend, &_float, sizeof(_float));
		m_positionSend += sizeof(_float);
	}
}

void PacketHandler::WriteInt(const int _int)
{
	if (!IsOutOfBounds(m_packetSend, m_positionSend + sizeof(int), MAX_PACKET_SIZE))
	{
		memcpy(m_positionSend, &_int, sizeof(_int));
		m_positionSend += sizeof(_int);
	}
}

void PacketHandler::WriteString(const char* _string)
{
	size_t length = strlen(_string) + 1;
	if (!IsOutOfBounds(m_packetSend, m_positionSend + length, MAX_PACKET_SIZE))
	{
		strcpy_s(m_positionSend, length, _string);
		m_positionSend += length;
	}
}



char PacketHandler::ReadByte()
{
	char var = 0;

	if (!IsOutOfBounds(m_packetReceive.Data, m_positionReceive + sizeof(char), m_packetReceive.Length))
	{
		memcpy(&var, m_positionReceive, sizeof(char));
		m_positionReceive += sizeof(char);
	}
	return var;
}

int PacketHandler::ReadInt()
{
	int var = 0;

	if (!IsOutOfBounds(m_packetReceive.Data, m_positionReceive + sizeof(int), m_packetReceive.Length))
	{
		memcpy(&var, m_positionReceive, sizeof(int));
		m_positionReceive += sizeof(int);
	}

	return var;
}
char* PacketHandler::ReadString()
{
	size_t length = strlen(m_positionReceive) + 1;
	char* var = new char[length];

	if (!IsOutOfBounds(m_packetReceive.Data, m_positionReceive + length, m_packetReceive.Length))
	{
		strcpy_s(var, length, m_positionReceive);
		m_positionReceive += length;
	}
	return var;
}
float PacketHandler::ReadFloat()
{
	float var = 0.f;

	if (!IsOutOfBounds(m_packetReceive.Data, m_positionReceive + sizeof(float), m_packetReceive.Length))
	{
		memcpy(&var, m_positionReceive, sizeof(float));
		m_positionReceive += sizeof(float);
	}

	return var;
}

bool PacketHandler::IsOutOfBounds(char* _begin, char* _position, short _length)
{
	if (_position - _begin > _length)
	{
		printf("Out of bounds while reading/writing packet.\n");
		return true;
	}
	else
		return false;
}