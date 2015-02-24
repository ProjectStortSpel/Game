#include "Packet.h"

using namespace Network;

NetConnection::NetConnection()
{
	IpAddress = new std::string();
	*IpAddress = "";
	Port = new unsigned short(0);
}

NetConnection::NetConnection(const NetConnection& _netConnection)
{
	IpAddress = new std::string();
	*IpAddress = *_netConnection.IpAddress;
	Port = new unsigned short(*_netConnection.Port);
}

NetConnection::NetConnection(const char* _address, unsigned short _port)
{
	IpAddress = new std::string();
	*IpAddress = _address;
	Port = new unsigned short(_port);
}

NetConnection::~NetConnection()
{
	SAFE_DELETE(IpAddress);
	SAFE_DELETE(Port);
}


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