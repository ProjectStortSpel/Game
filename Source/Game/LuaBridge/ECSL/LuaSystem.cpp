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
		LuaEmbedder::EmbedClassFunction<LuaSystem>("System", "InitializeNetworkEvents", &LuaSystem::InitializeNetworkEvents);

		LuaEmbedder::AddInt("Mandatory", (int)ECSL::FilterType::Mandatory, "FilterType");
		LuaEmbedder::AddInt("RequiresOneOf", (int)ECSL::FilterType::RequiresOneOf, "FilterType");
		LuaEmbedder::AddInt("Excluded", (int)ECSL::FilterType::Excluded, "FilterType");
	}

	void LuaSystem::Update(float _dt)
	{
		if (LuaEmbedder::HasFunction<LuaSystem>(this, "Update"))
		{
			LuaEmbedder::PushFloat(_dt);
			LuaEmbedder::CallMethod<LuaSystem>("System", "Update", this, 1);
		}
	}

	void LuaSystem::Initialize()
	{
		if (LuaEmbedder::HasFunction<LuaSystem>(this, "Initialize"))
		{
			LuaEmbedder::CallMethod<LuaSystem>("System", "Initialize", this);
		}
	}

	void LuaSystem::OnEntityAdded(unsigned int _entityId)
	{
		if (LuaEmbedder::HasFunction<LuaSystem>(this, "OnEntityAdded"))
		{
			LuaEmbedder::PushInt((int)_entityId);
			LuaEmbedder::CallMethod<LuaSystem>("System", "OnEntityAdded", this, 1);
		}
	}

	void LuaSystem::OnEntityRemoved(unsigned int _entityId)
	{
		if (LuaEmbedder::HasFunction<LuaSystem>(this, "OnEntityRemoved"))
		{
			LuaEmbedder::PushInt((int)_entityId);
			LuaEmbedder::CallMethod<LuaSystem>("System", "OnEntityRemoved", this, 1);
		}
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


	int LuaSystem::AddComponentTypeToFilter()
	{
		std::string componentType = LuaEmbedder::PullString(1);
		ECSL::FilterType filterType = (ECSL::FilterType)LuaEmbedder::PullInt(2);
		System::AddComponentTypeToFilter(componentType, filterType);
		return 0;
	}

	int LuaSystem::GetEntities()
	{
		LuaEmbedder::PushUnsignedIntArray(System::GetEntities()->data(), System::GetEntities()->size());
		return 1;
	}

	int LuaSystem::InitializeNetworkEvents()
	{
		NetworkInstance::GetClient()->SetOnBannedFromServer(std::bind(&LuaSystem::OnBannedFromServer, this, std::placeholders::_1, std::placeholders::_2));
		NetworkInstance::GetClient()->SetOnConnectedToServer(std::bind(&LuaSystem::OnConnectedToServer, this, std::placeholders::_1, std::placeholders::_2));
		NetworkInstance::GetClient()->SetOnDisconnectedFromServer(std::bind(&LuaSystem::OnDisconnectedFromServer, this, std::placeholders::_1, std::placeholders::_2));
		NetworkInstance::GetClient()->SetOnFailedToConnect(std::bind(&LuaSystem::OnFailedToConnect, this, std::placeholders::_1, std::placeholders::_2));
		NetworkInstance::GetClient()->SetOnKickedFromServer(std::bind(&LuaSystem::OnKickedFromServer, this, std::placeholders::_1, std::placeholders::_2));
		NetworkInstance::GetClient()->SetOnPasswordInvalid(std::bind(&LuaSystem::OnPasswordInvalid, this, std::placeholders::_1, std::placeholders::_2));
		NetworkInstance::GetClient()->SetOnRemotePlayerBanned(std::bind(&LuaSystem::OnRemotePlayerBanned, this, std::placeholders::_1, std::placeholders::_2));
		NetworkInstance::GetClient()->SetOnRemotePlayerConnected(std::bind(&LuaSystem::OnRemotePlayerConnected, this, std::placeholders::_1, std::placeholders::_2));
		NetworkInstance::GetClient()->SetOnRemotePlayerDisconnected(std::bind(&LuaSystem::OnRemotePlayerDisconnected, this, std::placeholders::_1, std::placeholders::_2));
		NetworkInstance::GetClient()->SetOnRemotePlayerKicked(std::bind(&LuaSystem::OnRemotePlayerKicked, this, std::placeholders::_1, std::placeholders::_2));
		NetworkInstance::GetClient()->SetOnServerFull(std::bind(&LuaSystem::OnServerFull, this, std::placeholders::_1, std::placeholders::_2));
		NetworkInstance::GetClient()->SetOnTimedOutFromServer(std::bind(&LuaSystem::OnTimedOutFromServer, this, std::placeholders::_1, std::placeholders::_2));

		NetworkInstance::GetServer()->SetOnPlayerConnected(std::bind(&LuaSystem::OnPlayerConnected, this, std::placeholders::_1, std::placeholders::_2));
		NetworkInstance::GetServer()->SetOnPlayerDisconnected(std::bind(&LuaSystem::OnPlayerDisconnected, this, std::placeholders::_1, std::placeholders::_2));
		NetworkInstance::GetServer()->SetOnPlayerTimedOut(std::bind(&LuaSystem::OnPlayerTimedOut, this, std::placeholders::_1, std::placeholders::_2));

		return 0;
	}

	void LuaSystem::OnBannedFromServer(Network::NetConnection _nc, const char* _message)
	{
		LuaEmbedder::PushString(_nc.GetIpAddress());
		LuaEmbedder::PushInt((int)_nc.GetPort());
		LuaEmbedder::PushString(_message);
		LuaEmbedder::CallMethod<LuaSystem>("System", "OnBannedFromServer", this, 3);
	}
	void LuaSystem::OnConnectedToServer(Network::NetConnection _nc, const char* _message)
	{
		LuaEmbedder::PushString(_nc.GetIpAddress());
		LuaEmbedder::PushInt((int)_nc.GetPort());

		LuaEmbedder::CallMethod<LuaSystem>("System", "OnConnectedToServer", this, 2);
	}
	void LuaSystem::OnDisconnectedFromServer(Network::NetConnection _nc, const char* _message)
	{
		LuaEmbedder::PushString(_nc.GetIpAddress());
		LuaEmbedder::PushInt((int)_nc.GetPort());
		LuaEmbedder::CallMethod<LuaSystem>("System", "OnDisconnectedFromServer", this, 2);
	}
	void LuaSystem::OnFailedToConnect(Network::NetConnection _nc, const char* _message)
	{
		LuaEmbedder::PushString(_nc.GetIpAddress());
		LuaEmbedder::PushInt((int)_nc.GetPort());
		LuaEmbedder::CallMethod<LuaSystem>("System", "OnFailedToConnect", this, 2);
	}
	void LuaSystem::OnKickedFromServer(Network::NetConnection _nc, const char* _message)
	{
		LuaEmbedder::PushString(_nc.GetIpAddress());
		LuaEmbedder::PushInt((int)_nc.GetPort());
		LuaEmbedder::PushString(_message);
		LuaEmbedder::CallMethod<LuaSystem>("System", "OnKickedFromServer", this, 3);
	}
	void LuaSystem::OnPasswordInvalid(Network::NetConnection _nc, const char* _message)
	{
		LuaEmbedder::PushString(_nc.GetIpAddress());
		LuaEmbedder::PushInt((int)_nc.GetPort());
		LuaEmbedder::CallMethod<LuaSystem>("System", "OnPasswordInvalid", this, 2);
	}
	void LuaSystem::OnRemotePlayerBanned(Network::NetConnection _nc, const char* _message)
	{
		LuaEmbedder::PushString(_message);
		LuaEmbedder::CallMethod<LuaSystem>("System", "OnRemotePlayerBanned", this, 1);
	}
	void LuaSystem::OnRemotePlayerConnected(Network::NetConnection _nc, const char* _message)
	{
		LuaEmbedder::PushString(_message);
		LuaEmbedder::CallMethod<LuaSystem>("System", "OnRemotePlayerConnected", this, 1);
	}
	void LuaSystem::OnRemotePlayerDisconnected(Network::NetConnection _nc, const char* _message)
	{
		LuaEmbedder::PushString(_message);
		LuaEmbedder::CallMethod<LuaSystem>("System", "OnRemotePlayerDisconnected", this, 1);
	}
	void LuaSystem::OnRemotePlayerKicked(Network::NetConnection _nc, const char* _message)
	{
		LuaEmbedder::PushString(_message);
		LuaEmbedder::CallMethod<LuaSystem>("System", "OnRemotePlayerKicked", this, 1);
	}
	void LuaSystem::OnServerFull(Network::NetConnection _nc, const char* _message)
	{
		LuaEmbedder::PushString(_nc.GetIpAddress());
		LuaEmbedder::PushInt((int)_nc.GetPort());
		LuaEmbedder::CallMethod<LuaSystem>("System", "OnServerFull", this, 2);
	}
	void LuaSystem::OnTimedOutFromServer(Network::NetConnection _nc, const char* _message)
	{
		LuaEmbedder::PushString(_nc.GetIpAddress());
		LuaEmbedder::PushInt((int)_nc.GetPort());
		LuaEmbedder::CallMethod<LuaSystem>("System", "OnTimedOutFromServer", this, 2);
	}

	void LuaSystem::OnPlayerConnected(Network::NetConnection _nc, const char* _message)
	{
		LuaEmbedder::PushString(_nc.GetIpAddress());
		LuaEmbedder::PushInt((int)_nc.GetPort());
		LuaEmbedder::PushString(_message);
		LuaEmbedder::CallMethod<LuaSystem>("System", "OnPlayerConnected", this, 3);
	}
	void LuaSystem::OnPlayerDisconnected(Network::NetConnection _nc, const char* _message)
	{
		LuaEmbedder::PushString(_nc.GetIpAddress());
		LuaEmbedder::PushInt((int)_nc.GetPort());
		LuaEmbedder::PushString(_message);
		LuaEmbedder::CallMethod<LuaSystem>("System", "OnPlayerDisconnected", this, 3);
	}
	void LuaSystem::OnPlayerTimedOut(Network::NetConnection _nc, const char* _message)
	{
		LuaEmbedder::PushString(_nc.GetIpAddress());
		LuaEmbedder::PushInt((int)_nc.GetPort());
		LuaEmbedder::PushString(_message);
		LuaEmbedder::CallMethod<LuaSystem>("System", "OnPlayerTimedOut", this, 3);
	}
}