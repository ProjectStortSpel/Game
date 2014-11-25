#ifndef WINSOCKET_H
#define WINSOCKET_H

#include <WinSock2.h>
#include "Network/ISocket.h"

class WinSocket : public ISocket
{

public:
	WinSocket(int _domain, int _type, int _protocol);
	~WinSocket();

	bool Connect(ISocket* _socket, void* _sockaddr, int* _addrlen)  { return false; }
	bool Bind(ISocket* _socket, void* _sockaddr, int* _addrlen)  { return false; }
	ISocket* Accept(ISocket* _socket, void* _sockaddr, int* _addrlen)  { return 0; }
	bool Listen(ISocket* _socket, int _backlog)  { return false; }
	int Recv(ISocket* _socket, void* _buffer, int _length, int _flags)  { return -1; }
	int Send(ISocket* _socket, void* _buffer, int _length, int _flags) { return -1; }

};
#endif