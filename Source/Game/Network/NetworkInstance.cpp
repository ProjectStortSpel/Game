#include "NetworkInstance.h"

Network::ServerNetwork* NetworkInstance::m_server = 0;
Network::ClientNetwork* NetworkInstance::m_client = 0;
NetworkHelper* NetworkInstance::m_clientNetworkHelper = 0;
NetworkHelper* NetworkInstance::m_serverNetworkHelper = 0;

void NetworkInstance::InitServer()
{
	SAFE_DELETE(m_server);
	m_server = new Network::ServerNetwork();
}

void NetworkInstance::InitClient()
{
	SAFE_DELETE(m_client);
	m_client = new Network::ClientNetwork();
}

void NetworkInstance::InitClientNetworkHelper(ECSL::World** _world)
{
	SAFE_DELETE(m_clientNetworkHelper);
	m_clientNetworkHelper = new NetworkHelper(_world);
}

void NetworkInstance::InitServerNetworkHelper(ECSL::World** _world)
{
	SAFE_DELETE(m_serverNetworkHelper);
	m_serverNetworkHelper = new NetworkHelper(_world);
}

void NetworkInstance::DestroyServer()
{
	SAFE_DELETE(m_server);
}

void NetworkInstance::DestroyClient()
{
	SAFE_DELETE(m_client);
}

void NetworkInstance::DestroyClientNetworkHelper()
{
	SAFE_DELETE(m_clientNetworkHelper);
}

void NetworkInstance::DestroyServerNetworkHelper()
{
	SAFE_DELETE(m_serverNetworkHelper);
}

Network::ClientNetwork* NetworkInstance::GetClient()
{
	return m_client;
}

Network::ServerNetwork* NetworkInstance::GetServer()
{
	return m_server;
}

NetworkHelper* NetworkInstance::GetClientNetworkHelper()
{
	return m_clientNetworkHelper;
}

NetworkHelper* NetworkInstance::GetServerNetworkHelper()
{
	return m_serverNetworkHelper;
}

bool NetworkInstance::isServer()
{
	return m_server != 0;
}

bool NetworkInstance::isClient()
{
	return m_client != 0;
}