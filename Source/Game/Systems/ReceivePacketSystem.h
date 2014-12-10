#ifndef RECEIVEPACKETSYSTEM_H
#define RECEIVEPACKETSYSTEM_H

#include "ECSL/Interfaces/System.h"
#include "Network/ClientNetwork.h"
#include "Network/ServerNetwork.h"


class ReceivePacketSystem : public ECSL::System
{
public:
	ReceivePacketSystem(Network::ClientNetwork* _client, Network::ServerNetwork* _server);
	~ReceivePacketSystem();

	void Update(float _dt);
	void Initialize();

	void OnEntityAdded(unsigned int _entityId);
	void OnEntityRemoved(unsigned int _entityId);

private:
	Network::ClientNetwork* m_client;
	Network::ServerNetwork* m_server;

};

#endif