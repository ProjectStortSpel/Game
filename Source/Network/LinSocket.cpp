#ifndef _WIN32

#include "Network/LinSocket.h"

LinSocket::LinSocket(int _domain, int _type, int _protocol)
{

}

LinSocket::~LinSocket()
{

}

bool LinSocket::Initialize()
{
	return false;
}


bool LinSocket::Connect(const char* _ip, const int _port)
{
	return false;
}

bool LinSocket::Bind(const int _port)
{
	return false;
}

ISocket* LinSocket::Accept()
{
	return 0;
}

bool LinSocket::Listen(int _backlog)
{
	return false;
}

int LinSocket::Recv(char* _buffer, int _length, int _flags)
{
	return -1;
}

int LinSocket::Send(char* _buffer, int _length, int _flags)
{
	return -1;
}

#endif