#include "MasterServerSystem.h"
#include "ECSL/Managers/ComponentTypeManager.h"
#include "Game/NetworkInstance.h"

MasterServerSystem::MasterServerSystem()
	:m_clientDatabase(0), m_requestServerListTimer(0)
{
}
MasterServerSystem::~MasterServerSystem()
{
	SAFE_DELETE(m_clientDatabase);
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
	m_clientDatabase = new ClientDatabase();
	m_clientDatabase->Connect();

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
		if (m_requestServerListTimer > 10.0)
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

	if (EntityHasComponent(_entityId, ECSL::ComponentTypeManager::GetInstance().GetTableId("AvailableSpawnpoint")))
	{
		m_clientDatabase->IncreaseMaxNoPlayers();
	}
	else if (EntityHasComponent(_entityId, ECSL::ComponentTypeManager::GetInstance().GetTableId("Player")))
	{
		if (EntityHasComponent(_entityId, ECSL::ComponentTypeManager::GetInstance().GetTableId("IsSpectator")))
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
	else if (EntityHasComponent(_entityId, ECSL::ComponentTypeManager::GetInstance().GetTableId("GameRunning")))
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