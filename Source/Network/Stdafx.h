#ifndef STDAFX_H
#define STDAFX_H

#include <functional>
#include <string>

#define MAX_PLAYERS 8


#define NET_DEBUG 1

typedef std::string NetConnection;

typedef std::function<void(unsigned char, NetConnection*)> NetEvent;

#ifdef _WIN32
	#define NetSleep(x) Sleep(x);
#else
	#define NetSleep(x) usleep(30 * 1000);
#endif

#endif