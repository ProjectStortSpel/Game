#ifndef PACKET_H
#define PACKET_H

#include "Stdafx.h"

class DECLSPEC NetConnection
{
public:
	NetConnection();
	NetConnection(const char* _address, unsigned short _port);
	~NetConnection() {};

#pragma warning( disable : 4251 )

	const char* IpAddress;
	unsigned short Port;

#pragma warning( default : 4251 )

	bool operator==(const NetConnection &other) const
	{
		if (this->Port == other.Port
			&& strcmp(IpAddress, other.IpAddress) == 0)
			return true;
		return false;
	}
};

inline bool operator< (const NetConnection& lhs, const NetConnection& rhs)
{
	if (lhs.Port < rhs.Port)
		return true;

	else if (lhs.Port == rhs.Port)
	{
		if (strlen(lhs.IpAddress) < strlen(rhs.IpAddress))
		//if (lhs.IpAddress.size() < rhs.IpAddress.size())
			return true;

		else if (strlen(lhs.IpAddress) == strlen(rhs.IpAddress))
		//else if (lhs.IpAddress.size() == rhs.IpAddress.size())
		{
			for (unsigned int i = 0; i < strlen(lhs.IpAddress); ++i)
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

class DECLSPEC Packet
{
public:
	Packet(void);
	Packet(unsigned char* _data, unsigned short _length, NetConnection _sender = NetConnection());

	~Packet();

public:

#pragma warning( disable : 4251 )

	unsigned char* Data;
	unsigned short Length;
	NetConnection Sender;

#pragma warning( default : 4251 )

};

#endif