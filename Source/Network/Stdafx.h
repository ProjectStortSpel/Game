#ifndef STDAFX_H
#define STDAFX_H

#include <functional>
#include <string>

#ifdef WIN32
#else
	#include <unistd.h>
#endif

#define MAX_PACKET_SIZE 65535	//max value for unsigned short 	
#define NET_DEBUG 1
#define SAFE_DELETE(x) if (x) { delete x; x = 0; }

struct NetConnection
{
	NetConnection()
	{
		IpAddress = "";
		Port = 0;
	}

	NetConnection(std::string _address, unsigned short _port)
	{
		IpAddress = _address;
		Port = _port;
	}

	std::string IpAddress;
	unsigned short Port;

/*
	bool operator<(const NetConnection& a)
	{
		if (this->Port < a.Port)
			return true;
	*///}
};

inline bool operator< (const NetConnection& lhs, const NetConnection& rhs)
{ 
	if (lhs.Port < rhs.Port)
		return true;
	
	else if (lhs.Port == rhs.Port)
	{
		if (lhs.IpAddress.size() < rhs.IpAddress.size())
			return true;

		else if (lhs.IpAddress.size() == rhs.IpAddress.size())
		{
			for (int i = 0; i < lhs.IpAddress.size(); ++i)
			{
				if (lhs.IpAddress[i] < rhs.IpAddress[i])
				{
					return true;
				}
			}
		}
	}

	return false;
}

typedef std::function<void(NetConnection)> NetEvent;


#ifdef WIN32
	#define NetSleep(x) Sleep(x);
#else
	#define NetSleep(x) usleep(30 * 1000);
#endif



#ifdef WIN32
	#define _CRTDBG_MAP_ALLOC

	#ifdef _DEBUG
		#ifndef DBG_NEW
			#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
			#define new DBG_NEW
		#endif
	#endif  // _DEBUG


	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#endif