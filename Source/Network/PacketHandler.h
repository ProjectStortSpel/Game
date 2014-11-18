#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H

#include <string>
#include <SDL/SDL.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/RakNetTypes.h>

#define MAX_PACKET_SIZE 2048

enum MessageIDType
{
	ID_USER_PACKET = ID_USER_PACKET_ENUM
};

class DECLSPEC PacketHandler
{
public:



	struct Packet
	{
		unsigned char* Data;
		unsigned short Length;
		RakNet::SystemAddress Sender;

		Packet()
		{
			Data	= 0;
			Length	= 0;
			Sender	= RakNet::UNASSIGNED_SYSTEM_ADDRESS;
		};
		Packet(unsigned char* _data, unsigned short _length, RakNet::SystemAddress _sender = RakNet::UNASSIGNED_SYSTEM_ADDRESS)
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
	void EndUnPack();

	void WriteByte(const unsigned char _byte);
	void WriteInt(const int _int);
	void WriteString(const char* _string);
	void WriteFloat(const float _float);


	char ReadByte();
	int ReadInt();
	char* ReadString();
	float ReadFloat();

private:
	bool IsOutOfBounds(unsigned char* _begin, unsigned char* _position, short _length);

private:

#pragma warning( disable : 4251 )

	unsigned char* m_packetSend;
	unsigned char* m_positionSend;

	PacketHandler::Packet* m_packetReceive;
	unsigned char* m_positionReceive;

#pragma warning( default : 4251 )

};

#endif