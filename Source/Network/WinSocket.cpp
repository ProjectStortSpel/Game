#ifdef WIN32

#include <WS2tcpip.h>

#include "WinSocket.h"

using namespace Network;

WinSocket::WinSocket()
{
	m_remoteAddress = new std::string();
	m_remotePort = new int(0);
	m_localPort = new int(0);
	m_active = new int(0);

	Initialize();

	m_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_socket != INVALID_SOCKET)
	{
		g_noActiveSockets++;
		if (NET_DEBUG > 0)
			DebugLog("New winsocket created.", LogSeverity::Info);
	}
	else if (NET_DEBUG > 0)
		DebugLog("Failed to create new winsocket. Error code: %d.", LogSeverity::Error, WSAGetLastError());

}
WinSocket::WinSocket(SOCKET _socket)
{
	m_remoteAddress = new std::string();
	m_remotePort = new int(0);
	m_localPort = new int(0);
	m_active = new int(0);

	Initialize();

	m_socket = _socket;

	if (m_socket != INVALID_SOCKET)
	{
		g_noActiveSockets++;
		if (NET_DEBUG > 0)
			DebugLog("New winsocket created.", LogSeverity::Info);
	}
	else if (NET_DEBUG > 0)
		DebugLog("Failed to create new winsocket. Error code: %d.", LogSeverity::Error, WSAGetLastError());

}
WinSocket::WinSocket(int _domain, int _type, int _protocol)
{
	m_remoteAddress = new std::string();
	m_remotePort = new int(0);
	m_localPort = new int(0);
	m_active = new int(0);

	Initialize();

	m_socket = socket(_domain, _type, _protocol);

	if (m_socket != INVALID_SOCKET)
	{
		g_noActiveSockets++;
		if (NET_DEBUG > 0)
			DebugLog("New winsocket created.", LogSeverity::Info);
	}
	else if (NET_DEBUG > 0)
		DebugLog("Failed to create new winsocket. Error code: %d.", LogSeverity::Error, WSAGetLastError());
}

WinSocket::~WinSocket()
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
		if (NET_DEBUG > 0)
			DebugLog("Failed to initialize winsock. Error code: %d.", LogSeverity::Error, WSAGetLastError());

		return false;
	}

	if (NET_DEBUG > 0)
		DebugLog("WINSOCK initialized.", LogSeverity::Info);

	g_initialized = true;
	return true;
}

bool WinSocket::Shutdown(void)
{
	if (!g_initialized || g_noActiveSockets > 0)
		return true;

	if (WSACleanup() != 0)
	{
		if (NET_DEBUG > 0)
			DebugLog("Failed to shutdown winsock. Error code: %d.", LogSeverity::Error, WSAGetLastError());

		return false;
	}

	if (NET_DEBUG > 0)
		DebugLog("WINSOCK shutdown.", LogSeverity::Info);

	g_initialized = false;
	return true;
}

bool WinSocket::CloseSocket(void)
{
	if (closesocket(m_socket) != 0)
	{
		if (NET_DEBUG > 0)
			DebugLog("Failed to close winsocket. Error code: %d.", LogSeverity::Error, WSAGetLastError());

		return false;
	}

	g_noActiveSockets--;
	if (NET_DEBUG > 0)
		DebugLog("Winsocket closed.", LogSeverity::Info);

	return true;
}
bool WinSocket::ShutdownSocket(int _how)
{
	if (shutdown(m_socket, _how) != 0)
	{
		int errorCode = WSAGetLastError();

		if (errorCode == 10057)
			return true;
		else if (NET_DEBUG > 0)
			DebugLog("Failed to shutdown winsocket. Error code: %d.", LogSeverity::Error, WSAGetLastError());

		return false;
	}

	if (NET_DEBUG > 0)
		DebugLog("Winsocket shutdown.", LogSeverity::Info);

	return true;
}


bool WinSocket::Connect(const char* _ipAddres, const int _port)
{
	addrinfo hints = { 0 };
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	addrinfo* addrs = 0;
	if (getaddrinfo(_ipAddres, std::to_string(_port).c_str(), &hints, &addrs) != 0)
	{
		if (NET_DEBUG > 0)
			DebugLog("Failed to get address info. Error code: %d.", LogSeverity::Error, WSAGetLastError());

		freeaddrinfo(addrs);
		return false;
	}

	addrinfo* rp;
	int errorCode = 0;
	for (rp = addrs; rp != 0; rp = rp->ai_next)
	{
		m_socket = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

		if (m_socket < 0)
			continue;

		int cnt = connect(m_socket, rp->ai_addr, rp->ai_addrlen);

		if (cnt == 0)
			break;
		else if (cnt < 0)
		{
			errorCode = WSAGetLastError();
			 if (errorCode == 10035)
				break;
		}

		closesocket(m_socket);
	}

	freeaddrinfo(addrs);

	if (rp == 0)
	{
		if (NET_DEBUG > 0)
			DebugLog("Failed to connect to %s:%d. Error code: %i.", LogSeverity::Error, _ipAddres, _port, errorCode);

		return false;
	}


	*m_remoteAddress = _ipAddres;
	*m_remotePort = _port;

	if (NET_DEBUG > 0)
		DebugLog("Connected to %s:%d.", LogSeverity::Info, _ipAddres, _port);

	return true;
}
bool WinSocket::Bind(const int _port)
{
	sockaddr_in address;
	address.sin_family = PF_INET;
	address.sin_port = htons(_port);
	address.sin_addr.S_un.S_addr = INADDR_ANY;

	if (bind(m_socket, (sockaddr*)(&address), sizeof(address)) != 0)
	{
		if (NET_DEBUG > 0)
			DebugLog("Failed to bind socket. Error code: %d.", LogSeverity::Error, WSAGetLastError());

		return false;
	}

	sockaddr_in sin;
	socklen_t len = sizeof(sin);

	if (getsockname(m_socket, (sockaddr*)&sin, &len) == 0)
		*m_localPort = ntohs(sin.sin_port);
	else
	{
		if (NET_DEBUG > 0)
			DebugLog("Failed to get local port for socket. Error code: %d.", LogSeverity::Error, WSAGetLastError());

		return false;
	}

	return true;
}
bool WinSocket::Listen(int _backlog)
{
	if (listen(m_socket, _backlog) < 0)
	{
		if (NET_DEBUG > 0)
			DebugLog("Failed to put socket in listen mode. Error code: %d.", LogSeverity::Error, WSAGetLastError());

		return false;
	}

	return true;
}

ISocket* WinSocket::Accept(void)
{
	sockaddr_in incomingAddress;
	int incomingAddressSize = sizeof(incomingAddress);

	SOCKET incomingSocket = accept(m_socket, (sockaddr*)&incomingAddress, &incomingAddressSize);

	if (incomingSocket == INVALID_SOCKET)
	{
		int errorCode = WSAGetLastError();

		if (errorCode == 10035)
			return 0;

		if (NET_DEBUG > 0)
			DebugLog("Accept failed. Error code: %d.", LogSeverity::Error, WSAGetLastError());

		return 0;
	}

	char value = 1;
	if (setsockopt(incomingSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value)) < 0)
	{
		if (NET_DEBUG > 0)
			DebugLog("Failed to set TCP_NODELAY on new socket. Error code: %d.", LogSeverity::Error, WSAGetLastError());

		return 0;
	}

	WinSocket* newWinSocket = new WinSocket(incomingSocket);

	char ipAddress[INET6_ADDRSTRLEN];
	inet_ntop(incomingAddress.sin_family, &incomingAddress.sin_addr, ipAddress, sizeof(ipAddress));

	sockaddr_in sin;
	socklen_t len = sizeof(sin);

	if (getsockname(incomingSocket, (sockaddr*)&sin, &len) == 0)
		*newWinSocket->m_localPort = ntohs(sin.sin_port);
	else
	{
		if (NET_DEBUG > 0)
			DebugLog("Failed to get local port from new socket. Socket error: %d.", LogSeverity::Error, WSAGetLastError());

		return false;
	}

	*newWinSocket->m_remoteAddress = ipAddress;
	*newWinSocket->m_remotePort = incomingAddress.sin_port;
	*newWinSocket->m_active = 1;

	if (NET_DEBUG > 0)
		DebugLog("New winsocket from %s:%d accepted.", LogSeverity::Info, ipAddress, incomingAddress.sin_port);

	return newWinSocket;
}

int WinSocket::Send(char* _buffer, int _length, int _flags)
{
	short len = htons(_length);
	short totalDataSent = 0;


	int bytesSent = send(m_socket, (char*)&len, 2, _flags);
	if (bytesSent == 2)
	{
		bytesSent = send(m_socket, _buffer, _length, _flags);
		totalDataSent = bytesSent + 2;

		if (NET_DEBUG == 2)
		{
			if (bytesSent > 0)
				DebugLog("Sent packet of size %d.", LogSeverity::Info, bytesSent);
			else if (bytesSent == 0)
				DebugLog("Unable to send packet. Socket shutdown gracefully.", LogSeverity::Info);
			else if (bytesSent < 0)
				DebugLog("Failed to send packet of size %d. Error code: %d.", LogSeverity::Error, bytesSent, WSAGetLastError());
		}

	}
	else if (bytesSent == 0 && NET_DEBUG == 2)
		DebugLog("Unable to send header packet. Socket shutdown gracefully.", LogSeverity::Info);
	else if (bytesSent < 0 && NET_DEBUG == 2)
		DebugLog("Failed to send header packet of size %d. Error code: %d", LogSeverity::Info, bytesSent, WSAGetLastError());

	return totalDataSent;
}

int WinSocket::Receive(char* _buffer, int _length, int _flags)
{
	short len;
	int headerLen = recv(m_socket, (char*)&len, 2, MSG_WAITALL);
	short totalDataReceived = 0;

	if (headerLen == 2)
	{
		len = ntohs(len);
		int sizeReceived = recv(m_socket, _buffer, (int)len, MSG_WAITALL);
		totalDataReceived = sizeReceived + 2;

		if (sizeReceived < len)
		{
			if (NET_DEBUG == 2)
				DebugLog("Received packet of incorrect size. Got %d, expected %d.", LogSeverity::Warning, len, _length);

			return -1;
		}

		if (len > _length)
		{
			if (NET_DEBUG == 2)
				DebugLog("Received packet of incorrect size. Got %d, expected %d.", LogSeverity::Warning, len, _length);

			return -1;
		}

		if (NET_DEBUG == 2)
			DebugLog("Received packet with size %d.", LogSeverity::Info, sizeReceived);

		return totalDataReceived;

	}
	else if (headerLen == 0)
	{
		if (NET_DEBUG == 2)
			DebugLog("Unable to receive header packet. Socket shutdown gracefully.", LogSeverity::Info);
		return 0;
	}
	else
	{
		if (NET_DEBUG == 2)
			DebugLog("Failed to receive header packet. Error code: %d.", LogSeverity::Error, WSAGetLastError());
		return -1;
	}

	return -1;
}

bool WinSocket::SetNonBlocking(bool _value)
{
	u_long value = _value;

	if (ioctlsocket(m_socket, FIONBIO, &value) < 0)
	{
		if (NET_DEBUG > 0)
			DebugLog("Failed to %s non blocking on socket. Error code: %d.", LogSeverity::Error, _value ? "enable" : "disable", WSAGetLastError());
		return false;
	}

	return true;
}

bool WinSocket::SetNoDelay(bool _value)
{
	char value = _value;

	if (setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value)) < 0)
	{
		if (NET_DEBUG > 0)
			DebugLog("Failed to %s no delay on socket. Error code: %d.", LogSeverity::Error, _value ? "enable" : "disable", WSAGetLastError());
		return false;
	}

	return true;
}

bool WinSocket::SetTimeoutDelay(int _value, bool _recv, bool _send)
{
	int timeout = _value;
	bool success = true;

	if (_recv)
	{
		if (setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0)
		{
			if (NET_DEBUG > 0)
				DebugLog("Failed to set %d timeout delay on receive. Error code: %d", LogSeverity::Error, _value, WSAGetLastError());
			success = false;
		}
	}

	if (_send)
	{
		if (setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout)) < 0)
		{
			if (NET_DEBUG > 0)
				DebugLog("Failed to set %d timeout delay on send. Error code: %d", LogSeverity::Error, _value, WSAGetLastError());
			success = false;
		}
	}

	return success;
}

#endif