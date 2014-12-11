#include "SyncEntitiesSystem.h"
#include "../NetworkInstance.h"

SyncEntitiesSystem::SyncEntitiesSystem()
{
	m_timer = 0;
}
SyncEntitiesSystem::~SyncEntitiesSystem()
{
}

void SyncEntitiesSystem::Initialize()
{
	/*	Rendersystem wants Network	*/
	AddComponentTypeToFilter("SyncNetwork", ECSL::FilterType::Mandatory);

	printf("SyncEntitiesSystem initialized!\n");
}

void SyncEntitiesSystem::Update(float _dt)
{
	Network::ServerNetwork* server = NetworkInstance::GetServer();
	if (server->IsRunning())
	{
		if (m_timer > 0.2f)
		{
			m_timer = 0;
			auto entities = *GetEntities();

			for (int i = 0; i < entities.size(); ++i)
			{
				ECSL::BitSet::DataType* data;
				data = (ECSL::BitSet::DataType*)GetComponent(entities[i], "ChangedComponents", 0);
				std::vector<unsigned int> changedComponents;
				ECSL::BitSet::BitSetConverter::BitSetToArray
					(
					changedComponents,
					(const ECSL::BitSet::DataType*)data,
					ECSL::BitSet::GetIntCount(ECSL::ComponentTypeManager::GetInstance().GetComponentTypeCount())
					);

				if (changedComponents.size() > 0)
				{
					Network::Packet* p = NetworkInstance::GetNetworkHelper()->WriteEntity(server->GetPacketHandler(), entities[i]);
					server->Broadcast(p);
				}

			}
		}
		m_timer += _dt;
	}
}

void SyncEntitiesSystem::OnEntityAdded(unsigned int _entityId)
{

}

void SyncEntitiesSystem::OnEntityRemoved(unsigned int _entityId)
{

}