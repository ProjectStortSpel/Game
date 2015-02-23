#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "Network/PacketHandler.h"

#include <vector>

namespace ClientManager
{
	void Initialize();

	void NewClient(Network::NetConnection _client);
	void ClientDisconnected(Network::NetConnection _client);
	void SetAllClientsToConnecting();

	std::vector<Network::NetConnection> GetConnectedClients();


	//add callback to tell gamecreator to load the gamemode
}

#endif