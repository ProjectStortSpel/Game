#ifndef ISOCKET_H
#define ISOCKET_H

#include "Stdafx.h"

namespace Network
{
	class DECLSPEC ISocket
	{
	public:
		static ISocket* CreateSocket();
		static ISocket* CreateSocket(int _domain, int _type, int _protocol = 0);

		virtual ~ISocket() {};

		virtual bool Initialize(void) = 0;
		virtual bool Shutdown(void) = 0;
		virtual bool Connect(const char* _ipAddress, const int _port) = 0;
		virtual bool Bind(const int _port) = 0;
		virtual bool Listen(int _backlog) = 0;
		virtual bool SetNonBlocking(bool _value) = 0;
		virtual bool SetNoDelay(bool _value) = 0;
		virtual bool SetTimeoutDelay(int _value) = 0;
		virtual bool CloseSocket(void) = 0;
		virtual bool ShutdownSocket(int _how = 1) = 0;
		virtual ISocket* Accept(void) = 0;
		virtual int Send(char* _buffer, int _length, int _flags = 0) = 0;
		virtual int Receive(char* _buffer, int _length, int _flags = 0) = 0;

		NetConnection GetNetConnection() { return NetConnection(m_remoteAddress->c_str(), *m_remotePort); }
		std::string GetRemoteIpAddress() { return *m_remoteAddress; }
		int GetRemotePort() { return *m_remotePort; }
		int GetLocalPort() { return *m_localPort; }

		int GetActive(void) { return *m_active; }
		void SetActive(int _value) { *m_active = _value; }

	protected:


		static bool g_initialized;
		static int  g_noActiveSockets;

		std::string* m_remoteAddress;
		int* m_remotePort;
		int* m_localPort;
		int* m_active;


	private:
	};
}

#endif