// Networkv2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"
#include "Client.h"

int _tmain(int argc, _TCHAR* argv[])
{
	
	Server s;
	s.SetNetPort(1111);
	s.SetPassword("pass");
	s.Connect();


	Client c;
	c.SetNetPort(1111);
	c.SetIp("localhost");
	c.SetClientPort(2222);
	c.SetPassword("pass");
	c.Connect();


	while (1)
	{
		s.GetPacket();
		c.GetPacket();
	}

	system("pause");

	return 0;
}

