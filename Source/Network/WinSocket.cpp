#ifdef WIN32

#include "WinSocket.h"

#include <WS2tcpip.h>

using namespace Network;

WinSocket::WinSocket(void)
{
	m_remoteAddress = new std::string("");
	m_remotePort = new int(0);
	m_localPort = new int(0);
	m_active = new int(1);

	Initialize();

	m_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (m_socket != INVALID_SOCKET)
	{
		*m_remoteAddress = "";
		*m_remotePort = 0;
		*m_localPort = 0;
		*m_active = 1;

		g_noActiveSockets++;
	}
	else if (NET_DEBUG)
		printf("Failed to create new winsocket.\n");

}

WinSocket::WinSocket(SOCKET _socket)
{
	m_remoteAddress = new std::string("");
	m_remotePort = new int(0);
	m_localPort = new int(0);
	m_active = new int(1);

	Initialize();

	m_socket = _socket;

	if (m_socket != INVALID_SOCKET)
	{
		*m_remoteAddress = "";
		*m_remotePort = 0;
		*m_localPort = 0;

		g_noActiveSockets++;
	}
	else if (NET_DEBUG)
		printf("Failed to create new winsocket.\n");
}

WinSocket::WinSocket(int _domain, int _type, int _protocol)
{
	m_remoteAddress = new std::string("");
	m_remotePort = new int(0);
	m_localPort = new int(0);
	m_active = new int(1);

	Initialize();

	m_socket = socket(_domain, _type, _protocol);

	if (m_socket != INVALID_SOCKET)
	{
		*m_remoteAddress = "";
		*m_remotePort = 0;
		*m_localPort = 0;

		g_noActiveSockets++;
	}
	else if (NET_DEBUG)
		printf("Failed to create new winsocket.\n");
}

WinSocket::~WinSocket(void)
{
	CloseSocket();
	Shutdown();

	SAFE_DELETE(m_remoteAddress);
	SAFE_DELETE(m_remotePort);
	SAFE_DELETE(m_localPort);
	SAFE_DELETE(m_active);

}

bool WinSocket::Initialize(void)
{
	if (g_initialized)
		return true;

	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
	{
		if (NET_DEBUG)
			printf("Failed to initialize winsock. Error Code: %d.\n", WSAGetLastError());
		return false;
	}

	g_initialized = true;
	return true;
}

bool WinSocket::Shutdown(void)
{
	if (!g_initialized || g_noActiveSockets > 0)
		return true;

	if (WSACleanup() != 0)
	{
		if (NET_DEBUG)
			printf("Failed to shutdown winsock. Error Code: %d.\n", WSAGetLastError());
		return false;
	}

	g_initialized = false;
	return true;
}

bool WinSocket::Connect(const char* _ipAddress, const int _port)
{
	addrinfo hints = { 0 };
	hints.ai_flags = AI_NUMERICHOST;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	addrinfo *addrs = NULL;
	if (getaddrinfo(_ipAddress, NULL, &hints, &addrs) != 0)
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
		int errorCode = WSAGetLastError();
		if (errorCode != 10035)
		{

			if (NET_DEBUG)
			{
				printf("Failed to connect to Ip address %s:%i. Error Code: %d.\n", _ipAddress, _port, errorCode);
			}
			return false;
		}
	}

	*m_remoteAddress = _ipAddress;
	*m_remotePort = _port;

	return true;
}
bool WinSocket::Bind(const int _port)
{
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
		*m_localPort = ntohs(sin.sin_port);

	return true;

}
bool WinSocket::Listen(int _backlog)
{
	int result = listen(m_socket, _backlog);

	if (result == SOCKET_ERROR)
	{
		if (NET_DEBUG)
			printf("Failed to start listen. Error Code: %d.\n", WSAGetLastError());
		return false;
	}
	return true;
}
bool WinSocket::SetNonBlocking(bool _value)
{
	u_long value = _value;
	if (ioctlsocket(m_socket, FIONBIO, &value) == SOCKET_ERROR)
	{
		if (NET_DEBUG)
			printf("Failed to set nonblock.\n");
		return false;
	}

	return true;
}

bool WinSocket::SetNoDelay(bool _value)
{
	char value = _value;
	if (setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value)) < 0)
	{
		if (NET_DEBUG)
			printf("Failed to enable TCP_NODELAY on new socket. Error Code: %d.\n", WSAGetLastError());

		return false;
	}

	return true;
}
bool WinSocket::SetTimeoutDelay(int _value)
{
	int timeout = _value;

	if (setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) != 0)
		return false;

	//if (setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout)) != 0)
	//	return false;

	return true;
}

bool WinSocket::CloseSocket(void)
{
	if (closesocket(m_socket) != 0)
	{
		if (NET_DEBUG)
			printf("Failed to close winsocket. Error Code: %d.\n", WSAGetLastError());
		return false;
	}

	g_noActiveSockets--;

	return true;
}
ISocket* WinSocket::Accept(void)
{
	sockaddr_in incomingAddress;
	int incomingAddressLength = sizeof(incomingAddress);

	SOCKET incomingSocket = INVALID_SOCKET;

	incomingSocket = accept(m_socket, (sockaddr*)&incomingAddress, &incomingAddressLength);
	if (incomingSocket == INVALID_SOCKET)
	{
		int errorCode = WSAGetLastError();
		if (errorCode != 10035 && NET_DEBUG)
			printf("Accept failed. Error Code: %d.\n", errorCode);

		return 0;
	}

	char value = 1;
	if (setsockopt(incomingSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value)) < 0)
	{
		if (NET_DEBUG)
			printf("Failed to enable TCP_NODELAY on new socket. Error Code: %d.\n", WSAGetLastError());

		return 0;
	}

	WinSocket* newWinSocket = new WinSocket(incomingSocket);
	char ipAddress[INET6_ADDRSTRLEN];
	inet_ntop(incomingAddress.sin_family, &incomingAddress.sin_addr, ipAddress, sizeof(ipAddress));

	sockaddr_in sin;
	socklen_t len = sizeof(sin);
	if (getsockname(incomingSocket, (sockaddr *)&sin, &len) == 0)
		*newWinSocket->m_localPort = ntohs(sin.sin_port);

	*newWinSocket->m_remoteAddress = ipAddress;
	*newWinSocket->m_remotePort = incomingAddress.sin_port;

	return newWinSocket;

}
/*
int WinSocket::Send(char* _buffer, int _length, int _flags)
{
	int result = send(m_socket, _buffer, _length, _flags);
	if (result == SOCKET_ERROR)
	{
		if (NET_DEBUG)
			printf("Failed to send packet of size '%i'. Error Code: %d.\n", _length, WSAGetLastError());

		return -1;
	}
	return result;
}
int WinSocket::Receive(char* _buffer, int _length, int _flags)
{
	return recv(m_socket, _buffer, _length, _flags);
}
*/


int WinSocket::Send(char* _buffer, int _length, int _flags)
{
	u_long packet_size = htonl(_length);

	if (send(m_socket, (char*)&packet_size, 4, 0) > 0)
	{
		int result = send(m_socket, _buffer, _length, 0);
		if (result < 0)
		{
			if (NET_DEBUG)
				printf("Failed to send packet of size '%iä. Error Code: %d.\n", _length, WSAGetLastError());

			return -1;
		}

		printf("Sent packet with size: %i.\n", result);
		return result;
	}

	return -1;
}
int WinSocket::Receive(char* _buffer, int _length, int _flags)
{
	u_long packet_size;
	int bytes_to_read = 4;
	int result;
	char* p_size = (char*)&packet_size;
	//NetSleep(0.5);
	while (bytes_to_read > 0)
	{
		result = recv(m_socket, p_size, bytes_to_read, 0);
		if (result > 0)
		{
			bytes_to_read -= result;
			p_size += result;
		}
		else if (result == 0 || this->m_active == 0)
		{
			printf("0\n");
			return 0;
		}
	}

	packet_size = ntohl(packet_size);
	bytes_to_read = packet_size;
	int totalSize = 0;
	while (bytes_to_read > 0)
	{
		result = recv(m_socket, _buffer, bytes_to_read, 0);
		if (result > 0)
		{
			bytes_to_read -= result;
			_buffer += result;
			totalSize += result;
		}
		else if (result == 0 || this->m_active == 0)
		{
			printf("0 no2\n");
			return 0;
		}
	}

	printf("Received packet with size: %i.\n", totalSize);
	return totalSize;
}




#endif