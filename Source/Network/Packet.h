#ifndef PACKET_H
#define PACKET_H

#include <string>

#include "Stdafx.h"
#include "NetConnection.h"

namespace Network
{
	class DECLSPEC Packet
	{
	public:
		Packet(void);
		Packet(char* _data, unsigned short _length, NetConnection _sender = NetConnection());

		~Packet();

	public:

		char* Data;
		unsigned short* Length;
		NetConnection* Sender;
	};
}

#endif