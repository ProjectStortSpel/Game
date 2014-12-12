#ifndef RECONNECTSYSTEM_H
#define RECONNECTSYSTEM_H

#include "ECSL/Interfaces/System.h"
#include "ECSL/Framework/World.h"
#include "Network/ServerNetwork.h"

class ReconnectSystem : public ECSL::System
{
	ECSL::World* m_world;
	float m_timer;

public:
	ReconnectSystem();
	~ReconnectSystem();

	void Update(float _dt);
	void Initialize();

	void OnEntityAdded(unsigned int _entityId);
	void OnEntityRemoved(unsigned int _entityId);

private:
	void OnUserDisconnected(Network::NetConnection _nc, const char* _message);
	void OnUserKicked(Network::NetConnection _nc, const char* _message);
	void OnUserTimedOut(Network::NetConnection _nc, const char* _message);

private:

	std::map<char*, bool> m_connectedUsers;

};

#endif