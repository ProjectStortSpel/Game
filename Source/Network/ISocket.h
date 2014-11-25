#ifndef ISOCKET_H
#define ISOCKET_H

#include "Network/Stdafx.h"

class ISocket
{

public:

	virtual ~ISocket() {}

	virtual bool Connect(const char* _ip, const int _port) = 0;
	virtual bool Bind(const int _port) = 0;
	virtual ISocket* Accept() = 0;
	virtual bool Listen(int _backlog) = 0;
	virtual int Recv(void* _buffer, int _length, int _flags) = 0;
	virtual int Send(void* _buffer, int _length, int _flags) = 0;

	static bool Initialize();

	static ISocket* CreateISocket(int domain, int type, int protocol);

};


#endif