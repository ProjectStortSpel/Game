#include "ReconnectSystem.h"
#include "../NetworkInstance.h"

ReconnectSystem::ReconnectSystem()
{
	m_timer = 0;
}
ReconnectSystem::~ReconnectSystem()
{
}

void ReconnectSystem::Initialize()
{
	/*	Rendersystem wants Network	*/
	AddComponentTypeToFilter("Username", ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("NetConnection", ECSL::FilterType::Mandatory);

	printf("ReconnectSystem initialized!\n");


	std::function<void(Network::NetConnection, const char*)> hook = std::bind(&ReconnectSystem::OnUserDisconnected, this, std::placeholders::_1, std::placeholders::_2);
	NetworkInstance::GetServer()->SetOnPlayerDisconnected(hook);

	//NetworkInstance::GetServer()->SetOnPlayerTimedOut(hook);
}

void ReconnectSystem::Update(float _dt)
{
	
}

void ReconnectSystem::OnEntityAdded(unsigned int _entityId)
{
	bool match = false;
	unsigned int matchId = 0;

	char* username		= GetComponent(_entityId, "Username", "Name");
	char* ipAddress		= GetComponent(_entityId, "NetConnection", "IpAddress");
	unsigned int port	= *(int*)GetComponent(_entityId, "NetConnection", "Port");


	auto entities = *GetEntities();

	for (int i = 0; i < entities.size(); ++i)
	{
		if (_entityId == entities[i])
			continue;

		if (strcmp(username, GetComponent(entities[i], "Username", 0)) == 0)
		{
			match = true;
			matchId = entities[i];
			break;
		}

	}

	if (match) // Found an already existing user
	{
		bool setActive = true;

		if (*(bool*)GetComponent(matchId, "NetConnection", "Active")) // If the matching user is still connected kick the new user
		{
			char* oldIp = GetComponent(matchId, "NetConnection", "IpAddress");
			unsigned int oldPort = *(int*)GetComponent(matchId, "NetConnection", "Port");

			NetworkInstance::GetServer()->Kick(Network::NetConnection(oldIp, oldPort), "Connected somewhere else");
		}

		SetComponent(matchId, "NetConnection", "IpAddress", ipAddress);
		SetComponent(matchId, "NetConnection", "Port", &port);
		SetComponent(matchId, "NetConnection", "Active", &setActive);

		KillEntity(_entityId);

	}
	else
	{
		bool setActive = true;
		SetComponent(_entityId, "NetConnection", "Active", &setActive);
	}

}

void ReconnectSystem::OnEntityRemoved(unsigned int _entityId)
{

}

void ReconnectSystem::OnUserDisconnected(Network::NetConnection _nc, const char* _message)
{
	const char* ipAddress = _nc.GetIpAddress();
	unsigned int port = _nc.GetPort();

	auto entities = *GetEntities();

	for (int i = 0; i < entities.size(); ++i)
	{
		if (strcmp(ipAddress, GetComponent(entities[i], "NetConnection", "IpAddress")) == 0
			&& port == *(int*)GetComponent(entities[i], "NetConnection", "Port"))
		{
			bool setActive = false;
			SetComponent(entities[i], "NetConnection", "Active", &setActive);
		}
	}

}