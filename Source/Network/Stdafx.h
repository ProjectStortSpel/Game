#ifndef STDAFX_NETWORK_H
#define STDAFX_NETWORK_H

#include <SDL/SDL.h>
#include <string>
#include <inttypes.h>
#include <cstring>

// Includes
#ifdef WIN32
//#ifdef _DEBUG
//#include <VLD/vld.h>
//#endif
#else
#include <unistd.h>
#endif

// Macros/Defines
#ifdef WIN32
#define NetSleep(x) Sleep(x);
#else
#define NetSleep(x) usleep(30 * 1000);
#endif

#define MAX_PACKET_SIZE 65535 // Max value for unsigned short
#define SAFE_DELETE(x) if(x) { delete x; x = 0; }

#define TYP_INIT 0 
#define TYP_SMLE 1 
#define TYP_BIGE 2 

namespace Network
{
	enum LogSeverity
	{
		Info,
		Warning,
		Error
	};


	unsigned long long hton_ll(unsigned long long src);
	unsigned long long ntoh_ll(unsigned long long src);
	static int NET_DEBUG = 1;

#pragma warning(push)
#pragma warning(disable: 4996)

	template<typename T, typename... Args>
	static void DebugLog(const char* _msg, LogSeverity _severity, T _value, Args... _args)
	{
		char buff[100];
		sprintf(buff, _msg, _value);

		DebugLog(buff, _severity, _args...);
	}

	template<typename T>
	static void DebugLog(const char* _msg, LogSeverity _severity, T _value)
	{
		char buff[100];
		sprintf(buff, _msg, _value);
		
		SDL_Log(buff);
	}

	static void DebugLog(const char* _msg, LogSeverity _severity)
	{
		SDL_Log(_msg);
	}

#pragma warning(pop)

}


#endif