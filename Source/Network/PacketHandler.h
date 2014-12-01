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


	char GetNetTypeMessageId(Packet* _p);

	uint64_t StartPack(const char* _name);
	uint64_t StartPack(char _identifier);

	Packet* EndPack(uint64_t _id);

	uint64_t StartUnpack(Packet* packet);

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
		Packet*		   Packet;
		unsigned char* Position;
	};


	void EndUnpack(uint64_t _id);
	
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