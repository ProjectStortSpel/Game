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
	virtual ISocket* Accept() = 0;
	virtual bool Listen(int _backlog) = 0;
	virtual int Recv(char* _buffer, int _length, int _flags) = 0;
	virtual int Send(char* _buffer, int _length, int _flags) = 0;
	virtual bool Close() = 0;
	virtual bool SetNonBlocking(bool _value) = 0;


	NetConnection GetNetConnection() {	return NetConnection(m_remoteAddress, m_remotePort); }
	std::string GetRemoteIpAddress() { return m_remoteAddress; }
	int GetRemotePort() { return m_remotePort; }
	int GetLocalPort() { return m_localPort; }

	static bool Initialize();
	static bool Shutdown();

	static ISocket* CreateISocket(int domain, int type, int protocol);

protected:

#pragma warning( disable : 4251 )

	std::string m_remoteAddress;
	int m_remotePort;
	int m_localPort;

	bool m_socketOpen;

#pragma warning( default : 4251 )

};


#endif