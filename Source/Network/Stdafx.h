#ifndef STDAFX_H
#define STDAFX_H

#include <SDL/SDL.h>
#include <string>

#ifdef WIN32
#define NetSleep(x) Sleep(x);
#else
#define NetSleep(x) usleep(30 * 1000);
#endif

#define MAX_PACKET_SIZE 65535 // Max value for unsigned short
#define NET_DEBUG 1
#define SAFE_DELETE(x) if(x) { delete x; x = 0; }

struct NetConnection
{
	std::string IpAddress;
	unsigned short Port;

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
};

struct Packet
{
	unsigned char* Data;
	unsigned short Length;
	NetConnection Sender;

	Packet()
	{
		Data = 0;
		Length = 0;
		Sender = NetConnection();
	}

	Packet(unsigned char* _data, unsigned short _length, NetConnection _sender = NetConnection())
	{
		Data = _data;
		Length = _length;
		Sender = _sender;
	}

};

#endif