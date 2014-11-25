#ifndef LINSOCKET_H
#define LINSOCKET_H

#include "Network/ISocket.h"

class LinSocket : public ISocket
{

public:
	LinSocket(int _domain, int _type, int _protocol);
	~LinSocket();

	bool Connect(ISocket* _socket, void* _sockaddr, int* _addrlen)  { return false; }
	bool Bind(ISocket* _socket, void* _sockaddr, int* _addrlen)  { return false; }
	ISocket* Accept(ISocket* _socket, void* _sockaddr, int* _addrlen)  { return 0; }
	bool Listen(ISocket* _socket, int _backlog)  { return false; }
	int Recv(ISocket* _socket, void* _buffer, int _length, int _flags)  { return -1; }
	int Send(ISocket* _socket, void* _buffer, int _length, int _flags) { return -1; }

};
#endif