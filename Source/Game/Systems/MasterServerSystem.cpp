#include "MasterServerSystem.h"
#include "ECSL/Managers/ComponentTypeManager.h"
#include "Game/NetworkInstance.h"

MasterServerSystem::MasterServerSystem()
	:m_clientDatabase(0)
{
}
MasterServerSystem::~MasterServerSystem()
{
	SAFE_DELETE(m_clientDatabase);
}

void MasterServerSystem::Initialize()
{
	SetSystemName("Master Server System");

	/*	Rendersystem wants Network	*/
	AddComponentTypeToFilter("AvailableSpawnpoint", ECSL::FilterType::RequiresOneOf);
	AddComponentTypeToFilter("Player", ECSL::FilterType::RequiresOneOf);
	AddComponentTypeToFilter("GameRunning", ECSL::FilterType::RequiresOneOf);

	m_gameRunningId = -1;
	m_oldGameRunningId = -1;
}

void MasterServerSystem::PostInitialize()
{
	if (NetworkInstance::GetServer()->IsRunning())
	{
		m_clientDatabase = new ClientDatabase();
		if (m_clientDatabase->Connect())
		{
			std::string pw = NetworkInstance::GetServer()->GetServerPassword();
			int port = NetworkInstance::GetServer()->GetIncomingPort();
			m_clientDatabase->SetServerPort(port);
			m_clientDatabase->SetPasswordProtected(pw.size() > 0);
		}
	}
}

void MasterServerSystem::Update(float _dt)
{
	if (!NetworkInstance::GetServer()->IsRunning())
		return;

	m_clientDatabase->Update(_dt);

	if (m_oldGameRunningId != m_gameRunningId)
	{
		m_oldGameRunningId = m_gameRunningId;
		m_clientDatabase->SetGameStarted(m_gameRunningId > 0);
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