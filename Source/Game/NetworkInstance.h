#ifndef NETWORKINSTANCE_H
#define NETWORKINSTANCE_H

#include "Network/ClientNetwork.h"
#include "Network/ServerNetwork.h"

class NetworkInstance
{
private:

	static Network::ServerNetwork* m_server;
	static Network::ClientNetwork* m_client;

public:
	static void InitServer();
	static void InitClient();

	static void DestroyServer();
	static void DestroyClient();

	static Network::ClientNetwork* GetClient();
	static Network::ServerNetwork* GetServer();

	static bool isServer();
	static bool isClient();

};


#endif