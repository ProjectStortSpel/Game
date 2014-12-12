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

	/*	Rendersystem wants Network	*/
	AddComponentTypeToFilter("SyncNetwork", ECSL::FilterType::Mandatory);

	printf("SyncEntitiesSystem initialized!\n");
}

void SyncEntitiesSystem::Update(float _dt)
{
	Network::ServerNetwork* server = NetworkInstance::GetServer();
	if (server->IsRunning())
	{
		if (m_timer > 0.05f)
		{
			m_timer = 0;
			auto entities = *GetEntities();

			for (int i = 0; i < entities.size(); ++i)
			{
				ECSL::BitSet::DataType* data;
				data = (ECSL::BitSet::DataType*)GetComponent(entities[i], "ChangedComponentsNetwork", 0);
				std::vector<unsigned int> changedComponents;
				ECSL::BitSet::BitSetConverter::BitSetToArray
					(
					changedComponents,
					(const ECSL::BitSet::DataType*)data,
					ECSL::BitSet::GetIntCount(ECSL::ComponentTypeManager::GetInstance().GetComponentTypeCount())
					);

				if (changedComponents.size() > 0)
				{
					Network::Packet* p = NetworkInstance::GetNetworkHelper()->WriteEntityDelta(server->GetPacketHandler(), entities[i], changedComponents);
					server->Broadcast(p);
				}

				//data = (ECSL::BitSet::DataType*)GetComponent(entities[i], "ChangedComponentsNetwork", 0);
				for (int n = 0; n < ECSL::BitSet::GetIntCount(ECSL::ComponentTypeManager::GetInstance().GetComponentTypeCount()); ++n)
					data[n] = 0;
			}
		}
		m_timer += _dt;
	}
}

void SyncEntitiesSystem::OnEntityAdded(unsigned int _entityId)
{
	Network::ServerNetwork* server = NetworkInstance::GetServer();
	if (server->IsRunning())
	{
		Network::Packet* p = NetworkInstance::GetNetworkHelper()->WriteEntityAll(server->GetPacketHandler(), _entityId);
		server->Broadcast(p);
	}
}

void SyncEntitiesSystem::OnEntityRemoved(unsigned int _entityId)
{
	Network::ServerNetwork* server = NetworkInstance::GetServer();
	if (server->IsRunning())
	{
		Network::Packet* p = NetworkInstance::GetNetworkHelper()->WriteEntityKill(server->GetPacketHandler(), _entityId);
		server->Broadcast(p);
	}
}