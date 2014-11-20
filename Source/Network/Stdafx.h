#ifndef STDAFX_H
#define STDAFX_H

#include <functional>
#include <string>

#define NET_DEBUG 0

typedef std::string NetConnection;

typedef std::function<void(unsigned char, NetConnection*)> NetEvent;

#ifdef _WIN32
	#define NetSleep(x) Sleep(x);
#else
	#define NetSleep(x) usleep(30 * 1000);
#endif

#endif