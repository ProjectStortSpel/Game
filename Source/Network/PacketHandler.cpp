#include "PacketHandler.h"
#include <RakNet/BitStream.h>

PacketHandler::PacketHandler()
	: m_packetSend(0), m_positionSend(0), m_positionReceive(0)
{
	m_packetSend = new unsigned char[MAX_PACKET_SIZE];
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
	if (NET_DEBUG)
		printf("Starting to pack message \"%s\"\n", _functionName);

	m_positionSend = m_packetSend;
	WriteByte(ID_USER_PACKET);
	WriteString(_functionName);
}

void PacketHandler::StartUnPack(Packet* _packet)
{


	m_packetReceive = _packet;
	m_positionReceive = m_packetReceive->Data;

	auto type = ReadByte();
	auto messageName = ReadString();

	if (NET_DEBUG)
		printf("Starting to unpack message \"%s\"\n", messageName);

	if (m_functionMap.find(messageName) != m_functionMap.end())
	{
		m_functionMap[messageName](this);
	}
	else if (NET_DEBUG)
	{
		printf("MessageName \"%s\" not bound to any function.\n", messageName);
	}

	EndUnPack();
}

PacketHandler::Packet PacketHandler::EndPack()
{
	if (NET_DEBUG)
		printf("Stop to pack message\n");

	unsigned short length = m_positionSend - m_packetSend;

	return Packet(m_packetSend, length);
}

void PacketHandler::EndUnPack()
{
	if (NET_DEBUG)
		printf("Stop to unpack message, deallocating packet\n");

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

void PacketHandler::AddNetMessageHook(char* _messageName, NetMessageHook _function)
{
	if (NET_DEBUG)
	{
		printf("Adding message hook to function\n");

		if (m_functionMap.find(_messageName) == m_functionMap.end())
			printf("NetMessageHook already exist.\n");
	}

	m_functionMap[_messageName] = _function;
}

void PacketHandler::WriteByte(const unsigned char _byte)
{
	if (NET_DEBUG)
		printf("Writing byte to packet: ");


	if (!IsOutOfBounds(m_packetSend, m_positionSend + sizeof(unsigned char), MAX_PACKET_SIZE))
	{
		memcpy(m_positionSend, &_byte, sizeof(_byte));
		m_positionSend += sizeof(_byte);

		if (NET_DEBUG)
			printf("%c\n", _byte);
	}

}

void PacketHandler::WriteFloat(const float _float)
{
	if (NET_DEBUG)
		printf("Writing float to packet: ");

	if (!IsOutOfBounds(m_packetSend, m_positionSend + sizeof(float), MAX_PACKET_SIZE))
	{
		memcpy(m_positionSend, &_float, sizeof(_float));
		m_positionSend += sizeof(_float);

		if (NET_DEBUG)
			printf("%f\n", _float);
	}

}

void PacketHandler::WriteInt(const int _int)
{
	if (NET_DEBUG)
		printf("Writing int to packet: ");

	if (!IsOutOfBounds(m_packetSend, m_positionSend + sizeof(int), MAX_PACKET_SIZE))
	{
		memcpy(m_positionSend, &_int, sizeof(_int));
		m_positionSend += sizeof(_int);

		if (NET_DEBUG)
			printf("%i\n", _int);
	}

}

void PacketHandler::WriteString(const char* _string)
{
	if (NET_DEBUG)
		printf("Writing string to packet: ");

	size_t length = strlen(_string) + 1;
	if (!IsOutOfBounds(m_packetSend, m_positionSend + length, MAX_PACKET_SIZE))
	{
		memcpy((char*)m_positionSend, _string, length);
		m_positionSend += length;

		if (NET_DEBUG)
			printf("%s\n", _string);
	}

}

char PacketHandler::ReadByte()
{
	if (NET_DEBUG)
		printf("Reading byte from packet: ");

	char var = 0;

	if (!IsOutOfBounds(m_packetReceive->Data, m_positionReceive + sizeof(char), m_packetReceive->Length))
	{
		memcpy(&var, m_positionReceive, sizeof(char));
		m_positionReceive += sizeof(char);

		if (NET_DEBUG)
			printf("%c\n", var);
	}


	return var;
}

int PacketHandler::ReadInt()
{
	if (NET_DEBUG)
		printf("Reading int from packet: ");

	int var = 0;

	if (!IsOutOfBounds(m_packetReceive->Data, m_positionReceive + sizeof(int), m_packetReceive->Length))
	{
		memcpy(&var, m_positionReceive, sizeof(int));
		m_positionReceive += sizeof(int);

		if (NET_DEBUG)
			printf("%i\n", var);
	}


	return var;
}
char* PacketHandler::ReadString()
{
	if (NET_DEBUG)
		printf("Reading string from packet: ");

	size_t length = strlen((char*)m_positionReceive) + 1;
	char* var = new char[length];

	if (!IsOutOfBounds(m_packetReceive->Data, m_positionReceive + length, m_packetReceive->Length))
	{
		memcpy(var,(char*)m_positionReceive, length);
		m_positionReceive += length;

		if (NET_DEBUG)
			printf("%s\n", var);
	}


	return var;
}
float PacketHandler::ReadFloat()
{
	if (NET_DEBUG)
		printf("Reading float from packet: ");

	float var = 0.f;

	if (!IsOutOfBounds(m_packetReceive->Data, m_positionReceive + sizeof(float), m_packetReceive->Length))
	{
		memcpy(&var, m_positionReceive, sizeof(float));
		m_positionReceive += sizeof(float);

		if (NET_DEBUG)
			printf("%f\n", var);
	}

	return var;
}

bool PacketHandler::IsOutOfBounds(unsigned char* _begin, unsigned char* _position, unsigned short _length)
{
	if (_position - _begin > _length)
	{
		printf("\nOut of bounds while reading/writing packet.\n");
		return true;
	}
	else
		return false;
}