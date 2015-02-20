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

	m_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_socket != INVALID_SOCKET)
	{
		*m_remoteAddress = "";
		*m_remotePort = 0;
		*m_localPort = 0;
		*m_active = 1;

		g_noActiveSockets++;
	}
	else if (NET_DEBUG)
		SDL_Log("Failed to create new winsocket.\n");

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
		SDL_Log("Failed to create new winsocket.\n");
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
		SDL_Log("Failed to create new winsocket.\n");
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
			SDL_Log("Failed to initialize winsock. Error Code: %d.\n", WSAGetLastError());
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
			SDL_Log("Failed to shutdown winsock. Error Code: %d.\n", WSAGetLastError());
		return false;
	}

	g_initialized = false;
	return true;
}

bool WinSocket::Connect(const char* _ipAddress, const int _port)
{
	addrinfo hints = { 0 };
	//hints.ai_flags = AI_NUMERICHOST;
	hints.ai_family = AF_UNSPEC; //PF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	addrinfo *addrs = NULL;
	if (getaddrinfo(_ipAddress, std::to_string(_port).c_str(), &hints, &addrs) != 0)
	{
		if (NET_DEBUG)
		{
			SDL_Log("Failed to get address info. Error Code: %d.\n", WSAGetLastError());
		}
		freeaddrinfo(addrs);
		return false;
	}

	addrinfo* rp;

	for (rp = addrs; rp != NULL; rp = rp->ai_next)
	{
		m_socket = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

		if (m_socket < 0)
			continue;

		if (connect(m_socket, rp->ai_addr, rp->ai_addrlen) == 0)
			break;

		else if (connect(m_socket, rp->ai_addr, rp->ai_addrlen) < 0)
		{
			if (WSAGetLastError() == 10035)
				break;			
		}

		closesocket(m_socket);
	}

	freeaddrinfo(addrs);

	if (rp == NULL)
	{
		if (NET_DEBUG)
		{
			SDL_Log("Failed to connect to Ip address %s:%i.", _ipAddress, _port);
		}
		return false;
	}

	*m_remoteAddress = _ipAddress;
	*m_remotePort = _port;

	return true;

/*
	sockaddr_in address;
	address.sin_addr.S_un.S_addr = ((sockaddr_in*)(addrs->ai_addr))->sin_addr.s_addr;
	address.sin_port = htons(_port);
	address.sin_family = PF_INET;

	freeaddrinfo(addrs);

	if (connect(m_socket, (sockaddr*)&address, sizeof(address)) < 0)
	{
		int errorCode = WSAGetLastError();
		if (errorCode != 10035)
		{
			if (NET_DEBUG)
			{
				SDL_Log("Failed to connect to Ip address %s:%i. Error Code: %d.\n", _ipAddress, _port, errorCode);
			}
			return false;
		}
	}

	*m_remoteAddress = _ipAddress;
	*m_remotePort = _port;

	return true;*/
}
bool WinSocket::Bind(const int _port)
{
	sockaddr_in address;
	address.sin_family = PF_INET;
	address.sin_port = htons(_port);
	address.sin_addr.S_un.S_addr = INADDR_ANY;

	if (bind(m_socket, (sockaddr*)(&address), sizeof(address)) != 0)
	{
		if (NET_DEBUG)
			SDL_Log("Failed to bind socket. Error Code: %d.\n", WSAGetLastError());

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
			SDL_Log("Failed to start listen. Error Code: %d.\n", WSAGetLastError());
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
			SDL_Log("Failed to set nonblock.\n");
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
			SDL_Log("Failed to enable TCP_NODELAY on new socket. Error Code: %d.\n", WSAGetLastError());

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
			SDL_Log("Failed to close winsocket. Error Code: %d.\n", WSAGetLastError());
		return false;
	}

	g_noActiveSockets--;

	return true;
}

bool WinSocket::ShutdownSocket(int _how)
{
	if (shutdown(m_socket, _how) != 0)
	{
		if (NET_DEBUG)
			SDL_Log("Failed to shutdown winsocket. Error Code: %d.\n", WSAGetLastError());
		return false;
	}

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
			SDL_Log("Accept failed. Error Code: %d.\n", errorCode);

		return 0;
	}

	char value = 1;
	if (setsockopt(incomingSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value)) < 0)
	{
		if (NET_DEBUG)
			SDL_Log("Failed to enable TCP_NODELAY on new socket. Error Code: %d.\n", WSAGetLastError());

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
SDL_Log("Failed to send packet of size '%i'. Error Code: %d.\n", _length, WSAGetLastError());

return -1;
}
return result;
}
*/
/*
int WinSocket::Receive(char* _buffer, int _length, int _flags)
{
return recv(m_socket, _buffer, _length, _flags);
}
*/

int WinSocket::Send(char* _buffer, int _length, int _flags)
{
	short len = 0;
	len = htons(_length);

	int byteSent = send(m_socket, (char*)&len, 2, _flags);
	if (byteSent == 2)
	{
		byteSent = send(m_socket, _buffer, _length, _flags);
		if (byteSent == SOCKET_ERROR)
		{
			if (NET_DEBUG)
				SDL_Log("Failed to send packet of size '%i'. Error Code: %d.\n", _length, WSAGetLastError());

		}
	}
	else if (byteSent == SOCKET_ERROR)
	{
		int errorCode = WSAGetLastError();

		if (errorCode == 10057)
		{
		}
		else if (byteSent == 0)
		{
			SDL_Log("Server shutdown gracefully.\n");
		}
		else if (errorCode != 10035 && byteSent != 0)
		{
			if (NET_DEBUG)
				SDL_Log("Failed to send \"Size packet\" of size '%i'. Error Code: %d.\n", byteSent, errorCode);
		}
	}
	return byteSent;

}






//int WinSocket::Receive(char* _buffer, int _length, int _flags)
//{
//	return recv(m_socket, _buffer, _length, _flags);
//}

//int WinSocket::Receive(char* _buffer, int length, int _flags)
//{
//	short size = 0;
//
//	int dataReceived = 0;
//	do
//	{
//		size = recv(m_socket, _buffer + dataReceived, MAX_PACKET_SIZE, 0);
//		if (size > 0)
//			dataReceived += size;
//
//	} while (size > 0);
//
//	return dataReceived;
//}

int WinSocket::Receive(char* _buffer, int _length, int _flags)
{
	if (m_socket == INVALID_SOCKET)
		return -1;

	short len;
	int len2 = recv(m_socket, (char*)&len, 2, MSG_WAITALL);
	if (len2 == 2)
	{
		len = ntohs(len);
		int sizeReceived = recv(m_socket, _buffer, (int)len, MSG_WAITALL);

		if (sizeReceived != len)
		{
			if (NET_DEBUG)
				SDL_Log("Error: Wrong packet size on received packet!\n");
			//return 0;
		}

		if (len > _length)
		{
			if (NET_DEBUG)
				SDL_Log("Error: To large packet received!\n");
			return -1;
		}

		return sizeReceived;
	}
	else if (len2 == SOCKET_ERROR)
	{
		if (NET_DEBUG)
			SDL_Log("Error: Failed to receive \"Size packet\". Error code: %d\n", WSAGetLastError());

		return -1;
	}
	else if (len2 == 0)
	{
		if (NET_DEBUG)
			SDL_Log("Server shutdown gracefully.\n");

		return 0;
	}
	else
	{
		if (NET_DEBUG)
			SDL_Log("Error: \"Size packet\" corrupt! Length: %d\n", len2);
		return -1;
	}
	return 0;
}




#endif