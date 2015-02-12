#ifndef WIN32

#ifndef LINSOCKET_H
#define LINSOCKET_H

#include "ISocket.h"

namespace Network
{
	class DECLSPEC LinSocket : public ISocket
	{
	public:

		LinSocket(int _socket);
		LinSocket(int _domain, int _type, int _protocol = 0);
		LinSocket(void);
		~LinSocket(void);

		bool Initialize(void);
		bool Shutdown(void);
		bool Connect(const char* _ipAddress, const int _port);
		bool Bind(const int _port);
		bool Listen(int _backlog);
		bool SetNonBlocking(bool _value);
		bool SetNoDelay(bool _value);
		bool SetTimeoutDelay(int _value);
		bool CloseSocket(void);
		bool ShutdownSocket(int _how);
		ISocket* Accept(void);
		int Send(char* _buffer, int _length, int _flags = 0);
		int Receive(char* _buffer, int _length, int _flags = 0);

	private:

		int* m_socket;
	};
}

#endif

#endif