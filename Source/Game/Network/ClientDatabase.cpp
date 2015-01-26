#include "ClientDatabase.h"
#include "Game/NetworkInstance.h"

ClientDatabase::ClientDatabase()
	:m_ipAddress("194.47.150.128"), m_password("DefaultMasterPassword"), m_remotePort(5509), m_localPort(0)
{
	m_client.SetMaxTimeOutIntervall(5);
	m_client.SetMaxTimeOutCounter(2);
}
ClientDatabase::~ClientDatabase()
{
	Disconnect();
}

bool ClientDatabase::Connect()
{
	bool result = m_client.Connect(m_ipAddress.c_str(), m_password.c_str(), m_remotePort, m_localPort);
	return result;
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

void ClientDatabase::SetGameStarted(bool _started)
{
	if (!m_client.IsConnected())
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_SYSTEM, "Tried to send \"GAME_STARTED\", but is not connected to MasterServer");
		return;
	}

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("GAME_STARTED");
	ph->WriteByte(id, _started);
	auto packet = ph->EndPack(id);

	m_client.Send(packet);

}
void ClientDatabase::SetPasswordProtected(bool _protected)
{
	if (!m_client.IsConnected())
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_SYSTEM, "Tried to send \"IS_PASSWORD_PROTECTED\", but is not connected to MasterServer");
		return;
	}

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("IS_PASSWORD_PROTECTED");
	ph->WriteByte(id, _protected);
	auto packet = ph->EndPack(id);

	m_client.Send(packet);
}
void ClientDatabase::SetServerPort(int _port)
{
	if (!m_client.IsConnected())
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_SYSTEM, "Tried to send \"SET_SERVER_PORT\", but is not connected to MasterServer");
		return;
	}

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("SET_SERVER_PORT");
	ph->WriteInt(id, _port);
	auto packet = ph->EndPack(id);

	m_client.Send(packet);
}

void ClientDatabase::IncreaseMaxNoPlayers()
{
	if (!m_client.IsConnected())
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_SYSTEM, "Tried to send \"MAX_PLAYER_COUNT_INCREASED\", but is not connected to MasterServer");
		return;
	}

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("MAX_PLAYER_COUNT_INCREASED");
	auto packet = ph->EndPack(id);

	m_client.Send(packet);

}

void ClientDatabase::IncreaseNoPlayers()
{
	if (!m_client.IsConnected())
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_SYSTEM, "Tried to send \"PLAYER_COUNT_INCREASED\", but is not connected to MasterServer");
		return;
	}

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("PLAYER_COUNT_INCREASED");
	auto packet = ph->EndPack(id);

	m_client.Send(packet);

}

void ClientDatabase::DecreaseNoPlayers()
{
	if (!m_client.IsConnected())
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_SYSTEM, "Tried to send \"PLAYER_COUNT_DECREASED\", but is not connected to MasterServer");
		return;
	}

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("PLAYER_COUNT_DECREASED");
	auto packet = ph->EndPack(id);

	m_client.Send(packet);

}

void ClientDatabase::IncreaseNoSpectators()
{
	if (!m_client.IsConnected())
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_SYSTEM, "Tried to send \"SPECTATOR_COUNT_INCREASED\", but is not connected to MasterServer");
		return;
	}

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("SPECTATOR_COUNT_INCREASED");
	auto packet = ph->EndPack(id);

	m_client.Send(packet);

}

void ClientDatabase::DecreaseNoSpectators()
{
	if (!m_client.IsConnected())
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_SYSTEM, "Tried to send \"SPECTATOR_COUNT_DECREASED\", but is not connected to MasterServer");
		return;
	}

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("SPECTATOR_COUNT_DECREASED");
	auto packet = ph->EndPack(id);

	m_client.Send(packet);

}