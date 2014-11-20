#ifndef STDAFX_H
#define STDAFX_H

#include <functional>

#define MAX_PLAYERS 8


#define NET_DEBUG 1

typedef std::function<void(unsigned char)> NetEvent;

#ifdef _WIN32
	#define NetSleep(x) Sleep(x);
#else
	#define NetSleep(x) usleep(30 * 1000);
#endif

#endif