#ifndef NETWORKINSTANCE_H
#define NETWORKINSTANCE_H

#include "Network/ClientNetwork.h"
#include "Network/ServerNetwork.h"
#include "Game/Network/NetworkHelper.h"
#include "ECSL/Framework/World.h"

class NetworkInstance
{
private:

	static Network::ServerNetwork* m_server;
	static Network::ClientNetwork* m_client;
	static NetworkHelper* m_clientNetworkHelper;
	static NetworkHelper* m_serverNetworkHelper;

public:
	static void InitServer();
	static void InitClient();
	static void InitClientNetworkHelper(ECSL::World** _world);
	static void InitServerNetworkHelper(ECSL::World** _world);

	static void DestroyServer();
	static void DestroyClient();
	static void DestroyClientNetworkHelper();
	static void DestroyServerNetworkHelper();

	static Network::ClientNetwork* GetClient();
	static Network::ServerNetwork* GetServer();
	static NetworkHelper* GetClientNetworkHelper();
	static NetworkHelper* GetServerNetworkHelper();

	static bool isServer();
	static bool isClient();

};


#endif