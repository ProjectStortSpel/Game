#ifndef PACKET_H
#define PACKET_H

#include "Stdafx.h"

class DECLSPEC NetConnection
{
public:
	NetConnection();
	NetConnection(std::string _address, unsigned short _port);
	~NetConnection() {};

	std::string IpAddress;
	unsigned short Port;

	bool operator==(const NetConnection &other) const
	{
		if (this->Port == other.Port
			&& this->IpAddress.compare(other.IpAddress) == 0)
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
		if (lhs.IpAddress.size() < rhs.IpAddress.size())
			return true;

		else if (lhs.IpAddress.size() == rhs.IpAddress.size())
		{
			for (int i = 0; i < lhs.IpAddress.size(); ++i)
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
	unsigned char* Data;
	unsigned short Length;
	NetConnection Sender;
};

#endif