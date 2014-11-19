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

	void StartPack(const char* _name);
	void StartUnPack(Packet* _packet);

	Packet EndPack();
	

	void WriteByte(const unsigned char _byte);
	void WriteInt(const int _int);
	void WriteString(const char* _string);
	void WriteFloat(const float _float);

	void AddNetMessageHook(char* _messageName, NetMessageHook _function);

	char ReadByte();
	int ReadInt();
	char* ReadString();
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