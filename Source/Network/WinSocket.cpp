#ifdef WIN32

#include "Network/WinSocket.h"
#include <WS2tcpip.h>

bool WinSocket::g_initialized = false;
unsigned int WinSocket::g_noActiveSockets = 0;

WinSocket::WinSocket(SOCKET _socket)
{
	m_socket = _socket;
	if (m_socket != INVALID_SOCKET)
	{
		m_socketOpen = true;
		m_remoteAddress = "";
		m_remotePort = 0;
		m_localPort = 0;

		g_noActiveSockets++; // Check if a lock is requiured
	}
	else if (NET_DEBUG)
		printf("Failed to create new winsocket(2).\n");
}

WinSocket::WinSocket(int _domain, int _type, int _protocol)
{
	m_socket = socket(_domain, _type, _protocol);
	if (m_socket != INVALID_SOCKET)
	{
		m_socketOpen = true;
		m_remoteAddress = "";
		m_remotePort = 0;
		m_localPort = 0;

		g_noActiveSockets++; // Check if a lock is required
	}
	else if(NET_DEBUG)
		printf("Failed to create new winsocket.\n");
}

WinSocket::~WinSocket()
{
	Close();
	Shutdown();
}

bool WinSocket::Initialize()
{
	if (g_initialized)
		return true;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 0), &wsa) != 0)
	{
		if(NET_DEBUG)
			std::printf("Failed to initialize winsocket. Error Code: %d.\n", WSAGetLastError());
		return false;
	}

	g_initialized = true;
	return true;
}

bool WinSocket::Shutdown()
{
	if (!g_initialized && g_noActiveSockets == 0)
		return true;

	if (WSACleanup() != 0)
	{
		if (NET_DEBUG)
			std::printf("Failed to shutdown winsocket. Error Code: %d.\n", WSAGetLastError());
		return false;
	}

	g_initialized = false;
	return true;

}


bool WinSocket::Connect(const char* _ip, const int _port)
{ 
	if (!g_initialized)
	{
		if (NET_DEBUG)
			printf("Tried to connect without initializing.\n");

		Initialize();
	}

	addrinfo hints = { 0 };
	hints.ai_flags = AI_NUMERICHOST;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	addrinfo *addrs = NULL;
	if (getaddrinfo(_ip, NULL, &hints, &addrs) != 0)
	{
		if (NET_DEBUG)
		{
			std::printf("Failed to get address info. Error Code: %d.\n", WSAGetLastError());
		}

		return false;
	}

	sockaddr_in address;
	address.sin_addr.S_un.S_addr = ((sockaddr_in*)(addrs->ai_addr))->sin_addr.s_addr;
	address.sin_port = htons(_port);
	address.sin_family = AF_INET;

	freeaddrinfo(addrs);

	if (connect(m_socket, (sockaddr*)&address, sizeof(address)) < 0)
	{
		if (NET_DEBUG)
		{
			printf("Failed to connect to Ip address %s:%i. Error Code: %d.\n", _ip, _port, WSAGetLastError());
		}
		return false;
	}

	m_remoteAddress = _ip;
	m_remotePort = _port;

	return true;
}

bool WinSocket::Bind(const int _port)
{ 
	if (!g_initialized)
	{
		if (NET_DEBUG)
			printf("Tried to connect without initializing.\n");

		Initialize();
	}


	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(_port);
	address.sin_addr.S_un.S_addr = INADDR_ANY;

	if (bind(m_socket, (sockaddr*)(&address), sizeof(address)) != 0)
	{
		if (NET_DEBUG)
			printf("Failed to bind socket. Error Code: %d.\n", WSAGetLastError());

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
			printf("Failed to enable TCP_NODELAY. Error Code: %d.\n", WSAGetLastError());

		return false;
	}

	return true;
}

bool WinSocket::Close()
{
	if (closesocket(m_socket) != 0)
	{
		if (NET_DEBUG)
			printf("Failed to close winsocket. Error Code: %d.\n", WSAGetLastError());
		return false;
	}
	m_socketOpen = false;
	g_noActiveSockets--;
	return true;
}

ISocket* WinSocket::Accept()
{ 
	sockaddr_in incomingAddress;
	int incomingAddressLength = sizeof(incomingAddress);
	SOCKET newSocket = INVALID_SOCKET;


	u_long NonBlock = 1;
	if (ioctlsocket(m_socket, FIONBIO, &NonBlock) == SOCKET_ERROR)
	{
		printf("Failed to set nonblock.\n");
	}
		newSocket = accept(m_socket, (sockaddr*)&incomingAddress, &incomingAddressLength);

	if (newSocket == INVALID_SOCKET)
	{
		int error = WSAGetLastError();

		if (error != 10035 && NET_DEBUG)
			printf("Accept failed. Error Code: %d.\n", error);

		return NULL;
	}

	WinSocket* sock = new WinSocket(newSocket);
	char s[INET6_ADDRSTRLEN];
	inet_ntop(incomingAddress.sin_family, &incomingAddress.sin_addr, s, sizeof(s));

	sockaddr_in sin;
	socklen_t len = sizeof(sin);
	if (getsockname(newSocket, (sockaddr *)&sin, &len) == 0)
		sock->m_localPort = ntohs(sin.sin_port);

	sock->m_remoteAddress = s;
	sock->m_remotePort = incomingAddress.sin_port;


	int flag = 1;
	if (setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int)) < 0)
	{
		if (NET_DEBUG)
			printf("Failed to enable TCP_NODELAY. Error Code: %d.\n", WSAGetLastError());

		return false;
	}


	return sock;
}

bool WinSocket::Listen(int _backlog)
{ 
	if (!g_initialized)
	{
		if (NET_DEBUG)
			printf("Tried to connect without initializing.\n");

		Initialize();
	}

	int result = listen(m_socket, 128);

	if (result == SOCKET_ERROR)
	{
		if(NET_DEBUG)
			printf("Failed to start listen. Error Code: %d.\n", WSAGetLastError());
		return false;
	}
	return true;
}

int WinSocket::Recv(char* _buffer, int _length, int _flags)
{ 
	return recv(m_socket, _buffer, _length, _flags);
}

int WinSocket::Send(char* _buffer, int _length, int _flags)
{ 
	int result = send(m_socket, _buffer, _length, _flags);
	if (result == SOCKET_ERROR) 
	{
		if(NET_DEBUG)
			printf("Failed to send packet of size '%i'. Error Code: %d.\n", _length, WSAGetLastError());

		return -1;
	}
	return result;
}
#endif