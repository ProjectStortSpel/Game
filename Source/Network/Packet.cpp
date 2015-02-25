#include "Packet.h"

using namespace Network;

Packet::Packet()
{
	Data = 0;
	Length = new unsigned short(0);
	Sender = new NetConnection();

}

Packet::Packet(unsigned char* _data, unsigned short _length, NetConnection _sender)
{
	Data = _data;
	Length = new unsigned short(_length);
	Sender = new NetConnection(_sender);
}


Packet::~Packet()
{
	SAFE_DELETE_ARRAY(Data);

	SAFE_DELETE(Length);
	SAFE_DELETE(Sender);
}