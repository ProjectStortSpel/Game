#include "ISocket.h"

#ifdef WIN32
#include "WinSocket.h"
#else
#include "LinSocket.h"
#endif

using namespace Network;

bool	ISocket::g_initialized = false;
int		ISocket::g_noActiveSockets = 0;

ISocket* ISocket::CreateSocket(int _domain, int _type, int _protocol)
{
#ifdef WIN32
	return new WinSocket(_domain, _type, _protocol);
#else
	return new LinSocket(_domain, _type, _protocol);
#endif

}


ISocket* ISocket::CreateSocket()
{

#ifdef WIN32
	return new WinSocket();
#else
	return new LinSocket();
#endif

}
