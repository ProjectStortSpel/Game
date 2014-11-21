#ifndef STDAFX_H
#define STDAFX_H

#include <functional>
#include <string>

#define NET_DEBUG 0

typedef std::string NetConnection;

typedef std::function<void(unsigned char, NetConnection*)> NetEvent;

#define SAFE_DELETE(x) if (x) { delete x; x = 0; }

#ifdef _WIN32
	#define NetSleep(x) Sleep(x);
#else
	#define NetSleep(x) usleep(30 * 1000);
#endif

#ifdef _WIN32
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