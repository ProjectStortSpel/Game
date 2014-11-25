#ifndef WIN32

#include "Network/LinSocket.h"

LinSocket::LinSocket(int _domain, int _type, int _protocol)
{
	m_socket = socket(_domain, _type, _protocol);
	m_remoteIP = "";
	m_remotePort = 0;
}

LinSocket::~LinSocket()
{

}

bool LinSocket::Initialize()
{
	return true;
}


bool LinSocket::Connect(const char* _ip, const int _port)
{
	sockaddr_in address;

	memset(&address, '0', sizeof(address)); 

	address.sin_family = AF_INET;
	address.sin_port = htons(_port);

	if(inet_pton(AF_INET, _ip, &address.sin_addr)<=0)
	{
		printf("\n inet_pton error occured\n");
		return false;
	} 

	if( connect(m_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		printf("\n Error : Connect Failed \n");
		return false;
	} 
	return true; 
}

bool LinSocket::Bind(const int _port)
{
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(_port);
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(m_socket, (struct sockaddr *) &address, sizeof(address)) < 0)
	{
		perror("ERROR on binding");
		return false;
	}
	return true;
}

ISocket* LinSocket::Accept(NetConnection& _netConnection)
{
	return 0;
}

bool LinSocket::Listen(int _backlog)
{
	return false;
}

int LinSocket::Recv(char* _buffer, int _length, int _flags)
{
	return -1;
}

int LinSocket::Send(char* _buffer, int _length, int _flags)
{
	return -1;
}

#endif