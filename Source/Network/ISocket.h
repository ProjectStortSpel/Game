#ifndef ISOCKET_H
#define ISOCKET_H

class ISocket
{

public:

	virtual ~ISocket() {}

	virtual bool Connect(ISocket* _socket, void* _sockaddr, int* _addrlen) = 0;
	virtual bool Bind(ISocket* _socket, void* _sockaddr, int* _addrlen) = 0;
	virtual ISocket* Accept(ISocket* _socket, void* _sockaddr, int* _addrlen) = 0;
	virtual bool Listen(ISocket* _socket, int _backlog) = 0;
	virtual int Recv(ISocket* _socket, void* _buffer, int _length, int _flags) = 0;
	virtual int Send(ISocket* _socket, void* _buffer, int _length, int _flags) = 0;

	static ISocket* CreateISocket(int domain, int type, int protocol);

};


#endif