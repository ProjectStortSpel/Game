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
	static NetworkHelper* m_networkHelper;

public:
	static void InitServer();
	static void InitClient();
	static void InitNetworkHelper(ECSL::World** _world);

	static void DestroyServer();
	static void DestroyClient();
	static void DestroyNetworkHelper();

	static Network::ClientNetwork* GetClient();
	static Network::ServerNetwork* GetServer();
	static NetworkHelper* GetNetworkHelper();

	static bool isServer();
	static bool isClient();

};


#endif