#ifdef _WIN32
#include "Network/WinSocket.h"

WinSocket::WinSocket(int _domain, int _type, int _protocol)
{
	m_socket = socket(_domain, _type, _protocol);
}

WinSocket::~WinSocket()
{

}


bool WinSocket::Connect(void* _sockaddr, int* _addrlen)
{ 
	return false; 
}

bool WinSocket::Bind(void* _sockaddr, int* _addrlen)
{ 
	return false; 
}

ISocket* WinSocket::Accept(void* _sockaddr, int* _addrlen)
{ 
	return 0; 
}

bool WinSocket::Listen(int _backlog)
{ 
	return false; 
}

int WinSocket::Recv(void* _buffer, int _length, int _flags)
{ 
	return -1; 
}

int WinSocket::Send(void* _buffer, int _length, int _flags)
{ 
	return -1; 
}
#endif