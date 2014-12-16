#include "ReceivePacketSystem.h"
#include "../NetworkInstance.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include <sstream>

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

	Network::NetMessageHook hook = std::bind(&ReceivePacketSystem::LuaPacket, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	NetworkInstance::GetClient()->AddNetworkHook("LuaPacket", hook);
	NetworkInstance::GetServer()->AddNetworkHook("LuaPacket", hook);

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

void ReceivePacketSystem::LuaPacket(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
{
	std::ostringstream ss;
	ss << _id;

	char* function = _ph->ReadString(_id);

	LuaEmbedder::PushString(ss.str());
	LuaEmbedder::PushString(_nc.GetIpAddress());
	LuaEmbedder::PushInt((int)_nc.GetPort());
	LuaEmbedder::CallSavedFunction(function, 3);
}