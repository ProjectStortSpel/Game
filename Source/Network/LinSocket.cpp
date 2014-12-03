#ifndef WIN32

#include "LinSocket.h"

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

using namespace Network;

LinSocket::LinSocket()
{
	Initialize();

	m_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (m_socket != -1)
	{
		m_socketOpen = true;
		m_remoteAddress = "";
		m_remotePort = 0;
		m_localPort = 0;
		m_active = 1;

		g_noActiveSockets++;
	}
	else if (NET_DEBUG)
		printf("Failed to create new linsocket.\n");
}

LinSocket::LinSocket(int _socket)
{
	Initialize();

	m_socket = _socket;

	if (m_socket != -1)
	{
		m_socketOpen = true;
		m_remoteAddress = "";
		m_remotePort = 0;
		m_localPort = 0;

		g_noActiveSockets++;
	}
	else if (NET_DEBUG)
		printf("Failed to create new linsocket.\n");
}

LinSocket::LinSocket(int _domain, int _type, int _protocol)
{
	Initialize();

	m_socket = socket(_domain, _type, _protocol);

	if (m_socket != -1)
	{
		m_socketOpen = true;
		m_remoteAddress = "";
		m_remotePort = 0;
		m_localPort = 0;

		g_noActiveSockets++;
	}
	else if (NET_DEBUG)
		printf("Failed to create new linsocket.\n");
}

LinSocket::~LinSocket()
{
	CloseSocket();
	Shutdown();
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
			std::printf("Failed to get address info. Error: %s.\n", strerror(errno));
		return false;
	}

	if (connect(m_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		if (NET_DEBUG)
			printf("Failed to connect to Ip address %s:%i. Error: %s.\n", _ip, _port, strerror(errno));
		return false;
	}

	m_remoteAddress = _ip;
	m_remotePort = _port;

	return true;
}
bool LinSocket::Bind(const int _port)
{
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(_port);
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(m_socket, (sockaddr *)&address, sizeof(address)) < 0)
	{
		if (NET_DEBUG)
			printf("Failed to bind socket. Error: %s.\n", strerror(errno));
		return false;
	}

	sockaddr_in sin;
	socklen_t len = sizeof(sin);
	if (getsockname(m_socket, (sockaddr *)&sin, &len) == 0)
		m_localPort = ntohs(sin.sin_port);


	int flag = 1;
	if (setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int)) < 0)
	{
		if (NET_DEBUG)
			printf("Failed to enable TCP_NODELAY. Error: %s.\n", strerror(errno));

		return false;
	}


	return true;
}
bool LinSocket::Listen(int _backlog)
{
	int result = listen(m_socket, _backlog);

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
	if( ioctl(m_socket, FIONBIO, &opt) != 0)
	{
		if(NET_DEBUG)
			printf("Failed to set nonblocking mode. Error: %s.\n", strerror(errno));
	}

	return true;
}
bool LinSocket::CloseSocket()
{
	if (close(m_socket) != 0)
	{
		if (NET_DEBUG)
			printf("Failed to close linsocket. Error: %s.\n", strerror(errno));
		return false;
	}
	m_socketOpen = false;
	g_noActiveSockets--;

	return true;
}
ISocket* LinSocket::Accept()
{
	sockaddr_in incomingAddress;
	socklen_t incomingAddressLength = sizeof(incomingAddress);
	int newSocket = -1;
	newSocket = accept(m_socket, (sockaddr*)&incomingAddress, &incomingAddressLength);

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
		sock->m_localPort = ntohs(sin.sin_port);

	sock->m_remoteAddress = s;
	sock->m_remotePort = incomingAddress.sin_port;

	return sock;
}

int LinSocket::Receive(char* _buffer, int _length, int _flags)
{
	return recv(m_socket, (void*)_buffer, _length, _flags);
}

int LinSocket::Send(char* _buffer, int _length, int _flags)
{
	int result = send(m_socket, (void*)_buffer, _length, _flags);
	if (result == -1) 
	{
		if (NET_DEBUG)
			printf("Failed to send packet of size '%i'. Error: %s.\n", _length, strerror(errno));

		return -1;
	}
	return result;
}

#endif