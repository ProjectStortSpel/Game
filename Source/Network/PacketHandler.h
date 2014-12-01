#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

class BaseNetwork;

#include <map>
#include "Stdafx.h"

class DECLSPEC PacketHandler
{
	friend class BaseNetwork;
public:

	PacketHandler(void);
	~PacketHandler(void);

	// Will make it possible to create a new packet to be sent to connection
	// This should always be called before any Write function is called
	// When done writing the packet EndPack should be called
	uint64_t StartPack(const char* _name);
	// Should never be called from outside of the dll
	// Safty risk - FIX
	uint64_t StartPack(char _identifier);

	char GetNetTypeMessageId(Packet* _p);
	// Marks the end of a packet and should always be called when done with packing the packet
	// Returns the newly created packet
	Packet* EndPack(uint64_t _id);

	// Will make it possible to unpack and read a packet
	// This should always be called before and Read function is called
	uint64_t StartUnpack(Packet* packet);
	// Tell that the packet is unpacked and should be deleted
	// Should never be called from outside of the dll as it will be called automatically
	// Safty risk - FIX
	void EndUnpack(uint64_t _id);

	// Write a byte to the packet
	// StartPack should be called before this is used
	void WriteByte(uint64_t _id, const unsigned char _byte);
	// Write a short to the packet
	// StartPack should be called before this is used
	void WriteShort(uint64_t _id, const short _short);
	// Write an int to the packet
	// StartPack should be called before this is used
	void WriteInt(uint64_t _id, const int _int);
	// Write a string to the packet
	// StartPack should be called before this is used
	void WriteString(uint64_t _id, const char* _string);
	// Write a float to the packet
	// StartPack should be called before this is used
	void WriteFloat(uint64_t _id, const float _float);


	// Read a byte from a packet
	// Should always be called from a function bound with AddNetMessageHook
	char ReadByte(uint64_t _id);
	// Read an int from a packet
	// Should always be called from a function bound with AddNetMessageHook
	short ReadShort(uint64_t _id);
	// Read a short from a packet
	// Should always be called from a function bound with AddNetMessageHook
	int ReadInt(uint64_t _id);
	// Read a string from a packet
	// Should always be called from a function bound with AddNetMessageHook
	char* ReadString(uint64_t _id);
	// Read a float from a packet
	// Should always be called from a function bound with AddNetMessageHook
	float ReadFloat(uint64_t _id);

private:

	struct PacketSendInfo
	{
		unsigned char  Data[MAX_PACKET_SIZE];
		unsigned char* Position;
	};

	struct PacketReceiveInfo
	{
		Packet*		   PacketData;
		unsigned char* Position;
	};

	PacketSendInfo* GetPacketSendInfo(uint64_t id);
	PacketReceiveInfo* GetPacketReceiveInfo(uint64_t id);
	
	bool IsOutOfBounds(unsigned char* _begin, unsigned char* _position, unsigned short _length);

private:

#pragma warning( disable : 4251 )

	std::map<uint64_t, PacketSendInfo*> m_packetSendInfoMap;
	std::map<uint64_t, PacketReceiveInfo*> m_packetReceiveInfoMap;

	//unsigned char* m_packetSend;
	//unsigned char* m_positionSend;

	//Packet*		   m_packetReceive;
	//unsigned char* m_positionReceive;

#pragma warning( default : 4251 )

};

#endif