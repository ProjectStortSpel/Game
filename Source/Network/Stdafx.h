#ifndef STDAFX_H
#define STDAFX_H

#define MAX_PLAYERS 8

typedef std::function<void(unsigned char)> NetEvent;

#ifdef _WIN32
	#define NetSleep(x) Sleep(x);
#else
	#define NetSleep(x) usleep(30 * 1000);
#endif

#endif