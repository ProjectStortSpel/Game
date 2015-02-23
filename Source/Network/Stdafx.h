#ifndef STDAFX_NETWORK_H
#define STDAFX_NETWORK_H

#include <SDL/SDL.h>
#include <string>
#include <inttypes.h>
#include <cstring>
#include <stdio.h>

#include "Logger/Managers/Logger.h"

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

#define NetworkHookPlaceholders std::placeholders::_1, std::placeholders::_2, std::placeholders::_3

#define MAX_PACKET_SIZE 65535 // Max value for unsigned short
#define SAFE_DELETE(x) if(x) { delete x; x = 0; }

#define TYP_INIT 0 
#define TYP_SMLE 1 
#define TYP_BIGE 2 

namespace Network
{
	//enum LogSeverity
	//{
	//	Info,
	//	Warning,
	//	Error
	//};

	unsigned long long hton_ll(unsigned long long src);
	unsigned long long ntoh_ll(unsigned long long src);
	static int NET_DEBUG = 2;

#pragma warning(push)
#pragma warning(disable: 4996)

	static void DebugLog(const char* _msg, LogSeverity _severity)
	{
		Logger::GetInstance().Log("Network", _severity, _msg);
	}
	
	static void DebugLog(const char* _msg, LogSeverity _severity, int _value)
	{
		char buff[100];
		sprintf(buff, _msg, _value);
		Logger::GetInstance().Log("Network", _severity, buff);
	}
	
	static void DebugLog(const char* _msg, LogSeverity _severity, const char* _value)
	{
		char buff[100];
		sprintf(buff, _msg, _value);
		Logger::GetInstance().Log("Network", _severity, buff);
	}
	
	static void DebugLog(const char* _msg, LogSeverity _severity, const char* _value, int _value2)
	{
		char buff[100];
		sprintf(buff, _msg, _value, _value2);
		Logger::GetInstance().Log("Network", _severity, buff);
	}

	static void DebugLog(const char* _msg, LogSeverity _severity, int _value, const char* _value2)
	{
		char buff[100];
		sprintf(buff, _msg, _value, _value2);
		Logger::GetInstance().Log("Network", _severity, buff);
	}
	
	static void DebugLog(const char* _msg, LogSeverity _severity, const char* _value, const char* _value2)
	{
		char buff[100];
		sprintf(buff, _msg, _value, _value2);
		Logger::GetInstance().Log("Network", _severity, buff);
	}
	
	static void DebugLog(const char* _msg, LogSeverity _severity, int _value, int _value2)
	{
		char buff[100];
		sprintf(buff, _msg, _value, _value2);
		Logger::GetInstance().Log("Network", _severity, buff);
	}
	
	static void DebugLog(const char* _msg, LogSeverity _severity, const char* _ip, int _port, int _error)
	{
		char buff[100];
		sprintf(buff, _msg, _ip, _port, _error);
		Logger::GetInstance().Log("Network", _severity, buff);
	}
	
	static void DebugLog(const char* _msg, LogSeverity _severity, const char* _ip, int _port, const char* _error)
	{
		char buff[100];
		sprintf(buff, _msg, _ip, _port, _error);
		Logger::GetInstance().Log("Network", _severity, buff);
	}

	
	
	
#pragma warning(pop)

}


#endif