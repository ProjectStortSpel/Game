#ifdef _WIN32
#ifndef WINSOCKET_H
#define WINSOCKET_H
#include <WinSock2.h>




#include "Network/ISocket.h"

class WinSocket : public ISocket
{

	SOCKET m_socket;

public:
	WinSocket(int _domain, int _type, int _protocol);
	~WinSocket();

	bool Connect(void* _sockaddr, int* _addrlen);
	bool Bind(void* _sockaddr, int* _addrlen);
	ISocket* Accept(void* _sockaddr, int* _addrlen);
	bool Listen(int _backlog);
	int Recv(void* _buffer, int _length, int _flags);
	int Send(void* _buffer, int _length, int _flags);

};

#endif
#endif