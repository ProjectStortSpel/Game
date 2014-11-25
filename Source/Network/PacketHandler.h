#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#include <string>
#include <map>
#include <SDL/SDL.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/RakNetTypes.h>
#include "Network/Stdafx.h"

#define MAX_PACKET_SIZE 65535	//max value for unsigned short 	

enum MessageIDType
{
	ID_USER_PACKET = ID_USER_PACKET_ENUM
};

class DECLSPEC PacketHandler
{
private:

	typedef std::function<void(PacketHandler*, NetConnection*)> NetMessageHook;

public:

	struct Packet
	{
		unsigned char* Data;
		unsigned short Length;
		NetConnection* Sender;

		Packet()
		{
			Data	= 0;
			Length	= 0;
			Sender	= NULL;
		};
		Packet(unsigned char* _data, unsigned short _length, NetConnection* _sender = NULL)
		{
			Data = _data;
			Length = _length;
			Sender = _sender;
		};

	};


public:
	PacketHandler();
	~PacketHandler();

	// Start to pack a packet
	// This should always be called before starting to write anything to the packet
	// _name should be the string bound to a function with AddNetMessageHook
	void StartPack(const char* _name);
	// Start to unpack a packet
	// This will call the function hooked to the specific message
	void ConsumePacket(PacketHandler::Packet* _packet);
	// End pack of a packet
	// Should always be called when finish with building a packet
	PacketHandler::Packet EndPack();
	
	// Write a byte to the packet
	// StartPack should be called before this is used
	void WriteByte(const unsigned char _byte);
	// Write an int to the packet
	// StartPack should be called before this is used
	void WriteInt(const int _int);
	// Write an string to the packet
	// StartPack should be called before this is used
	void WriteString(const char* _string);
	// Write an float to the packet
	// StartPack should be called before this is used
	void WriteFloat(const float _float);

	// Bind a function to a specific string
	// _messageName takes the desired name
	// _function is a function with "PacketHandler*" as only input parameter
	// To bind a function outside of a class, send the address of the function (eg. &Function)
	// To bind a function within a class you must use std::bind (eg. std::bind(this, Function, std::placeholders::_1)
	void AddNetMessageHook(char* _messageName, NetMessageHook _function);

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
	void EndUnPack();

private:

#pragma warning( disable : 4251 )
	

	std::map<std::string, NetMessageHook> m_functionMap;

	unsigned char* m_packetSend;
	unsigned char* m_positionSend;

	PacketHandler::Packet* m_packetReceive;
	unsigned char* m_positionReceive;

#pragma warning( default : 4251 )

};

#endif