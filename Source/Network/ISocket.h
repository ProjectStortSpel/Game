#ifndef ISOCKET_H
#define ISOCKET_H

#include "Stdafx.h"
#include "NetConnection.h"

namespace Network
{
	class DECLSPEC ISocket
	{
	public:
		// Create a new default socket
		static ISocket* CreateSocket();
		// Create a new specified socket
		static ISocket* CreateSocket(int _domain, int _type, int _protocol = 0);

		virtual ~ISocket() {};

		virtual bool Shutdown(void) = 0;
		virtual bool Connect(const char* _ipAddress, const int _port) = 0;
		virtual bool Bind(const int _port) = 0;
		virtual bool Listen(int _backlog) = 0;
		virtual bool CloseSocket(void) = 0;
		virtual bool ShutdownSocket(int _how) = 0;
		virtual ISocket* Accept(void) = 0;
		virtual int Send(char* _buffer, int _length, int _flags = 0) = 0;
		virtual int Receive(char* _buffer, int _length, int _flags = 0) = 0;

		virtual bool SetNonBlocking(bool _value) = 0;
		virtual	bool SetNoDelay(bool _value) = 0;
		virtual bool SetTimeoutDelay(int _value, bool _recv, bool _send) = 0;

		// Set whether or not the socket is active
		// * 0 - No active connection on the socket
		// * 1 - An active connection but not yet fully initialized
		// * 2 - An fully initialized connection
		void SetActive(int _value) { *m_active = _value; }

		// Get a netconnection based on ip address and port
		NetConnection GetNetConnection() { return NetConnection(m_remoteAddress->c_str(), *m_remotePort); }
		// Get the remote ip address
		std::string GetRemoteIpAddress() { return *m_remoteAddress; }
		// Get the remort port
		int GetRemotePort() { return *m_remotePort; }
		// Get the local port
		int GetLocalPort() { return *m_localPort; }
		// Get whether or not the socket is active
		// * 0 - No active connection on the socket
		// * 1 - An active connection but not yet fully initialized
		// * 2 - An fully initialized connection
		int GetActive() { return *m_active; }

	protected:
		virtual bool Initialize(void) = 0;


	protected:

		static bool g_initialized;
		static int	g_noActiveSockets;

		std::string* m_remoteAddress;
		int* m_remotePort;
		int* m_localPort;
		int* m_active;

	private:

	};

}
#endif