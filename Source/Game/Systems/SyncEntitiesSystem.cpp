#include "SyncEntitiesSystem.h"
#include "../NetworkInstance.h"

SyncEntitiesSystem::SyncEntitiesSystem()
{
	m_timer = 0;

	Network::NetMessageHook hook = std::bind(&NetworkHelper::ReceiveEntityAll, NetworkInstance::GetNetworkHelper(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	NetworkInstance::GetClient()->AddNetworkHook("Entity", hook);

	hook = std::bind(&NetworkHelper::ReceiveEntityDelta, NetworkInstance::GetNetworkHelper(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	NetworkInstance::GetClient()->AddNetworkHook("EntityDelta", hook);

	hook = std::bind(&NetworkHelper::ReceiveEntityKill, NetworkInstance::GetNetworkHelper(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	NetworkInstance::GetClient()->AddNetworkHook("EntityKill", hook);
}
SyncEntitiesSystem::~SyncEntitiesSystem()
{
}

void SyncEntitiesSystem::Initialize()
{
	SetSystemName("Sync Entities System");

	SetUpdateTaskCount(1);
	SetEntitiesAddedTaskCount(1);
	SetEntitiesRemovedTaskCount(1);

	/*	Rendersystem wants Network	*/
	AddComponentTypeToFilter("SyncNetwork", ECSL::FilterType::Mandatory);

	m_numberOfBitSets = ECSL::BitSet::GetDataTypeCount(ECSL::ComponentTypeManager::GetInstance().GetComponentTypeCount());
	m_componentId = ECSL::ComponentTypeManager::GetInstance().GetTableId("ChangedComponentsNetwork");
}

void SyncEntitiesSystem::Update(const ECSL::RuntimeInfo& _runtime)
{
	Network::ServerNetwork* server = NetworkInstance::GetServer();
	if (server->IsRunning())
	{
		if (m_timer > 0.05f)
		{
			m_timer = 0;
			auto entities = *GetEntities();

			for (auto entityId : entities)
			{
				ECSL::BitSet::DataType* data;
				data = (ECSL::BitSet::DataType*)GetComponent(entityId, m_componentId, 0);
				bool hasChanged = false;
				for (int n = 0; n < m_numberOfBitSets; ++n)
					if (data[n] != 0)
					{
						hasChanged = true;
						break;
					}


				if (hasChanged)
				{
					std::vector<unsigned int> changedComponents;
					ECSL::BitSet::BitSetConverter::BitSetToArray
						(
						changedComponents,
						(const ECSL::BitSet::DataType*)data,
						ECSL::BitSet::GetDataTypeCount(ECSL::ComponentTypeManager::GetInstance().GetComponentTypeCount())
						);
					Network::Packet* p = NetworkInstance::GetNetworkHelper()->WriteEntityDelta(server->GetPacketHandler(), entityId, changedComponents);
					server->Broadcast(p);
				}

				//data = (ECSL::BitSet::DataType*)GetComponent(entities[i], "ChangedComponentsNetwork", 0);
				for (int n = 0; n < ECSL::BitSet::GetDataTypeCount(ECSL::ComponentTypeManager::GetInstance().GetComponentTypeCount()); ++n)
					data[n] = 0;
			}
		}
		m_timer += _runtime.Dt;
	}
}

void SyncEntitiesSystem::EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	Network::ServerNetwork* server = NetworkInstance::GetServer();
	if (server->IsRunning())
	{
		for (auto entityId : _entities)
		{
			Network::Packet* p = NetworkInstance::GetNetworkHelper()->WriteEntityAll(server->GetPacketHandler(), entityId);
			server->Broadcast(p);
		}
	}
}

void SyncEntitiesSystem::EntitiesRemoved(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	Network::ServerNetwork* server = NetworkInstance::GetServer();
	if (server->IsRunning())
	{
		for (auto entityId : _entities)
		{
			Network::Packet* p = NetworkInstance::GetNetworkHelper()->WriteEntityKill(server->GetPacketHandler(), entityId);
			server->Broadcast(p);
		}
	}
}