#ifndef PACKET_H
#define PACKET_H

#include "Stdafx.h"
#include <string>

namespace Network
{
	class DECLSPEC NetConnection
	{
	private:
		std::string* IpAddress;
		unsigned short* Port;
	public:
		NetConnection();
		NetConnection(const NetConnection& _netConnection);
		NetConnection(const char* _address, unsigned short _port);
		~NetConnection();


		const char* GetIpAddress() { return IpAddress->c_str(); }
		unsigned short GetPort() { return *Port; }


		bool operator==(const NetConnection &other) const
		{
			if (*this->Port == *other.Port
				&& IpAddress->compare(*other.IpAddress) == 0)
				return true;
			return false;
		}

		bool operator< (const NetConnection& other) const
		{
			if (*Port < *other.Port)
				return true;

			else if (*Port == *other.Port)
			{
				return IpAddress->compare(*other.IpAddress) < 0;
			}

			return false;
		}

		void operator= (const NetConnection& other)
		{
			*IpAddress = *other.IpAddress;
			*Port = *other.Port;
		}
	};


	class DECLSPEC Packet
	{
	public:
		Packet(void);
		Packet(unsigned char* _data, unsigned short _length, NetConnection _sender = NetConnection());

		~Packet();

	public:

		unsigned char* Data;
		unsigned short* Length;
		NetConnection* Sender;


	};
}

#endif