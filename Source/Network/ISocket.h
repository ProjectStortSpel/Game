#ifndef ISOCKET_H
#define ISOCKET_H

#include <SDL/SDL.h>

#include "Network/Stdafx.h"

class DECLSPEC ISocket
{

public:

	virtual ~ISocket() {}

	virtual bool Connect(const char* _ip, const int _port) = 0;
	virtual bool Bind(const int _port) = 0;
	virtual ISocket* Accept(NetConnection& _netConnection) = 0;
	virtual bool Listen(int _backlog) = 0;
	virtual int Recv(char* _buffer, int _length, int _flags) = 0;
	virtual int Send(char* _buffer, int _length, int _flags) = 0;

	static bool Initialize();

	static ISocket* CreateISocket(int domain, int type, int protocol);

protected:

	bool m_socketOpen;

};


#endif