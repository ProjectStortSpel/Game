#include "NetConnection.h"

using namespace Network;

NetConnection::NetConnection()
{
	IpAddress = new std::string();
	Port = new unsigned short(0);
}

NetConnection::NetConnection(const NetConnection& _netConnection)
{
	IpAddress = new std::string(*_netConnection.IpAddress);
	Port = new unsigned short(*_netConnection.Port);
}

NetConnection::NetConnection(const char* _address, unsigned short _port)
{
	IpAddress = new std::string(_address);
	Port = new unsigned short(_port);
}

NetConnection::~NetConnection()
{
	SAFE_DELETE(IpAddress);
	SAFE_DELETE(Port);
}