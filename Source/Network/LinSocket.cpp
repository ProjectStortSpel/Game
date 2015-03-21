#ifndef WIN32

#include "LinSocket.h"
  

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sstream>
#include <netdb.h>

using namespace Network;

LinSocket::LinSocket()
{
	m_remoteAddress = new std::string("");
	m_remotePort = new int(0);
	m_localPort = new int(0);
	m_active = new int(1);
	m_socket = new int(0);

	Initialize();

	*m_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

#if defined(__IOS__) || defined(__OSX__)
	int set = 1;
	setsockopt(*m_socket, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
#endif

	if (*m_socket != -1)
	{
	  
	    *m_remoteAddress  = "";
	    *m_remotePort = 0;
	    *m_localPort = 0;
	    *m_active = 0;
	  
		g_noActiveSockets++;
		if (NET_DEBUG > 0)
			DebugLog("New linsocket created.", LogSeverity::Info);
	}
	else if (NET_DEBUG > 0)
		DebugLog("Failed to create new linsocket. Error: %s.", LogSeverity::Error, strerror(errno));

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

#if defined(__IOS__) || defined(__OSX__)
	int set = 1;
	setsockopt(*m_socket, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
#endif

	if (*m_socket != -1)
	{
	    *m_remoteAddress = "";
	    *m_remotePort = 0;
	    *m_localPort = 0;
	    *m_active = 0;
	  
		g_noActiveSockets++;
		if (NET_DEBUG > 0)
			DebugLog("New linsocket created.", LogSeverity::Info);
	}
	else if (NET_DEBUG > 0)
		DebugLog("Failed to create new linsocket. Error: %s.", LogSeverity::Error, strerror(errno));

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

#if defined(__IOS__) || defined(__OSX__)
	int set = 1;
	setsockopt(*m_socket, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
#endif

	if (*m_socket != -1)
	{
	    *m_remoteAddress = "";
	    *m_remotePort = 0;
	    *m_localPort = 0;
	    *m_active = 0;
	  
	  
		g_noActiveSockets++;
		if (NET_DEBUG > 0)
			DebugLog("New linsocket created.", LogSeverity::Info);
	}
	else if (NET_DEBUG > 0)
		DebugLog("Failed to create new linsocket. Error: %s.", LogSeverity::Error, strerror(errno));
}

LinSocket::~LinSocket()
{
	CloseSocket();
	Shutdown();

	SAFE_DELETE(m_remoteAddress);
	SAFE_DELETE(m_remotePort);
	SAFE_DELETE(m_localPort);
	SAFE_DELETE(m_active);
}

bool LinSocket::Initialize(void)
{
	if (g_initialized)
		return true;

	if (NET_DEBUG > 0)
		DebugLog("LINSOCK initialized.", LogSeverity::Info);

	g_initialized = true;
	return true;
}

bool LinSocket::Shutdown(void)
{
	if (!g_initialized || g_noActiveSockets > 0)
		return true;

	if (NET_DEBUG > 0)
		DebugLog("LINSOCK shutdown.", LogSeverity::Info);

	g_initialized = false;
	return true;
}

bool LinSocket::CloseSocket(void)
{
	if (close(*m_socket) != 0)
	{
		if (NET_DEBUG > 0)
			DebugLog("Failed to close linsocket. Error: %s.", LogSeverity::Error, strerror(errno));

		return false;
	}

	g_noActiveSockets--;

	if (NET_DEBUG > 0)
		DebugLog("Linsocket closed.", LogSeverity::Info);

	return true;
}
bool LinSocket::ShutdownSocket(int _how)
{
	if (shutdown(*m_socket, _how) != 0)
	{
		if (NET_DEBUG > 0)
			DebugLog("Failed to shutdown linsocket. Error: %s.", LogSeverity::Error, strerror(errno));

		return false;
	}

	if (NET_DEBUG > 0)
		DebugLog("Linsocket shutdown.", LogSeverity::Info);

	return true;
}


bool LinSocket::Connect(const char* _ipAddres, const int _port)
{
	addrinfo hints = { 0 };
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	addrinfo* addrs = 0;

	std::stringstream ss;
	ss << _port;

	if (getaddrinfo(_ipAddres, ss.str().c_str(), &hints, &addrs) != 0)
	{
		if (NET_DEBUG > 0)
			DebugLog("Failed to get address info. Error: %s.", LogSeverity::Error, strerror(errno));

		freeaddrinfo(addrs);
		return false;
	}

	addrinfo* rp;
	for (rp = addrs; rp != 0; rp = rp->ai_next)
	{
		*m_socket = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

		if (*m_socket == -1)
			continue;

		if (connect(*m_socket, rp->ai_addr, rp->ai_addrlen) != -1)
			break;

		close(*m_socket);
	}

	freeaddrinfo(addrs);

	if (rp == 0)
	{
		if (NET_DEBUG > 0)
			DebugLog("Failed to connect to %s:%d. Error: %s.", LogSeverity::Error, _ipAddres, _port, strerror(errno));

		return false;
	}


	*m_remoteAddress = _ipAddres;
	*m_remotePort = _port;

	if (NET_DEBUG > 0)
		DebugLog("Connected to %s:%d.", LogSeverity::Info, _ipAddres, _port);

	return true;
}
bool LinSocket::Bind(const int _port)
{
	sockaddr_in address;
	address.sin_family = PF_INET;
	address.sin_port = htons(_port);
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(*m_socket, (sockaddr*)&address, sizeof(address)) == -1	)
	{
		if (NET_DEBUG > 0)
			DebugLog("Failed to bind socket. Error: %s.", LogSeverity::Error, strerror(errno));

		return false;
	}

	sockaddr_in sin;
	socklen_t len = sizeof(sin);

	if (getsockname(*m_socket, (sockaddr*)&sin, &len) == 0)
		*m_localPort = ntohs(sin.sin_port);
	else
	{
		if (NET_DEBUG > 0)
			DebugLog("Failed to get local port for socket. Error: %s.", LogSeverity::Error, strerror(errno));

		return false;
	}

	return true;
}
bool LinSocket::Listen(int _backlog)
{
	if (listen(*m_socket, _backlog) != 0)
	{
		if (NET_DEBUG > 0)
			DebugLog("Failed to put socket in listen mode. Error: %s.", LogSeverity::Error, strerror(errno));

		return false;
	}

	return true;
}

ISocket* LinSocket::Accept(void)
{
	sockaddr_in incomingAddress;
	socklen_t incomingAddressSize = sizeof(incomingAddress);
	int newSocket = -1;
	newSocket = accept(*m_socket, (sockaddr*)&incomingAddress, &incomingAddressSize);

	if (newSocket == -1)
	{
		int e = errno;
		if (NET_DEBUG > 0 && e != 11 && e != 35)
			DebugLog("Accept failed. Error: %s.", LogSeverity::Error, strerror(e));

		return 0;
	}

	LinSocket* sock = new LinSocket(newSocket);
	char ipAddress[INET6_ADDRSTRLEN];
	inet_ntop(incomingAddress.sin_family, &incomingAddress.sin_addr, ipAddress, sizeof(ipAddress));

	sockaddr_in sin;
	socklen_t len = sizeof(sin);

	if (getsockname(newSocket, (sockaddr *)&sin, &len) == 0)
		*sock->m_localPort = ntohs(sin.sin_port);
	else
	{
		if (NET_DEBUG > 0)
			DebugLog("Failed to get local port from new socket. Socket error: %s.", LogSeverity::Error, strerror(errno));

		return 0;
	}

	*sock->m_remoteAddress = ipAddress;
	*sock->m_remotePort = incomingAddress.sin_port;
	*sock->m_active = 1;

	if (NET_DEBUG > 0)
		DebugLog("New linsocket from %s:%d accepted.", LogSeverity::Info, ipAddress, incomingAddress.sin_port);

	return sock;
}

int LinSocket::Send(char* _buffer, int _length, int _flags)
{
#if !defined(__OSX__) && !defined(__IOS__)
	if (_flags == 0)
		_flags = MSG_NOSIGNAL;
#endif

	char* unencryptedData = 0;
	char* encryptedData = 0;
	short packetSize = 0;

	try
	{
		packetSize = _length; // Get the size of the un-encrypted data packet
		short blockSize = 16; // block size of the encrypted data (any packet larger then this will be divided into several blocks)
		short padding = packetSize % blockSize; // how much padding the encrypted packet will need

		CRijndael oRijndael;
		oRijndael.MakeKey("R2fö5Ø!89qå*3v5T", CRijndael::sm_chain0, blockSize, blockSize); // Create the key used for encrypt the data

		if (padding) // if padding wasn't a multiply of blockSize
			packetSize += blockSize - padding; // add the required padding to the packetSize

		unencryptedData		= new char[packetSize + 1];
		encryptedData		= new char[packetSize + 1];

		memcpy(unencryptedData, _buffer, packetSize + 1);
		memset(encryptedData, 0, packetSize + 1);

		oRijndael.Encrypt(unencryptedData, encryptedData, packetSize, CRijndael::ECB); // Encrypt the packet using the ECB mode. This is not ideal for encryption but since we are not able to randomize the key each time
																					   // (since the encryption and decryption will happend on different clients) and we can't base the encryption on previously data
																					   // since the server might receive packets from another client which a third client will not have, which in turn will mess up the
																					   // encryption steps.

	}
	catch (exception& roException)
	{
		SDL_Log("Exception", roException.what());
	}


	short len = htons(packetSize);
	short totalDataSent = 0;

	int bytesSent = send(*m_socket, (void*)&len, 2, _flags);
	if (bytesSent == 2)
	{
		bytesSent = send(*m_socket, (void*)encryptedData, packetSize, _flags);

		if (NET_DEBUG == 2)
		{
			if (bytesSent > 0)
				DebugLog("Sent packet of size %d.", LogSeverity::Info, bytesSent);
			else if (bytesSent == 0)
				DebugLog("Unable to send packet. Socket shutdown gracefully.", LogSeverity::Info);
			else if (bytesSent < 0)
				DebugLog("Failed to send packet of size %d. Error: %s.", LogSeverity::Error, bytesSent, strerror(errno));
		}

	}
	else if (bytesSent == 0 && NET_DEBUG == 2)
		DebugLog("Unable to send header packet. Socket shutdown gracefully.", LogSeverity::Info);
	else if (bytesSent < 0 && NET_DEBUG == 2)
		DebugLog("Failed to send header packet of size %d. Error code: %d", LogSeverity::Info, bytesSent, strerror(errno));


	SAFE_DELETE_ARRAY(unencryptedData);
	SAFE_DELETE_ARRAY(encryptedData);

	return bytesSent;
}

int LinSocket::Receive(char* _buffer, int _length, int _flags)
{
	short len;
	int headerLen = recv(*m_socket, (void*)&len, 2, MSG_WAITALL);

	if (headerLen == 2)
	{
		len = ntohs(len);
		int sizeReceived = recv(*m_socket, (void*)_buffer, (int)len, MSG_WAITALL);

		try
		{
			short blockSize = 16;

			CRijndael oRijndael;
			oRijndael.MakeKey("R2fö5Ø!89qå*3v5T", CRijndael::sm_chain0, blockSize, blockSize); // Create the key used to decrypt the recived package

			char* decryptedData = new char[sizeReceived + 1];
			memset(decryptedData, 0, sizeReceived + 1);

			oRijndael.Decrypt(_buffer, decryptedData, sizeReceived, CRijndael::ECB); // Encrypt the packet using the ECB mode. This is not ideal for encryption but since we are not able to randomize the key each time
																					 // (since the encryption and decryption will happend on different clients) and we can't base the encryption on previously data
																					 // since the server might receive packets from another client which a third client will not have, which in turn will mess up the
																					 // encryption steps.

			memcpy(_buffer, decryptedData, sizeReceived+1);
			SAFE_DELETE_ARRAY(decryptedData);

		}
		catch (exception& roException)
		{
			SDL_Log("Exception", roException.what());
		}

		if (sizeReceived < len)
		{
			if (NET_DEBUG == 2)
				DebugLog("Received packet of incorrect size. Got %d, expected %d.", LogSeverity::Warning, len, _length);

			return -1;
		}

		if (len > _length)
		{
			if (NET_DEBUG == 2)
				DebugLog("Received packet of incorrect size. Got %d, expected %d.", LogSeverity::Warning, (int)len, _length);

			return -1;
		}

		if (NET_DEBUG == 2)
			DebugLog("Received packet with size %d.", LogSeverity::Info, sizeReceived);

		return sizeReceived;

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
			DebugLog("Failed to receive header packet. Error: %s.", LogSeverity::Error, strerror(errno));
		return -1;
	}

	return -1;
}

bool LinSocket::SetNonBlocking(bool _value)
{
	int value = _value;
	int result = ioctl(*m_socket, FIONBIO, &value);
	int error = errno;
	if (result != 0)
	{
		if (NET_DEBUG > 0)
			DebugLog("Failed to %s non blocking on socket. Error: %s.", LogSeverity::Error, _value ? "enable" : "disable", strerror(errno));
		return false;
	}

	return true;
}

bool LinSocket::SetNoDelay(bool _value)
{
	int value = _value;
	if (setsockopt(*m_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&value, sizeof(int)) < 0)
	{
		if (NET_DEBUG > 0)
			DebugLog("Failed to %s no delay on socket. Error: %s.", LogSeverity::Error, _value ? "enable" : "disable", strerror(errno));
		return false;
	}

	return true;
}

bool LinSocket::SetTimeoutDelay(int _value, bool _recv, bool _send)
{
	int timeout = _value;
	bool success = true;

	if (_recv)
	{
		if (setsockopt(*m_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0)
		{
			if (NET_DEBUG > 0)
				DebugLog("Failed to set %d timeout delay on receive. Error: %s", LogSeverity::Error, _value, strerror(errno));
			success = false;
		}
	}

	if (_send)
	{
		if (setsockopt(*m_socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout)) < 0)
		{
			if (NET_DEBUG > 0)
				DebugLog("Failed to set %d timeout delay on send. Error: %s", LogSeverity::Error, _value, strerror(errno));
			success = false;
		}
	}

	return success;
}


#endif