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
		bool active = *(bool*)GetComponent(matchId, "NetConnection", "Active");

		if (active) // If the matching user is still connected kick the new user
		{
			//NetworkInstance::GetServer()->Kick();
			KillEntity(_entityId);
		}
		else // If the matching user is not connected anymore, change the ip & port to the new one
		{
			bool setActive = true;

			SetComponent(matchId, "NetConnection", "IpAddress", ipAddress);
			SetComponent(matchId, "NetConnection", "Port", &port);
			SetComponent(matchId, "NetConnection", "Active", &setActive);
		}

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