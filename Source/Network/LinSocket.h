#ifndef _WIN32

#ifndef LINSOCKET_H
#define LINSOCKET_H

#include "Network/ISocket.h"

class LinSocket : public ISocket
{

public:
	LinSocket(int _domain, int _type, int _protocol);
	~LinSocket();

	bool Connect(void* _sockaddr, int* _addrlen);
	bool Bind(void* _sockaddr, int* _addrlen);
	ISocket* Accept(void* _sockaddr, int* _addrlen);
	bool Listen(int _backlog);
	int Recv(void* _buffer, int _length, int _flags);
	int Send(void* _buffer, int _length, int _flags);

};
#endif
#endif