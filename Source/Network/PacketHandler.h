#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#include "Stdafx.h"

class DECLSPEC PacketHandler
{

public:

	PacketHandler(void);
	~PacketHandler(void);

	void StartPack(const char* _name);
	void StartPack(char _identifier);

	Packet* EndPack(void);

	void Unpack(Packet* packet);

	// Write a byte to the packet
	// StartPack should be called before this is used
	void WriteByte(const unsigned char _byte);
	// Write an short to the packet
	// StartPack should be called before this is used
	void WriteShort(const short _short);
	// Write an int to the packet
	// StartPack should be called before this is used
	void WriteInt(const int _int);
	// Write an string to the packet
	// StartPack should be called before this is used
	void WriteString(const char* _string);
	// Write an float to the packet
	// StartPack should be called before this is used
	void WriteFloat(const float _float);


	// Read a byte from a packet
	// Should always be called from a function bound with AddNetMessageHook
	char ReadByte();
	// Read an int from a packet
	// Should always be called from a function bound with AddNetMessageHook
	int ReadInt();
	// Read an string from a packet
	// Should always be called from a function bound with AddNetMessageHook
	char* ReadString();
	// Read an float from a packet
	// Should always be called from a function bound with AddNetMessageHook
	float ReadFloat();

private:
	bool IsOutOfBounds(unsigned char* _begin, unsigned char* _position, unsigned short _length);
	void EndUnpack();

private:
	unsigned char* m_packetSend;
	unsigned char* m_positionSend;

	Packet*		   m_packetReceive;
	unsigned char* m_positionReceive;

};

#endif