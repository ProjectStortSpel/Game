#ifndef MASTERSERVERSYSTEM_H
#define MASTERSERVERSYSTEM_H

#include "ECSL/Interfaces/System.h"
#include "ECSL/Framework/World.h"
#include "../Network/ClientDatabase.h"

class MasterServerSystem : public ECSL::System
{
	enum MASTER_SERVER_MESSAGES
	{
		ADD_TO_DATABASE,
		REMOVE_FROM_DATABASE,
		GAME_STARTED,
		IS_PASSWORD_PROTECTED,
		SET_SERVER_PORT,
		MAX_PLAYER_COUNT_INCREASED,
		MAX_PLAYER_COUNT_CHANGED,
		PLAYER_COUNT_INCREASED,
		PLAYER_COUNT_DECREASED,
		SPECTATOR_COUNT_INCREASED,
		SPECTATOR_COUNT_DECREASED,
		GET_SERVER_LIST,
		GAME_PING
	};

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

	std::vector<MASTER_SERVER_MESSAGES> m_mServerMessages;

	std::string m_name;

	int m_gameRunningId;
	int m_oldGameRunningId;
	
	bool m_pwProtected;
	bool m_serverStarted;
	bool m_connect;
	int m_port;
	int m_maxPlayers;

	float m_timeoutTimer;

	void OnConnectionAccepted(Network::NetConnection _nc, const char* _msg);
	void OnGetServerList(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
	void OnServerShutdown(Network::NetConnection _nc, const char* _msg);

	void EntitiesAddedServer(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);
	void EntitiesAddedClient(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);
	void EntitiesAddedNeither(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);

	void EntitiesRemovedServer(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);
	void EntitiesRemovedClient(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);
	void EntitiesRemovedNeither(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);

};

#endif