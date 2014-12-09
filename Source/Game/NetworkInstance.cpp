#include "NetworkInstance.h"

Network::ServerNetwork* NetworkInstance::m_server = 0;
Network::ClientNetwork* NetworkInstance::m_client = 0;

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

void NetworkInstance::DestroyServer()
{
	SAFE_DELETE(m_server);
}

void NetworkInstance::DestroyClient()
{
	SAFE_DELETE(m_client);
}

Network::ClientNetwork* NetworkInstance::GetClient()
{
	return m_client;
}

Network::ServerNetwork* NetworkInstance::GetServer()
{
	return m_server;
}

bool NetworkInstance::isServer()
{
	return m_server != 0;
}

bool NetworkInstance::isClient()
{
	return m_client != 0;
}