#include "Packet.h"

using namespace Network;

NetConnection::NetConnection()
{
	IpAddress = "";
	Port = 0;
}

NetConnection::NetConnection(const char* _address, unsigned short _port)
{
	IpAddress = _address;
	Port = _port;
}


Packet::Packet()
{
	Data = 0;
	Length = 0;
	Sender = NetConnection();
}

Packet::Packet(unsigned char* _data, unsigned short _length, NetConnection _sender)
{
	Data = _data;
	Length = _length;
	Sender = _sender;
}


Packet::~Packet()
{
	SAFE_DELETE(Data);
}