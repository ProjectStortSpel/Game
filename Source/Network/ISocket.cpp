#include "ISocket.h"

#ifdef _WIN32
#include "Network/WinSocket.h"
#else
#include "Network/LinSocket.h"
#endif

static bool Initialize()
{
#ifdef _WIN32
	return WinSocket::Initialize();
#else
	return LinSocket::Initialize();
#endif
}


static ISocket* CreateISocket(int _domain, int _type, int _protocol)
{
#ifdef _WIN32
	return new WinSocket(_domain, _type, _protocol);
#else
	return new LinSocket(_domain, _type, _protocol);
#endif
}