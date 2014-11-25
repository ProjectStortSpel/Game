#ifdef _WIN32
#include "Network/WinSocket.h"

WinSocket::WinSocket(int _domain, int _type, int _protocol)
{
	m_socket = socket(_domain, _type, _protocol);
}

WinSocket::~WinSocket()
{

}


bool Connect(void* _sockaddr, int* _addrlen)
{ 
	return false; 
}

bool Bind(void* _sockaddr, int* _addrlen)
{ 
	return false; 
}

ISocket* Accept(void* _sockaddr, int* _addrlen)
{ 
	return 0; 
}

bool Listen(int _backlog)
{ 
	return false; 
}

int Recv(void* _buffer, int _length, int _flags)
{ 
	return -1; 
}

int Send(void* _buffer, int _length, int _flags)
{ 
	return -1; 
}
#endif