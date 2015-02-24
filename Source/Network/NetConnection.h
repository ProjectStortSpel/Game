#ifndef NETCONNECTION_H
#define NETCONNECTION_H

#include "Stdafx.h"

namespace Network
{

	class DECLSPEC NetConnection
	{

	private:
		std::string* IpAddress;
		unsigned short* Port;

	public:
		NetConnection();
		NetConnection(const NetConnection& _connection);
		NetConnection(const char* _address, unsigned short _port);
		~NetConnection();

		const char* GetIpAddress() { return IpAddress->c_str(); }
		unsigned short GetPort() { return *Port; }

		bool operator==(const NetConnection& other) const
		{
			if (*this->Port == *other.Port && IpAddress->compare(*other.IpAddress) == 0)
				return true;

			return false;
		}

		bool operator< (const NetConnection& other) const
		{
			if (*Port < *other.Port)
				return true;
			else if (*Port == *other.Port)
				return IpAddress->compare(*other.IpAddress) < 0;

			return false;
		}

		void operator= (const NetConnection& other)
		{
			*IpAddress = *other.IpAddress;
			*Port = *other.Port;
		}


	};
}

#endif