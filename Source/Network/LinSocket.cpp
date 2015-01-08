#ifndef WIN32

#include "LinSocket.h"

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>

using namespace Network;

LinSocket::LinSocket()
{
	m_remoteAddress = new std::string("");
	m_remotePort = new int(0);
	m_localPort = new int(0);
	m_active = new int(1);
	m_socket = new int(0);

	Initialize();

	*m_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (*m_socket != -1)
	{
		*m_remoteAddress = "";
		*m_remotePort = 0;
		*m_localPort = 0;
		*m_active = 1;

		g_noActiveSockets++;
	}
	else if (NET_DEBUG)
		printf("Failed to create new linsocket.\n");
}

LinSocket::LinSocket(int _socket)
{
	m_remoteAddress = new std::string("");
	m_remotePort = new int(0);
	m_localPort = new int(0);
	m_active = new int(1);
	m_socket = new int(0);

	Initialize();

	*m_socket = _socket;

	if (*m_socket != -1)
	{
		*m_remoteAddress = "";
		*m_remotePort = 0;
		*m_localPort = 0;

		g_noActiveSockets++;
	}
	else if (NET_DEBUG)
		printf("Failed to create new linsocket.\n");
}

LinSocket::LinSocket(int _domain, int _type, int _protocol)
{
	m_remoteAddress = new std::string("");
	m_remotePort = new int(0);
	m_localPort = new int(0);
	m_active = new int(1);
	m_socket = new int(0);

	Initialize();

	*m_socket = socket(_domain, _type, _protocol);

	if (*m_socket != -1)
	{
		*m_remoteAddress = "";
		*m_remotePort = 0;
		*m_localPort = 0;

		g_noActiveSockets++;
	}
	else if (NET_DEBUG)
		printf("Failed to create new linsocket.\n");
}

LinSocket::~LinSocket()
{
	CloseSocket();
	Shutdown();

	SAFE_DELETE(m_remoteAddress);
	SAFE_DELETE(m_remotePort);
	SAFE_DELETE(m_localPort);
	SAFE_DELETE(m_active);
	SAFE_DELETE(m_socket);
}

bool LinSocket::Initialize()
{
	g_initialized = true;
	return true;
}

bool LinSocket::Shutdown()
{
	g_initialized = false;
	return true;
}


bool LinSocket::Connect(const char* _ip, const int _port)
{
	sockaddr_in address;

	memset(&address, '0', sizeof(address));

	address.sin_family = AF_INET;
	address.sin_port = htons(_port);

	if (inet_pton(AF_INET, _ip, &address.sin_addr) <= 0)
	{
		if (NET_DEBUG)
			printf("Failed to get address info. Error: %s.\n", strerror(errno));
		return false;
	}

	if (connect(*m_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		if (NET_DEBUG)
			printf("Failed to connect to Ip address %s:%i. Error: %s.\n", _ip, _port, strerror(errno));
		return false;
	}

	*m_remoteAddress = _ip;
	*m_remotePort = _port;

	return true;
}
bool LinSocket::Bind(const int _port)
{
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(_port);
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(*m_socket, (sockaddr *)&address, sizeof(address)) < 0)
	{
		if (NET_DEBUG)
			printf("Failed to bind socket. Error: %s.\n", strerror(errno));
		return false;
	}

	sockaddr_in sin;
	socklen_t len = sizeof(sin);
	if (getsockname(*m_socket, (sockaddr *)&sin, &len) == 0)
		*m_localPort = ntohs(sin.sin_port);

	return true;
}
bool LinSocket::Listen(int _backlog)
{
	int result = listen(*m_socket, _backlog);

	if (result == -1)
	{
		if (NET_DEBUG)
			printf("Failed to start listen. Error: %s.\n", strerror(errno));
		return false;
	}
	return true;
}
bool LinSocket::SetNonBlocking(bool _value)
{
	int opt = _value;
	if( ioctl(*m_socket, FIONBIO, &opt) != 0)
	{
		if(NET_DEBUG)
			printf("Failed to set nonblocking mode. Error: %s.\n", strerror(errno));
	}

	return true;
}
bool LinSocket::SetNoDelay(bool _value)
{
	int flag = _value;
	if (setsockopt(*m_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int)) < 0)
	{
		if (NET_DEBUG)
			printf("Failed to enable TCP_NODELAY. Error: %s.\n", strerror(errno));

		return false;
	}

	return true;
}

bool LinSocket::SetTimeoutDelay(int _value)
{
	int timeout = _value;

	if (setsockopt(*m_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) != 0)
		return false;

	if (setsockopt(*m_socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout)) != 0)
		return false;
}

bool LinSocket::CloseSocket()
{
	if (close(*m_socket) != 0)
	{
		if (NET_DEBUG)
			printf("Failed to close linsocket. Error: %s.\n", strerror(errno));
		return false;
	}
	g_noActiveSockets--;

	return true;
}
ISocket* LinSocket::Accept()
{
	sockaddr_in incomingAddress;
	socklen_t incomingAddressLength = sizeof(incomingAddress);
	int newSocket = -1;
	newSocket = accept(*m_socket, (sockaddr*)&incomingAddress, &incomingAddressLength);

	if (newSocket == -1)
	{
		int errorCode = errno;

		if (errorCode != 11 && NET_DEBUG)
			printf("Accept failed. Error: %s.\n", strerror(errorCode));
		return NULL;
	}

	LinSocket* sock = new LinSocket(newSocket);
	char s[INET6_ADDRSTRLEN];
	inet_ntop(incomingAddress.sin_family, &incomingAddress.sin_addr, s, sizeof(s));

	sockaddr_in sin;
	socklen_t len = sizeof(sin);
	if (getsockname(newSocket, (sockaddr *)&sin, &len) == 0)
		*sock->m_localPort = ntohs(sin.sin_port);

	*sock->m_remoteAddress = s;
	*sock->m_remotePort = incomingAddress.sin_port;

	return sock;
}
/*
int LinSocket::Receive(char* _buffer, int _length, int _flags)
{
	return recv(*m_socket, (void*)_buffer, _length, _flags);
}

int LinSocket::Send(char* _buffer, int _length, int _flags)
{
	int result = send(*m_socket, (void*)_buffer, _length, _flags);
	if (result == -1) 
	{
		if (NET_DEBUG)
			printf("Failed to send packet of size '%i'. Error: %s.\n", _length, strerror(errno));

		return -1;
	}
	return result;
}
*/
//
//int LinSocket::Receive(char* _buffer, int _length, int _flags)
//{
//
//	static short len;
//
//	if (recv(*m_socket, (void*)&len, 2, MSG_WAITALL))
//	{
//		len = ntohs(len);
//		return recv(*m_socket, (void*)_buffer, len, MSG_WAITALL);
//	}
//	return 0;
//}

int LinSocket::Receive(char* _buffer, int _length, int _flags)
{
	short len;
	int len2 = recv(*m_socket, (void*)&len, 2, MSG_WAITALL);
	if (len2 == 2)
	{
		len = ntohs(len);
		int sizeReceived = recv(*m_socket, (void*)_buffer, len, MSG_WAITALL);

		if (sizeReceived != len)
		{
			if (NET_DEBUG)
				printf("Error: Wrong packet size on received packet!\n");
			//return 0;
		}

		if (len > _length)
		{
			if (NET_DEBUG)
				printf("Error: To large packet received!\n");
			return 0;
		}

		return sizeReceived;
	}
	else if (len2 == -1)
	{
		if (NET_DEBUG)
			printf("Error: Failed to receive \"Size packet\". Error: %s.\n", strerror(errno));
	}
	else
	{
		if (NET_DEBUG)
			printf("Error: \"Size packet\" corrupt! Length: %d\n", len2);
		//return 0;
	}
	return 0;
}

int LinSocket::Send(char* _buffer, int _length, int _flags)
{
	static short len = 0;
	len = htons(_length);
	if (send(*m_socket, (void*)&len, 2, _flags) != -1)
	{
		int result = send(*m_socket, (void*)_buffer, _length, _flags);
		if (result == -1)
		{
			if (NET_DEBUG)
				printf("Failed to send packet of size '%i'. Error: %s.\n", _length, strerror(errno));

			return -1;
		}
		return result;
	}
	return -1;
}

#endif