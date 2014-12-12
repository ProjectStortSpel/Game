#ifndef RECONNECTSYSTEM_H
#define RECONNECTSYSTEM_H

#include "ECSL/Interfaces/System.h"
#include "ECSL/Framework/World.h"

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

	std::map<char*, bool> m_connectedUsers;

};

#endif