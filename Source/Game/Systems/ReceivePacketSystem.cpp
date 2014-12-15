#include "ReceivePacketSystem.h"
#include "../NetworkInstance.h"

ReceivePacketSystem::ReceivePacketSystem()
{

}
ReceivePacketSystem::~ReceivePacketSystem()
{
}

void ReceivePacketSystem::Initialize()
{
	SetSystemName("Receive Packet System");

	/*	Rendersystem wants Network	*/
	AddComponentTypeToFilter("Null", ECSL::FilterType::Mandatory);

	printf("ReceivePacketSystem initialized!\n");
}

void ReceivePacketSystem::Update(float _dt)
{
	Network::ServerNetwork* server = NetworkInstance::GetServer();
	if (server->IsRunning())
	{
		while (server->PopAndExecutePacket() > 0) {}
	}

	Network::ClientNetwork* client = NetworkInstance::GetClient();
	if (client->IsConnected())
	{
		while (client->PopAndExecutePacket() > 0) { }
	}
}