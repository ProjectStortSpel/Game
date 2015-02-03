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

	void Update(const ECSL::RuntimeInfo& _runtime);
	void Initialize();
	void PostInitialize();

	void EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);
	void EntitiesRemoved(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);

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