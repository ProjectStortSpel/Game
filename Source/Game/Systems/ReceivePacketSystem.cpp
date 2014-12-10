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
	/*	Rendersystem wants Network	*/
	AddComponentTypeToFilter("Network", ECSL::FilterType::Mandatory);


	printf("ReceivePacketSystem initialized!\n");
}

void ReceivePacketSystem::Update(float _dt)
{
	
	if (NetworkInstance::GetServer()->IsRunning())
		NetworkInstance::GetServer()->Update(_dt);

	if (NetworkInstance::GetClient()->IsConnected())
		NetworkInstance::GetClient()->Update(_dt);

	if (NetworkInstance::GetServer()->IsRunning())
		while (NetworkInstance::GetServer()->GetPacket() > 0) {}

	if (NetworkInstance::GetClient()->IsConnected())
		while (NetworkInstance::GetClient()->GetPacket() > 0) {}

}

void ReceivePacketSystem::OnEntityAdded(unsigned int _entityId)
{

}

void ReceivePacketSystem::OnEntityRemoved(unsigned int _entityId)
{

}