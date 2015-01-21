#include "LuaSystem.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "LuaComponent.h"
#include "../../NetworkInstance.h"

namespace LuaBridge
{
	LuaSystem::LuaSystem() { }

	void LuaSystem::Embed()
	{
		LuaEmbedder::EmbedClass<LuaSystem>("System", false);
		LuaEmbedder::EmbedClassFunction<LuaSystem>("System", "AddComponentTypeToFilter", &LuaSystem::AddComponentTypeToFilter);
		LuaEmbedder::EmbedClassFunction<LuaSystem>("System", "GetEntities", &LuaSystem::GetEntities);
		LuaEmbedder::EmbedClassFunction<LuaSystem>("System", "GetComponent", &LuaSystem::GetComponent);
		LuaEmbedder::EmbedClassFunction<LuaSystem>("System", "EntityHasComponent", &LuaSystem::EntityHasComponent);
		LuaEmbedder::EmbedClassFunction<LuaSystem>("System", "InitializeNetworkEvents", &LuaSystem::InitializeNetworkEvents);

		LuaEmbedder::EmbedClassFunction<LuaSystem>("System", "SetUpdateTaskCount", &LuaSystem::SetUpdateTaskCount);
		LuaEmbedder::EmbedClassFunction<LuaSystem>("System", "SetEntitiesAddedTaskCount", &LuaSystem::SetEntitiesAddedTaskCount);
		LuaEmbedder::EmbedClassFunction<LuaSystem>("System", "SetEntitiesRemovedTaskCount", &LuaSystem::SetEntitiesRemovedTaskCount);

		LuaEmbedder::EmbedClassFunction<LuaSystem>("System", "SetName", &LuaSystem::SetName);

		LuaEmbedder::AddInt("Mandatory", (int)ECSL::FilterType::Mandatory, "FilterType");
		LuaEmbedder::AddInt("RequiresOneOf", (int)ECSL::FilterType::RequiresOneOf, "FilterType");
		LuaEmbedder::AddInt("Excluded", (int)ECSL::FilterType::Excluded, "FilterType");
	}

	void LuaSystem::Update(const ECSL::RuntimeInfo& _runtime)
	{
		LuaEmbedder::PushFloat(_runtime.Dt);
		LuaEmbedder::PushInt(_runtime.TaskIndex);
		LuaEmbedder::PushInt(_runtime.TaskCount);
		LuaEmbedder::CallMethod<LuaSystem>("System", "Update", this, 3);
	}

	void LuaSystem::Initialize()
	{
		if (LuaEmbedder::HasFunction<LuaSystem>(this, "Initialize"))
		{
			LuaEmbedder::CallMethod<LuaSystem>("System", "Initialize", this);
		}
	}

	void LuaSystem::EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
	{
		LuaEmbedder::PushFloat(_runtime.Dt);
		LuaEmbedder::PushInt(_runtime.TaskIndex);
		LuaEmbedder::PushInt(_runtime.TaskCount);
		LuaEmbedder::PushUnsignedIntArray(_entities.data(), _entities.size(), false);
		LuaEmbedder::CallMethod<LuaSystem>("System", "EntitiesAdded", this, 4);
	}

	void LuaSystem::EntitiesRemoved(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
	{
		LuaEmbedder::PushFloat(_runtime.Dt);
		LuaEmbedder::PushInt(_runtime.TaskIndex);
		LuaEmbedder::PushInt(_runtime.TaskCount);
		LuaEmbedder::PushUnsignedIntArray(_entities.data(), _entities.size(), false);
		LuaEmbedder::CallMethod<LuaSystem>("System", "EntitiesRemoved", this, 4);
	}

	void LuaSystem::PostInitialize()
	{
		if (LuaEmbedder::HasFunction<LuaSystem>(this, "PostInitialize"))
		{
			LuaEmbedder::CallMethod<LuaSystem>("System", "PostInitialize", this);
		}
	}

	int LuaSystem::GetComponent()
	{
		ECSL::DataLocation dataLocation;
		unsigned int entityId = (unsigned int)LuaEmbedder::PullInt(1);
		std::string componentType = LuaEmbedder::PullString(2);
		if (LuaEmbedder::IsInt(3))
		{
			unsigned int index = (unsigned int)LuaEmbedder::PullInt(3);
			dataLocation = System::GetComponent(entityId, componentType, index);
		}
		else
		{
			std::string variableName = LuaEmbedder::PullString(3);
			dataLocation = System::GetComponent(entityId, componentType, variableName);
		}
		LuaComponent* component = new LuaComponent(dataLocation, this, entityId, componentType);
		LuaEmbedder::PushObject<LuaComponent>("Component", component, true);
		return 1;
	}

	int LuaSystem::SetName()
	{
		std::string systemName = LuaEmbedder::PullString(1);
		SetSystemName(systemName);
		return 0;
	}

	int LuaSystem::AddComponentTypeToFilter()
	{
		std::string componentType = LuaEmbedder::PullString(1);
		ECSL::FilterType filterType = (ECSL::FilterType)LuaEmbedder::PullInt(2);
		System::AddComponentTypeToFilter(componentType, filterType);
		return 0;
	}

	int LuaSystem::GetEntities()
	{
		const std::vector<unsigned int>* entities = System::GetEntities();

		if (LuaEmbedder::IsString(1))
		{
			std::string comp = LuaEmbedder::PullString(1);
			unsigned int componentTypeId = ECSL::ComponentTypeManager::GetInstance().GetTableId(comp);

			unsigned int* selectedEntities = new unsigned int[entities->size()];

			int index = 0;
			for (int i = 0; i < entities->size(); ++i)
			{
				if (System::EntityHasComponent(entities->at(i), componentTypeId))
				{
					selectedEntities[index] = entities->at(i);
					++index;
				}
			}
			LuaEmbedder::PushUnsignedIntArray(selectedEntities, index);
		}
		else
		{
			LuaEmbedder::PushUnsignedIntArray(entities->data(), entities->size(), false);
		}

		return 1;
	}
	
	int LuaSystem::EntityHasComponent()
	{
		unsigned int entityId = (unsigned int)LuaEmbedder::PullInt(1);
		unsigned int componentTypeId = ECSL::ComponentTypeManager::GetInstance().GetTableId(LuaEmbedder::PullString(2));
		LuaEmbedder::PushBool(System::EntityHasComponent(entityId, componentTypeId));
		return 1;
	}

	int LuaSystem::InitializeNetworkEvents()
	{
		Network::NetEvent  hook;

		hook = std::bind(&LuaSystem::OnBannedFromServer, this, std::placeholders::_1, std::placeholders::_2);
		NetworkInstance::GetClient()->SetOnBannedFromServer(hook);

		hook = std::bind(&LuaSystem::OnConnectedToServer, this, std::placeholders::_1, std::placeholders::_2);
		NetworkInstance::GetClient()->SetOnConnectedToServer(hook);

		hook = std::bind(&LuaSystem::OnDisconnectedFromServer, this, std::placeholders::_1, std::placeholders::_2);
		NetworkInstance::GetClient()->SetOnDisconnectedFromServer(hook);

		hook = std::bind(&LuaSystem::OnFailedToConnect, this, std::placeholders::_1, std::placeholders::_2);
		NetworkInstance::GetClient()->SetOnFailedToConnect(hook);

		hook = std::bind(&LuaSystem::OnKickedFromServer, this, std::placeholders::_1, std::placeholders::_2);
		NetworkInstance::GetClient()->SetOnKickedFromServer(hook);

		hook = std::bind(&LuaSystem::OnPasswordInvalid, this, std::placeholders::_1, std::placeholders::_2);
		NetworkInstance::GetClient()->SetOnPasswordInvalid(hook);

		hook = std::bind(&LuaSystem::OnRemotePlayerBanned, this, std::placeholders::_1, std::placeholders::_2);
		NetworkInstance::GetClient()->SetOnRemotePlayerBanned(hook);

		hook = std::bind(&LuaSystem::OnRemotePlayerConnected, this, std::placeholders::_1, std::placeholders::_2);
		NetworkInstance::GetClient()->SetOnRemotePlayerConnected(hook);

		hook = std::bind(&LuaSystem::OnRemotePlayerDisconnected, this, std::placeholders::_1, std::placeholders::_2);
		NetworkInstance::GetClient()->SetOnRemotePlayerDisconnected(hook);

		hook = std::bind(&LuaSystem::OnRemotePlayerKicked, this, std::placeholders::_1, std::placeholders::_2);
		NetworkInstance::GetClient()->SetOnRemotePlayerKicked(hook);

		hook = std::bind(&LuaSystem::OnServerFull, this, std::placeholders::_1, std::placeholders::_2);
		NetworkInstance::GetClient()->SetOnServerFull(hook);

		hook = std::bind(&LuaSystem::OnTimedOutFromServer, this, std::placeholders::_1, std::placeholders::_2);
		NetworkInstance::GetClient()->SetOnTimedOutFromServer(hook);



		hook = std::bind(&LuaSystem::OnPlayerConnected, this, std::placeholders::_1, std::placeholders::_2);
		NetworkInstance::GetServer()->SetOnPlayerConnected(hook);

		hook = std::bind(&LuaSystem::OnPlayerDisconnected, this, std::placeholders::_1, std::placeholders::_2);
		NetworkInstance::GetServer()->SetOnPlayerDisconnected(hook);

		hook = std::bind(&LuaSystem::OnPlayerTimedOut, this, std::placeholders::_1, std::placeholders::_2);
		NetworkInstance::GetServer()->SetOnPlayerTimedOut(hook);

		return 0;
	}

	int LuaSystem::SetUpdateTaskCount()
	{
		int updateTaskCount = LuaEmbedder::PullInt(1);
		System::SetUpdateTaskCount(updateTaskCount);
		return 0;
	}
	int LuaSystem::SetEntitiesAddedTaskCount()
	{
		int entitiesAddedTaskCount = LuaEmbedder::PullInt(1);
		System::SetEntitiesAddedTaskCount(entitiesAddedTaskCount);
		return 0;
	}
	int LuaSystem::SetEntitiesRemovedTaskCount()
	{
		int entitiesRemovedTaskCount = LuaEmbedder::PullInt(1);
		System::SetEntitiesAddedTaskCount(entitiesRemovedTaskCount);
		return 0;
	}

	void LuaSystem::OnBannedFromServer(Network::NetConnection _nc, const char* _message)
	{
		if (LuaEmbedder::HasFunction<LuaSystem>(this, "OnBannedFromServer"))
		{
			LuaEmbedder::PushString(_nc.GetIpAddress());
			LuaEmbedder::PushInt((int)_nc.GetPort());
			LuaEmbedder::PushString("OnBannedFromServer");
			LuaEmbedder::CallMethod<LuaSystem>("System", "OnBannedFromServer", this, 3);
		}
	}
	void LuaSystem::OnConnectedToServer(Network::NetConnection _nc, const char* _message)
	{
		if (LuaEmbedder::HasFunction<LuaSystem>(this, "OnConnectedToServer"))
		{
			LuaEmbedder::PushString(_nc.GetIpAddress());
			LuaEmbedder::PushInt((int)_nc.GetPort());
			LuaEmbedder::CallMethod<LuaSystem>("System", "OnConnectedToServer", this, 2);
		}
	}
	void LuaSystem::OnDisconnectedFromServer(Network::NetConnection _nc, const char* _message)
	{
		if (LuaEmbedder::HasFunction<LuaSystem>(this, "OnDisconnectedFromServer"))
		{
			LuaEmbedder::PushString(_nc.GetIpAddress());
			LuaEmbedder::PushInt((int)_nc.GetPort());
			LuaEmbedder::CallMethod<LuaSystem>("System", "OnDisconnectedFromServer", this, 2);
		}
	}
	void LuaSystem::OnFailedToConnect(Network::NetConnection _nc, const char* _message)
	{
		if (LuaEmbedder::HasFunction<LuaSystem>(this, "OnFailedToConnect"))
		{
			LuaEmbedder::PushString(_nc.GetIpAddress());
			LuaEmbedder::PushInt((int)_nc.GetPort());
			LuaEmbedder::CallMethod<LuaSystem>("System", "OnFailedToConnect", this, 2);
		}
	}
	void LuaSystem::OnKickedFromServer(Network::NetConnection _nc, const char* _message)
	{
		if (LuaEmbedder::HasFunction<LuaSystem>(this, "OnKickedFromServer"))
		{
			LuaEmbedder::PushString(_nc.GetIpAddress());
			LuaEmbedder::PushInt((int)_nc.GetPort());
			LuaEmbedder::PushString(_message);
			LuaEmbedder::CallMethod<LuaSystem>("System", "OnKickedFromServer", this, 3);
		}
	}
	void LuaSystem::OnPasswordInvalid(Network::NetConnection _nc, const char* _message)
	{
		if (LuaEmbedder::HasFunction<LuaSystem>(this, "OnPasswordInvalid"))
		{
			LuaEmbedder::PushString(_nc.GetIpAddress());
			LuaEmbedder::PushInt((int)_nc.GetPort());
			LuaEmbedder::CallMethod<LuaSystem>("System", "OnPasswordInvalid", this, 2);
		}
	}
	void LuaSystem::OnRemotePlayerBanned(Network::NetConnection _nc, const char* _message)
	{
		if (LuaEmbedder::HasFunction<LuaSystem>(this, "OnRemotePlayerBanned"))
		{
			LuaEmbedder::PushString("OnRemotePlayerBanned");
			LuaEmbedder::CallMethod<LuaSystem>("System", "OnRemotePlayerBanned", this, 1);
		}
	}
	void LuaSystem::OnRemotePlayerConnected(Network::NetConnection _nc, const char* _message)
	{
		if (LuaEmbedder::HasFunction<LuaSystem>(this, "OnRemotePlayerConnected"))
		{
			LuaEmbedder::PushString("OnRemotePlayerConnected");
			LuaEmbedder::CallMethod<LuaSystem>("System", "OnRemotePlayerConnected", this, 1);
		}
	}
	void LuaSystem::OnRemotePlayerDisconnected(Network::NetConnection _nc, const char* _message)
	{
		if (LuaEmbedder::HasFunction<LuaSystem>(this, "OnRemotePlayerDisconnected"))
		{
			LuaEmbedder::PushString("OnRemotePlayerDisconnected");
			LuaEmbedder::CallMethod<LuaSystem>("System", "OnRemotePlayerDisconnected", this, 1);

		}
	}
	void LuaSystem::OnRemotePlayerKicked(Network::NetConnection _nc, const char* _message)
	{
		if (LuaEmbedder::HasFunction<LuaSystem>(this, "OnRemotePlayerKicked"))
		{
			LuaEmbedder::PushString("OnRemotePlayerKicked");
			LuaEmbedder::CallMethod<LuaSystem>("System", "OnRemotePlayerKicked", this, 1);
		}
	}
	void LuaSystem::OnServerFull(Network::NetConnection _nc, const char* _message)
	{
		if (LuaEmbedder::HasFunction<LuaSystem>(this, "OnServerFull"))
		{
			LuaEmbedder::PushString(_nc.GetIpAddress());
			LuaEmbedder::PushInt((int)_nc.GetPort());
			LuaEmbedder::CallMethod<LuaSystem>("System", "OnServerFull", this, 2);
		}
	}
	void LuaSystem::OnTimedOutFromServer(Network::NetConnection _nc, const char* _message)
	{
		if (LuaEmbedder::HasFunction<LuaSystem>(this, "OnTimedOutFromServer"))
		{
			LuaEmbedder::PushString(_nc.GetIpAddress());
			LuaEmbedder::PushInt((int)_nc.GetPort());
			LuaEmbedder::CallMethod<LuaSystem>("System", "OnTimedOutFromServer", this, 2);
		}
	}

	void LuaSystem::OnPlayerConnected(Network::NetConnection _nc, const char* _message)
	{
		if (LuaEmbedder::HasFunction<LuaSystem>(this, "OnPlayerConnected"))
		{
			LuaEmbedder::PushString(_nc.GetIpAddress());
			LuaEmbedder::PushInt((int)_nc.GetPort());
			LuaEmbedder::PushString("OnPlayerConnected");
			LuaEmbedder::CallMethod<LuaSystem>("System", "OnPlayerConnected", this, 3);
		}
	}
	void LuaSystem::OnPlayerDisconnected(Network::NetConnection _nc, const char* _message)
	{
		if (LuaEmbedder::HasFunction<LuaSystem>(this, "OnPlayerDisconnected"))
		{
			LuaEmbedder::PushString(_nc.GetIpAddress());
			LuaEmbedder::PushInt((int)_nc.GetPort());
			LuaEmbedder::PushString("OnPlayerDisconnected");
			LuaEmbedder::CallMethod<LuaSystem>("System", "OnPlayerDisconnected", this, 3);
		}
	}
	void LuaSystem::OnPlayerTimedOut(Network::NetConnection _nc, const char* _message)
	{
		if (LuaEmbedder::HasFunction<LuaSystem>(this, "OnPlayerTimedOut"))
		{
			LuaEmbedder::PushString(_nc.GetIpAddress());
			LuaEmbedder::PushInt((int)_nc.GetPort());
			LuaEmbedder::PushString("OnPlayerTimedOut");
			LuaEmbedder::CallMethod<LuaSystem>("System", "OnPlayerTimedOut", this, 3);
		}
	}
}