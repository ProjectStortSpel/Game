#include "MasterServerSystem.h"
#include "ECSL/Managers/ComponentTypeManager.h"
#include "Game/NetworkInstance.h"
#include "Game/Logger/Logger.h"

MasterServerSystem::MasterServerSystem()
	:m_clientDatabase(0), m_requestServerListTimer(0)
{
}
MasterServerSystem::~MasterServerSystem()
{
	//SAFE_DELETE(m_clientDatabase);
}

void MasterServerSystem::Initialize()
{
	SetSystemName("MasterServerSystem");

	/*	Rendersystem wants Network	*/
	AddComponentTypeToFilter("AvailableSpawnpoint", ECSL::FilterType::RequiresOneOf);
	AddComponentTypeToFilter("Player", ECSL::FilterType::RequiresOneOf);
	AddComponentTypeToFilter("GameRunning", ECSL::FilterType::RequiresOneOf);

	m_gameRunningId = -1;
	m_oldGameRunningId = -1;
}

void MasterServerSystem::PostInitialize()
{
	m_clientDatabase = &ClientDatabase::GetInstance();

	
	if (!m_clientDatabase->Connect())
		return;

	if (NetworkInstance::GetServer()->IsRunning())
	{
		std::string pw = NetworkInstance::GetServer()->GetServerPassword();
		int port = NetworkInstance::GetServer()->GetIncomingPort();
		m_clientDatabase->AddToDatabase();
		m_clientDatabase->SetServerPort(port);
		m_clientDatabase->SetPasswordProtected(pw.size() > 0);
	}
	else
	{
		Network::NetMessageHook customHook;
		customHook = std::bind(&MasterServerSystem::OnGetServerList, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		m_clientDatabase->HookOnGetServerList(customHook);
		Logger::GetInstance().Log("MasterServer", Info, "Hooking custom hook \"GET_SERVER_LIST\"to \"OnGetServerList\"");
		m_clientDatabase->RequestServerList();
	}
}

void MasterServerSystem::Update(float _dt)
{
	// Return if the user is a already connected client
	if (NetworkInstance::GetClient()->IsConnected())
		return;


	m_clientDatabase->Update(_dt);

	// Update to the masterserver is the user is a server
	if (NetworkInstance::GetServer()->IsRunning())
	{
		if (m_oldGameRunningId != m_gameRunningId)
		{
			m_oldGameRunningId = m_gameRunningId;
			m_clientDatabase->SetGameStarted(m_gameRunningId > 0);
		}


	}

	// If the user is neither, we should request a new server list every 10 seconds
	else
	{
		m_requestServerListTimer += _dt;
		if (m_requestServerListTimer > 2.0)
		{
			m_clientDatabase->RequestServerList();
			m_requestServerListTimer = 0.f;
		}
	}

}

void MasterServerSystem::OnEntityAdded(unsigned int _entityId)
{
	if (!NetworkInstance::GetServer()->IsRunning())
		return;

	if (HasComponent(_entityId, ECSL::ComponentTypeManager::GetInstance().GetTableId("AvailableSpawnpoint")))
	{
		m_clientDatabase->IncreaseMaxNoPlayers();
	}
	else if (HasComponent(_entityId, ECSL::ComponentTypeManager::GetInstance().GetTableId("Player")))
	{
		if (HasComponent(_entityId, ECSL::ComponentTypeManager::GetInstance().GetTableId("IsSpectator")))
		{
			m_clientDatabase->IncreaseNoSpectators();
			m_playerIds[_entityId] = true;
		}
		else
		{
			m_clientDatabase->IncreaseNoPlayers(); // <--
			m_playerIds[_entityId] = false;
		}
		
	}
	else if (HasComponent(_entityId, ECSL::ComponentTypeManager::GetInstance().GetTableId("GameRunning")))
	{
		m_gameRunningId = _entityId;
	}
}

void MasterServerSystem::OnEntityRemoved(unsigned int _entityId)
{
	if (!NetworkInstance::GetServer()->IsRunning())
		return;

	// Go through all playerIds and search for an match
	for (auto it = m_playerIds.begin(); it != m_playerIds.end(); ++it)
	{

		if (it->first == _entityId)
		{
			if (it->second) // Spectator
			{
				m_clientDatabase->DecreaseNoSpectators();
				m_playerIds.erase(it);
				return;
			}
			else // Player
			{
				m_clientDatabase->DecreaseNoPlayers();
				m_playerIds.erase(it);
				return;
			}
		}
	}

	if (m_gameRunningId == _entityId)
		m_gameRunningId = -1;

}

void MasterServerSystem::OnGetServerList(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
{
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


		m_serverIds.push_back(id);
	}
}