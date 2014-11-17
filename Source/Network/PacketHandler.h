#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#include <string>
#include <SDL/SDL.h>

#define MAX_PACKET_SIZE 2048

class DECLSPEC PacketHandler
{
public:
	struct Packet
	{
		char* Data;
		unsigned short Length;

		Packet()
		{
		};
		Packet(char* _data, unsigned short _length)
		{
			Data = _data;
			Length = _length;
		};

	};


public:
	PacketHandler();
	~PacketHandler();

	void StartPack(const char* _name);
	void StartUnPack(Packet _packet);

	Packet EndPack();

	void WriteByte(const char _byte);
	void WriteInt(const int _int);
	void WriteString(const char* _string);
	void WriteFloat(const float _float);


	char ReadByte();
	int ReadInt();
	char* ReadString();
	float ReadFloat();

private:
	bool IsOutOfBounds(char* _begin, char* _position, short _length);

private:

	char* m_packetSend;
	char* m_positionSend;

	PacketHandler::Packet m_packetReceive;
	char* m_positionReceive;

};

#endif