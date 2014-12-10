#include "ReceivePacketSystem.h"

ReceivePacketSystem::ReceivePacketSystem(Network::ClientNetwork* _client, Network::ServerNetwork* _server)
{
	m_client = _client;
	m_server = _server;
}
ReceivePacketSystem::~ReceivePacketSystem()
{
	m_client = 0;
	m_server = 0;
}

void ReceivePacketSystem::Initialize()
{
	/*	Rendersystem wants Network	*/
	AddComponentTypeToFilter("Network", ECSL::FilterType::Mandatory);


	printf("ReceivePacketSystem initialized!\n");
}

void ReceivePacketSystem::Update(float _dt)
{
	if (m_server->IsRunning())
		m_server->Update(_dt);

	if(m_client->IsConnected())
		m_client->Update(_dt);

	if (m_server->IsRunning())
		while (m_server->GetPacket() > 0) {}

	if (m_client->IsConnected())
		while (m_client->GetPacket() > 0) {}

}

void ReceivePacketSystem::OnEntityAdded(unsigned int _entityId)
{

}

void ReceivePacketSystem::OnEntityRemoved(unsigned int _entityId)
{

}