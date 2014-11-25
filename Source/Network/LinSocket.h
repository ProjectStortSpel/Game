#ifndef _WIN32

#ifndef LINSOCKET_H
#define LINSOCKET_H

#include "Network/ISocket.h"

class LinSocket : public ISocket
{

public:
	LinSocket(int _domain, int _type, int _protocol);
	~LinSocket();

	static bool Initialize();

	bool Connect(const char* _ip, const int _port);
	bool Bind(const int _port);
	ISocket* Accept();
	bool Listen(int _backlog);
	int Recv(char* _buffer, int _length, int _flags);
	int Send(char* _buffer, int _length, int _flags);

};
#endif
#endif