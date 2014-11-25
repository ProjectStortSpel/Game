#ifndef _WIN32

#ifndef LINSOCKET_H
#define LINSOCKET_H

#include "Network/ISocket.h"

class DECLSPEC LinSocket : public ISocket
{

	int m_socket;

	std::string m_remoteIP;
	int m_remotePort;
	int m_localPort;

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