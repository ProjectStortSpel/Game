#ifndef ISOCKET_H
#define ISOCKET_H

class ISocket
{

public:

	virtual ~ISocket() {}

	virtual bool Connect(void* _sockaddr, int* _addrlen) = 0;
	virtual bool Bind(void* _sockaddr, int* _addrlen) = 0;
	virtual ISocket* Accept(void* _sockaddr, int* _addrlen) = 0;
	virtual bool Listen(int _backlog) = 0;
	virtual int Recv(void* _buffer, int _length, int _flags) = 0;
	virtual int Send(void* _buffer, int _length, int _flags) = 0;

	static ISocket* CreateISocket(int domain, int type, int protocol);

};


#endif