#ifndef MASTERSERVERSYSTEM_H
#define MASTERSERVERSYSTEM_H

#include "ECSL/Interfaces/System.h"
#include "ECSL/Framework/World.h"
#include "../Network/ClientDatabase.h"

class MasterServerSystem : public ECSL::System
{
	

public:
	MasterServerSystem();
	~MasterServerSystem();

	void Update(float _dt);
	void Initialize();
	void PostInitialize();

	void OnEntityAdded(unsigned int _entityId);
	void OnEntityRemoved(unsigned int _entityId);

private:
	ClientDatabase* m_clientDatabase;
	std::map<int, bool> m_playerIds;
	std::vector<int> m_serverIds;

	int m_gameRunningId;
	int m_oldGameRunningId;
	float m_requestServerListTimer;
	
	void OnGetServerList(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
};

#endif