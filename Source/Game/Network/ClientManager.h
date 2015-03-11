#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "Network/PacketHandler.h"

#include <vector>
#include <string>

namespace ClientManager
{
	void Initialize();

	void NewClient(Network::NetConnection _client);
	void ClientDisconnected(Network::NetConnection _client);

	void Update();

	void SetAllClientsToConnecting();
	std::vector<Network::NetConnection> GetConnectedClients();
	std::string GetPlayerName(Network::NetConnection& _nc);
	std::string SetPlayerName(Network::NetConnection& _nc, const char* _name);

	//add callback to tell gamecreator to load the gamemode
}

#endif