#include "PacketHandler.h"
#include "NetTypeMessageID.h"

PacketHandler::PacketHandler()
{
	m_packetSend = new unsigned char[MAX_PACKET_SIZE];
	m_packetReceive = new Packet();

	m_positionSend = 0;
	m_positionReceive = 0;
}

PacketHandler::~PacketHandler()
{
	SAFE_DELETE(m_packetSend);
	m_positionSend = 0;

	EndUnpack();
}

void PacketHandler::StartPack(const char* _functionName)
{
	m_positionSend = m_packetSend;
	WriteByte(ID_CUSTOM_PACKET);
	WriteString(_functionName);
}

void PacketHandler::StartPack(char _identifier)
{
	m_positionSend = m_packetSend;
	WriteByte(_identifier);
}

Packet* PacketHandler::EndPack()
{
	unsigned short length = m_positionSend - m_packetSend;

	Packet* p = new Packet();
	p->Data = new unsigned char[length];
	memcpy(p->Data, m_packetSend, length);
	p->Length = length;

	return p;
}

void PacketHandler::Unpack(Packet* _packet)
{
	if (!_packet)
		return;

	m_packetReceive = _packet;
	m_positionReceive = m_packetReceive->Data;

	char type = ReadByte();

	if (type == NetTypeMessageId::ID_CUSTOM_PACKET)
	{
		// Custom user packets
	}
	else
	{
		// Net specific packets
	}

}

void PacketHandler::EndUnpack()
{
	if (m_packetReceive->Data)
	{
		delete m_packetReceive->Data;
		m_packetReceive->Data = 0;
	}

	if (m_packetReceive)
	{
		delete m_packetReceive;
		m_packetReceive = 0;
	}

	m_positionReceive = 0;
}


void PacketHandler::WriteByte(const unsigned char _byte)
{
	if (!IsOutOfBounds(m_packetSend, m_positionSend + sizeof(unsigned char), MAX_PACKET_SIZE))
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
void PacketHandler::WriteShort(const short _short)
{
	if (!IsOutOfBounds(m_packetSend, m_positionSend + sizeof(int), MAX_PACKET_SIZE))
	{
		memcpy(m_positionSend, &_short, sizeof(_short));
		m_positionSend += sizeof(_short);
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
		memcpy((char*)m_positionSend, _string, length);
		m_positionSend += length;
	}
}

char PacketHandler::ReadByte()
{
	char var = 0;

	if (!IsOutOfBounds(m_packetReceive->Data, m_positionReceive + sizeof(char), m_packetReceive->Length))
	{
		memcpy(&var, m_positionReceive, sizeof(char));
		m_positionReceive += sizeof(char);
	}
	return var;
}
int PacketHandler::ReadInt()
{
	int var = 0;

	if (!IsOutOfBounds(m_packetReceive->Data, m_positionReceive + sizeof(int), m_packetReceive->Length))
	{
		memcpy(&var, m_positionReceive, sizeof(int));
		m_positionReceive += sizeof(int);
	}

	return var;
}
char* PacketHandler::ReadString()
{
	size_t length = strlen((char*)m_positionReceive) + 1;
	char* var = new char[length];

	if (!IsOutOfBounds(m_packetReceive->Data, m_positionReceive + length, m_packetReceive->Length))
	{
		memcpy(var, (char*)m_positionReceive, length);
		//strcpy_s(var, length, (char*)m_positionReceive);
		m_positionReceive += length;
	}
	return var;
}
float PacketHandler::ReadFloat()
{
	float var = 0.f;

	if (!IsOutOfBounds(m_packetReceive->Data, m_positionReceive + sizeof(float), m_packetReceive->Length))
	{
		memcpy(&var, m_positionReceive, sizeof(float));
		m_positionReceive += sizeof(float);
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