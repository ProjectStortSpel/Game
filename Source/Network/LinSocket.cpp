#ifndef WIN32

#include "Network/LinSocket.h"

LinSocket::LinSocket(int _socket)
{
	m_socket = _socket;
}

LinSocket::LinSocket(int _domain, int _type, int _protocol)
{
	m_socket = socket(_domain, _type, _protocol);
	m_remoteAddress = "";
	m_remotePort = 0;
}

LinSocket::~LinSocket()
{
	Close();
}

bool LinSocket::Initialize()
{
	return true;
}

bool LinSocket::Shutdown()
{
	return true;
}


bool LinSocket::Connect(const char* _ip, const int _port)
{
	sockaddr_in address;

	memset(&address, '0', sizeof(address)); 

	address.sin_family = AF_INET;
	address.sin_port = htons(_port);

	if(inet_pton(AF_INET, _ip, &address.sin_addr)<=0)
	{
		if (NET_DEBUG)
			printf("\n inet_pton error occured\n");
		return false;
	} 

	if( connect(m_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		if (NET_DEBUG)
			printf("Failed to connect to Ip address %s:%i.\n", _ip, _port);
		return false;
	} 
	return true; 
}

bool LinSocket::Bind(const int _port)
{
	printf("Bind.\n");
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(_port);
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(m_socket, (struct sockaddr *) &address, sizeof(address)) < 0)
	{
		if (NET_DEBUG)
			printf("ERROR on binding");
		return false;
	}

	m_localPort = _port;

	return true;
}

bool LinSocket::Close()
{
	if(close(m_socket) != 0)
	{
		if(NET_DEBUG)
			printf("Failed to close linuxsocket. Error: %s.\n", strerror(errno));

		return false;
	}

	return true;
}

ISocket* LinSocket::Accept()
{
	printf("Accept.\n");
	sockaddr_in incomingAddress;
	socklen_t incomingAddressLength = sizeof(incomingAddress);
	int newSocket = -1;
	newSocket = accept(m_socket, (sockaddr*)&incomingAddress, &incomingAddressLength);


	if (newSocket == -1)
	{
		if (NET_DEBUG)
			printf("Accept failed.\n");
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

bool LinSocket::Listen(int _backlog)
{
	printf("Listen.\n");
	int result = listen(m_socket, 128);

	if (result == -1)
	{
		if(NET_DEBUG)
			printf("Failed to start listen.\n");
		return false;
	}
	return true;
}

int LinSocket::Recv(char* _buffer, int _length, int _flags)
{
	return recv(m_socket, (void*)_buffer, _length, _flags);
}

int LinSocket::Send(char* _buffer, int _length, int _flags)
{
	int result = send(m_socket, (void*)_buffer, _length, _flags);
	if (result == -1) 
	{
		if (NET_DEBUG)
			printf("Failed to send packet of size '%i'.\n", _length);

		return -1;
	}
	return result;
}

#endif