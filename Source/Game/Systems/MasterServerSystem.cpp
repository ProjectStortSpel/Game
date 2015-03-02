#include "MasterServerSystem.h"
#include "ECSL/Managers/ComponentTypeManager.h"
#include "Game/Network/NetworkInstance.h"
#include "Logger/Managers/Logger.h"
#include "Input/InputWrapper.h"

MasterServerSystem::MasterServerSystem()
	:m_clientDatabase(0), m_name("DefaultName")
{
}
MasterServerSystem::~MasterServerSystem()
{
	//SAFE_DELETE(m_clientDatabase);
	//m_clientDatabase->~ClientDatabase();

	m_clientDatabase->ResetNetworkEvents();
	m_mServerMessages.clear();
	
}

void MasterServerSystem::Initialize()
{
	SetSystemName("MasterServerSystem");
	
	SetUpdateTaskCount(1);
	SetEntitiesAddedTaskCount(1);
	SetEntitiesRemovedTaskCount(1);

	/*	Rendersystem wants Network	*/
	AddComponentTypeToFilter("MapSpecs", ECSL::FilterType::RequiresOneOf);
	AddComponentTypeToFilter("Player", ECSL::FilterType::RequiresOneOf);
	AddComponentTypeToFilter("GameRunning", ECSL::FilterType::RequiresOneOf);
	AddComponentTypeToFilter("RefreshServerList", ECSL::FilterType::RequiresOneOf);
	AddComponentTypeToFilter("HostSettings", ECSL::FilterType::RequiresOneOf);

	m_gameRunningId = -1;
	m_oldGameRunningId = -1;
}

void MasterServerSystem::PostInitialize()
{
	m_clientDatabase = &ClientDatabase::GetInstance();
	m_connect = true;
	m_timeoutTimer = 0.f;

	Network::NetEvent hook = std::bind(&MasterServerSystem::OnConnectionAccepted, this, std::placeholders::_1, std::placeholders::_2);
	m_clientDatabase->HookOnConnectionAccepted(hook);

	if (NetworkInstance::GetServer()->IsRunning())
	{
		hook = std::bind(&MasterServerSystem::OnServerShutdown, this, std::placeholders::_1, std::placeholders::_2);
		NetworkInstance::GetServer()->SetOnServerShutdown(hook);

		Network::NetMessageHook customHook;
		customHook = std::bind(&MasterServerSystem::OnGrantDisconnect, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		m_clientDatabase->HookOnGrantDisconnect(customHook);
	}
	else if(NetworkInstance::GetClient()->IsConnected())
	{
		m_clientDatabase->Disconnect();
	}
	else
	{
		Network::NetMessageHook customHook;
		customHook = std::bind(&MasterServerSystem::OnGetServerList, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		m_clientDatabase->HookOnGetServerList(customHook);

		customHook = std::bind(&MasterServerSystem::OnGrantDisconnect, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		m_clientDatabase->HookOnGrantDisconnect(customHook);

		Logger::GetInstance().Log("MasterServer", Info, "Hooking custom hook \"GET_SERVER_LIST\"to \"OnGetServerList\"");
	}
}

void MasterServerSystem::Update(const ECSL::RuntimeInfo& _runtime)
{
	// Return if the user is a already connected client
	if (NetworkInstance::GetClient()->IsConnected() && !NetworkInstance::GetServer()->IsRunning())
		return;

	if (m_mServerMessages.size() > 0 && m_connect)
	{
		m_connect = false;
		m_clientDatabase->Connect();
	}
	
	//if (m_timer > 2.f)
	//{
	//	m_timer = 0.f;
	//}

	
	m_clientDatabase->Update(_runtime.Dt);

	// Update to the masterserver if the user is a server
	if (NetworkInstance::GetServer()->IsRunning())
	{
		if (m_oldGameRunningId != m_gameRunningId)
		{
			m_oldGameRunningId = m_gameRunningId;
			m_serverStarted = m_serverStarted ? false : true;
			//m_clientDatabase->SetGameStarted(m_gameRunningId > 0);
			m_mServerMessages.push_back(GAME_STARTED);
		}

	}

	if (NetworkInstance::GetServer()->IsRunning())
	{
		m_timeoutTimer += _runtime.Dt;
		if (m_timeoutTimer > 10.f)
		{
			m_timeoutTimer = 0.f;
			m_mServerMessages.push_back(GAME_PING);
		}
	}
}

void MasterServerSystem::EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	if (!NetworkInstance::GetServer()->IsRunning() && !NetworkInstance::GetClient()->IsConnected())
		EntitiesAddedNeither(_runtime, _entities);
	else if (NetworkInstance::GetServer()->IsRunning())
		EntitiesAddedServer(_runtime, _entities);
	else if (NetworkInstance::GetClient()->IsConnected())
		EntitiesAddedClient(_runtime, _entities);
}

void MasterServerSystem::EntitiesRemoved(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	if (!NetworkInstance::GetServer()->IsRunning() && !NetworkInstance::GetClient()->IsConnected())
		EntitiesRemovedNeither(_runtime, _entities);
	else if (NetworkInstance::GetServer()->IsRunning())
		EntitiesRemovedServer(_runtime, _entities);
	else if (NetworkInstance::GetClient()->IsConnected())
		EntitiesRemovedClient(_runtime, _entities);
}

void MasterServerSystem::OnConnectionAccepted(Network::NetConnection _nc, const char* _msg)
{

	for (int i = 0; i < m_mServerMessages.size(); ++i)
	{
		switch (m_mServerMessages[i])
		{
		case ADD_TO_DATABASE:
			m_clientDatabase->AddToDatabase(m_name.c_str(), m_port, m_pwProtected);
			m_timeoutTimer = 0.f;
			break;
		case REMOVE_FROM_DATABASE:
			m_clientDatabase->RemoveFromDatabase();
			m_timeoutTimer = 0.f;
			break;
		case GAME_STARTED:
			m_clientDatabase->SetGameStarted(m_serverStarted);
			m_timeoutTimer = 0.f;
			break;
		case IS_PASSWORD_PROTECTED:
			m_clientDatabase->SetPasswordProtected(m_pwProtected);
			m_timeoutTimer = 0.f;
			break;
		case SET_SERVER_PORT:
			m_clientDatabase->SetServerPort(m_port);
			m_timeoutTimer = 0.f;
			break;
		case MAX_PLAYER_COUNT_INCREASED:
			m_clientDatabase->IncreaseMaxNoPlayers();
			m_timeoutTimer = 0.f;
			break;
		case MAX_PLAYER_COUNT_CHANGED:
			m_clientDatabase->SetMaxNoPlayers(m_maxPlayers);
			m_timeoutTimer = 0.f;
			break;
		case PLAYER_COUNT_INCREASED:
			m_clientDatabase->IncreaseNoPlayers();
			m_timeoutTimer = 0.f;
			break;
		case PLAYER_COUNT_DECREASED:
			m_clientDatabase->DecreaseNoPlayers();
			m_timeoutTimer = 0.f;
			break;
		case SPECTATOR_COUNT_INCREASED:
			m_clientDatabase->IncreaseNoSpectators();
			m_timeoutTimer = 0.f;
			break;
		case SPECTATOR_COUNT_DECREASED:
			m_clientDatabase->DecreaseNoSpectators();
			m_timeoutTimer = 0.f;
			break;
		case GET_SERVER_LIST:
			m_clientDatabase->RequestServerList();
			break;
		case GAME_PING:
			m_clientDatabase->PingServer();
			break;
		default:
			break;
		}
	}

	if (m_mServerMessages.size() > 0)
		m_clientDatabase->RequestDisconnect();

	m_mServerMessages.clear();
	//m_clientDatabase->Disconnect();
}

void MasterServerSystem::OnServerShutdown(Network::NetConnection _nc, const char* _msg)
{
	m_mServerMessages.push_back(REMOVE_FROM_DATABASE);
}

void MasterServerSystem::OnGetServerList(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
{
	if (NetworkInstance::GetClient()->IsConnected() || NetworkInstance::GetServer()->IsRunning())
		return;

	for (int i = 0; i < m_serverIds.size(); ++i)
		KillEntity(m_serverIds[i]);

	m_serverIds.clear();


	int noServers = _ph->ReadInt(_id);
	ServerInfo si;

	for (int i = 0; i < noServers; ++i)
	{
		si.Name = _ph->ReadString(_id);
		si.IpAddress = _ph->ReadString(_id);
		si.Port = _ph->ReadInt(_id);
		si.NoUsers = _ph->ReadShort(_id);
		si.MaxUsers = _ph->ReadShort(_id);
		si.NoSpectators = _ph->ReadShort(_id);
		si.GameStarted = _ph->ReadByte(_id);
		si.PasswordProtected = _ph->ReadByte(_id);

		unsigned int id = CreateNewEntity();
		CreateComponentAndAddTo("ServerListEntry", id);

		char* data;
		int* integer;
		bool* boolean;

		// Name
		data = (char*)GetComponent(id, "ServerListEntry", "Name");
		memcpy(data, si.Name.c_str(), si.Name.length() + 1);

		// IpAddress
		data = (char*)GetComponent(id, "ServerListEntry", "IpAddress");
		memcpy(data, si.IpAddress.c_str(), si.IpAddress.length() + 1);

		// Port
		integer = (int*)GetComponent(id, "ServerListEntry", "Port");
		*integer = si.Port;

		// NoUsers
		integer = (int*)GetComponent(id, "ServerListEntry", "NoUsers");
		*integer = si.NoUsers;

		// MaxUsers
		integer = (int*)GetComponent(id, "ServerListEntry", "MaxUsers");
		*integer = si.MaxUsers;

		// NoSpectators
		integer = (int*)GetComponent(id, "ServerListEntry", "NoSpectators");
		*integer = si.NoSpectators;

		// GameStarted
		boolean = (bool*)GetComponent(id, "ServerListEntry", "GameStarted");
		*boolean = si.GameStarted;

		// PasswordProtected
		boolean = (bool*)GetComponent(id, "ServerListEntry", "PasswordProtected");
		*boolean = si.PasswordProtected;


		this->m_serverIds.push_back(id);
	}
}

void MasterServerSystem::OnGrantDisconnect(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
{
	m_clientDatabase->Disconnect();
	m_connect = true;
}

void MasterServerSystem::EntitiesAddedServer(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	for (unsigned int entityId : _entities)
	{
		if (HasComponent(entityId, ECSL::ComponentTypeManager::GetInstance().GetTableId("MapSpecs")))
		{
			m_maxPlayers = *((int*)GetComponent(entityId, "MapSpecs", "NoOfSpawnpoints"));
			m_mServerMessages.push_back(MAX_PLAYER_COUNT_CHANGED);
			//m_clientDatabase->IncreaseMaxNoPlayers();
		}
		else if (HasComponent(entityId, ECSL::ComponentTypeManager::GetInstance().GetTableId("Player")))
		{
			if (HasComponent(entityId, ECSL::ComponentTypeManager::GetInstance().GetTableId("IsSpectator")))
			{
				m_mServerMessages.push_back(SPECTATOR_COUNT_INCREASED);
				//m_clientDatabase->IncreaseNoSpectators();
				m_playerIds[entityId] = true;
			}
			else
			{
				//m_clientDatabase->IncreaseNoPlayers(); // <--
				m_mServerMessages.push_back(PLAYER_COUNT_INCREASED);
				m_playerIds[entityId] = false;
			}

		}
		else if (HasComponent(entityId, ECSL::ComponentTypeManager::GetInstance().GetTableId("GameRunning")))
		{
			m_gameRunningId = entityId;
		}
		else if (HasComponent(entityId, ECSL::ComponentTypeManager::GetInstance().GetTableId("HostSettings")))
		{
			m_pwProtected = strlen(NetworkInstance::GetServer()->GetServerPassword()) > 0;
			m_port = NetworkInstance::GetServer()->GetIncomingPort();
			m_name = (char*)GetComponent(entityId, "HostSettings", "Name");

			m_mServerMessages.push_back(ADD_TO_DATABASE);
		}

	}
}
void MasterServerSystem::EntitiesAddedClient(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	for (unsigned int entityId : _entities)
	{
	}
}
void MasterServerSystem::EntitiesAddedNeither(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	for (unsigned int entityId : _entities)
	{
		if (HasComponent(entityId, ECSL::ComponentTypeManager::GetInstance().GetTableId("RefreshServerList")))
		{
			m_mServerMessages.push_back(GET_SERVER_LIST);
			
			KillEntity(entityId);
			m_clientDatabase->SetTryConnect(true);

		}
	}
}


void MasterServerSystem::EntitiesRemovedServer(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	for (unsigned int entityId : _entities)
	{
		// Go through all playerIds and search for an match
		for (auto it = m_playerIds.begin(); it != m_playerIds.end(); ++it)
		{

			if (it->first == entityId)
			{
				if (it->second) // Spectator
				{
					//m_clientDatabase->DecreaseNoSpectators();
					m_mServerMessages.push_back(SPECTATOR_COUNT_DECREASED);
					m_playerIds.erase(it);
					return;
				}
				else // Player
				{
					//m_clientDatabase->DecreaseNoPlayers();
					m_mServerMessages.push_back(PLAYER_COUNT_DECREASED);
					m_playerIds.erase(it);
					return;
				}
			}
		}

		if (m_gameRunningId == entityId)
			m_gameRunningId = -1;
	}
}
void MasterServerSystem::EntitiesRemovedClient(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	for (unsigned int entityId : _entities)
	{
	}
}
void MasterServerSystem::EntitiesRemovedNeither(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	for (unsigned int entityId : _entities)
	{
	}
}