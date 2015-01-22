#include "ClientDatabase.h"
#include "Game/NetworkInstance.h"

ClientDatabase& ClientDatabase::GetInstance()
{
	static ClientDatabase* instance = new ClientDatabase();
	return *instance;
}


ClientDatabase::ClientDatabase()
	:m_ipAddress("194.47.150.128"), m_password("DefaultMasterPassword"), m_remotePort(5509), m_localPort(0)
{

	Network::NetEvent  hook;
	hook = std::bind(&ClientDatabase::OnPlayerConnected, this, std::placeholders::_1, std::placeholders::_2);
	NetworkInstance::GetServer()->SetOnPlayerConnected(hook);

	hook = std::bind(&ClientDatabase::OnPlayerDisconnected, this, std::placeholders::_1, std::placeholders::_2);
	NetworkInstance::GetServer()->SetOnPlayerDisconnected(hook);

	hook = std::bind(&ClientDatabase::OnPlayerTimedOut, this, std::placeholders::_1, std::placeholders::_2);
	NetworkInstance::GetServer()->SetOnPlayerTimedOut(hook);



	m_client.SetMaxTimeOutIntervall(500);
	m_client.SetMaxTimeOutCounter(2);
}
ClientDatabase::~ClientDatabase()
{
}

bool ClientDatabase::Connect()
{
	m_client.Connect(m_ipAddress.c_str(), m_password.c_str(), m_remotePort, m_localPort);
	return true;
}
bool ClientDatabase::Disconnect()
{
	m_client.Disconnect();
	return true;
}

void ClientDatabase::Update(float dt)
{
	if (!m_client.IsConnected())
		return;

	m_client.Update(dt);
	while (m_client.PopAndExecutePacket() > 0) {}
}


void ClientDatabase::OnPlayerConnected(Network::NetConnection& _nc, const char* _msg)
{
	SDL_Log("ClientDatabase::OnPlayerConnected\n");

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("PLAYER_COUNT_INCREASED");
	auto packet = ph->EndPack(id);

	m_client.Send(packet);
}

void ClientDatabase::OnPlayerDisconnected(Network::NetConnection& _nc, const char* _msg)
{
	SDL_Log("ClientDatabase::OnPlayerConnected\n");

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("PLAYER_COUNT_DECREASED");
	auto packet = ph->EndPack(id);

	m_client.Send(packet);
}

void ClientDatabase::OnPlayerTimedOut(Network::NetConnection& _nc, const char* _msg)
{
	SDL_Log("ClientDatabase::OnPlayerConnected\n");

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("PLAYER_COUNT_DECREASED");
	auto packet = ph->EndPack(id);

	m_client.Send(packet);
}