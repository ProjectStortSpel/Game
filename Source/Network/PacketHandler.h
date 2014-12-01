#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

class BaseNetwork;

#include "Stdafx.h"

class DECLSPEC PacketHandler
{

public:

	PacketHandler(void);
	~PacketHandler(void);

	// Will make it possible to create a new packet to be sent to connection
	// This should always be called before any Write function is called
	// When done writing the packet EndPack should be called
	void StartPack(const char* _name);
	// Should never be called from outside of the dll
	// Safty risk - FIX
	void StartPack(char _identifier);

	// Marks the end of a packet and should always be called when done with packing the packet
	// Returns the newly created packet
	Packet* EndPack(void);

	// Will make it possible to unpack and read a packet
	// This should always be called before and Read function is called
	char StartUnpack(Packet* packet);
	// Tell that the packet is unpacked and should be deleted
	// Should never be called from outside of the dll as it will be called automatically
	// Safty risk - FIX
	void EndUnpack(void);

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

private:

#pragma warning( disable : 4251 )

	unsigned char* m_packetSend;
	unsigned char* m_positionSend;

	Packet*		   m_packetReceive;
	unsigned char* m_positionReceive;

#pragma warning( default : 4251 )

};

#endif