#ifndef _WIN32

#include "Network/LinSocket.h"

LinSocket::LinSocket(int _domain, int _type, int _protocol)
{

}

LinSocket::~LinSocket()
{

}

bool LinSocket::Connect(void* _sockaddr, int* _addrlen)
{
	return false;
}

bool LinSocket::Bind(void* _sockaddr, int* _addrlen)
{
	return false;
}

ISocket* LinSocket::Accept(void* _sockaddr, int* _addrlen)
{
	return 0;
}

bool LinSocket::Listen(int _backlog)
{
	return false;
}

int LinSocket::Recv(void* _buffer, int _length, int _flags)
{
	return -1;
}

int LinSocket::Send(void* _buffer, int _length, int _flags)
{
	return -1;
}

#endif