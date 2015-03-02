#include "SyncEntitiesSystem.h"
#include "../Network/NetworkInstance.h"
#include "Game/Network/ClientManager.h"

SyncEntitiesSystem::SyncEntitiesSystem()
{
	m_timer = 0;
}
SyncEntitiesSystem::~SyncEntitiesSystem()
{
	delete(m_timer);
}

void SyncEntitiesSystem::Initialize()
{
	SetSystemName("SyncEntitiesSystem");

	SetUpdateTaskCount(1);//GetThreadCount() == 1 ? 1 : GetThreadCount() - 1);
	SetEntitiesAddedTaskCount(1);
	SetEntitiesRemovedTaskCount(1);

	/*	Rendersystem wants Network	*/
	AddComponentTypeToFilter("SyncNetwork", ECSL::FilterType::Mandatory);

	m_numberOfBitSets = ECSL::BitSet::GetDataTypeCount(ECSL::ComponentTypeManager::GetInstance().GetComponentTypeCount());
	m_componentId = ECSL::ComponentTypeManager::GetInstance().GetTableId("ChangedComponentsNetwork");
	m_timer = new float[GetUpdateTaskCount()];
	for (unsigned int i = 0; i < GetUpdateTaskCount(); ++i)
		m_timer[i] = 0.0f;
}

void SyncEntitiesSystem::Update(const ECSL::RuntimeInfo& _runtime)
{
	Network::ServerNetwork* server = NetworkInstance::GetServer();
	if (server->IsRunning())
	{
		if (m_timer[_runtime.TaskIndex] > 0.05f)
		{
			m_timer[_runtime.TaskIndex] = 0.0f;
			std::vector<Network::NetConnection> NCs = ClientManager::GetConnectedClients();
			auto entities = *GetEntities();
			std::vector<ECSL::DataLocation> components;
			GetComponents(components, &entities, m_componentId, 0);

			unsigned int startAt, endAt;
			MPL::MathHelper::SplitIterations(startAt, endAt, entities.size(), _runtime.TaskIndex, _runtime.TaskCount);
			for (unsigned int i = startAt; i < endAt; ++i)
			{
				ECSL::BitSet::DataType* data = (ECSL::BitSet::DataType*)components[i];
				bool hasChanged = false;
				for (unsigned int n = 0; n < m_numberOfBitSets; ++n)
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
						m_numberOfBitSets
						);

					Network::Packet* p = NetworkInstance::GetServerNetworkHelper()->WriteEntityDelta(server->GetPacketHandler(), entities[i], changedComponents);
					server->Send(p, NCs);
				}

				//data = (ECSL::BitSet::DataType*)GetComponent(entities[i], "ChangedComponentsNetwork", 0);
				for (unsigned int n = 0; n < m_numberOfBitSets; ++n)
					data[n] = 0;
			}
		}
		m_timer[_runtime.TaskIndex] += _runtime.Dt;
	}
}

void SyncEntitiesSystem::EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	Network::ServerNetwork* server = NetworkInstance::GetServer();
	if (server->IsRunning())
	{
		for (auto entityId : _entities)
		{
			Network::Packet* p = NetworkInstance::GetServerNetworkHelper()->WriteEntityAll(server->GetPacketHandler(), entityId);
            std::vector<Network::NetConnection> NCs = ClientManager::GetConnectedClients();

			server->Send(p, NCs);
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
			Network::Packet* p = NetworkInstance::GetServerNetworkHelper()->WriteEntityKill(server->GetPacketHandler(), entityId);
            std::vector<Network::NetConnection> NCs = ClientManager::GetConnectedClients();
			server->Send(p, NCs);
		}
	}
}