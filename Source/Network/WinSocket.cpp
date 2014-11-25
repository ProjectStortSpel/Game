#ifdef _WIN32
#include "Network/WinSocket.h"
#include <WS2tcpip.h>

WinSocket::WinSocket(int _domain, int _type, int _protocol)
{
	m_socket = socket(_domain, _type, _protocol);
	m_remoteIP = "";
	m_remotePort = 0;
}

WinSocket::~WinSocket()
{

}

bool WinSocket::Initialize()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		std::printf("Failed. Error Code : %d", WSAGetLastError());
		return false;
	}
	return true;
}


bool WinSocket::Connect(const char* _ip, const int _port)
{ 
	addrinfo hints = { 0 };
	hints.ai_flags = AI_NUMERICHOST;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	addrinfo *addrs = NULL;
	if (getaddrinfo(_ip, NULL, &hints, &addrs) != 0)
	{
		return false;
	}

	sockaddr_in address;
	address.sin_addr.S_un.S_addr = ((sockaddr_in*)(addrs->ai_addr))->sin_addr.s_addr;
	address.sin_port = htons(_port);
	address.sin_family = AF_INET;

	freeaddrinfo(addrs);

	if (connect(m_socket, (sockaddr*)&address, sizeof(address)) != 0)
	{
		m_remoteIP = _ip;
		m_remotePort = _port;
		return false;
	}
	return true; 
}

bool WinSocket::Bind(const int _port)
{ 
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(_port);
	address.sin_addr.S_un.S_addr = INADDR_ANY;

	if (bind(m_socket, (sockaddr*)&address, sizeof(address)) != 0)
	{
		return false;
	}
	return true;
}

ISocket* WinSocket::Accept()
{ 
	sockaddr incomingAddress;
	int incomingAddressLength;
	accept(m_socket, &incomingAddress, &incomingAddressLength);

	return 0; 
}

bool WinSocket::Listen(int _backlog)
{ 
	return false; 
}

int WinSocket::Recv(void* _buffer, int _length, int _flags)
{ 
	return -1; 
}

int WinSocket::Send(void* _buffer, int _length, int _flags)
{ 
	return -1; 
}
#endif