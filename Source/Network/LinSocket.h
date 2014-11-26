#ifndef WIN32

#ifndef LINSOCKET_H
#define LINSOCKET_H
#include <sys/socket.h>

#include <arpa/inet.h> 

#include "Network/ISocket.h"


class DECLSPEC LinSocket : public ISocket
{

	int m_socket;

public:
	LinSocket(int _socket);
	LinSocket(int _domain, int _type, int _protocol);
	~LinSocket();

	static bool Initialize();
	static bool Shutdown();

	bool Connect(const char* _ip, const int _port);
	bool Bind(const int _port);
	bool Close();
	ISocket* Accept();
	bool Listen(int _backlog);
	int Recv(char* _buffer, int _length, int _flags);
	int Send(char* _buffer, int _length, int _flags);

};
#endif
#endif