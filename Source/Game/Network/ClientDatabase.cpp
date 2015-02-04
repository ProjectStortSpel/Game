#include "ClientDatabase.h"
#include "Game/Network/NetworkInstance.h"
#include "Logger/Managers/Logger.h"

ClientDatabase& ClientDatabase::GetInstance()
{
	static ClientDatabase* instance = new ClientDatabase();
	return *instance;
}

ClientDatabase::ClientDatabase()
	:m_ipAddress("127.0.0.1"), m_password("DefaultMasterPassword"), m_remotePort(5509), m_localPort(0), m_connected(false), m_maxTries(0)
{
	Logger::GetInstance().AddGroup("MasterServer", false);

	m_client.SetMaxTimeOutIntervall(60);
	m_client.SetMaxTimeOutCounter(2);
	m_client.SetRemoteAddress(m_ipAddress.c_str());
	m_client.SetServerPassword(m_password.c_str());
	m_client.SetOutgoingPort(m_remotePort);
	m_client.SetIncomingPort(m_localPort);

    Network::NetEvent event = std::bind(&ClientDatabase::OnDisconnected, this, std::placeholders::_1, std::placeholders::_2);
    
    m_client.SetOnDisconnectedFromServer(event);
    m_client.SetOnTimedOutFromServer(event);


}
ClientDatabase::~ClientDatabase()
{
	Disconnect();
}

bool ClientDatabase::Connect()
{
	if (m_maxTries > 4)
		return false;

	if (!m_connected)
		m_connected = m_client.Connect(m_ipAddress.c_str(), m_password.c_str(), m_remotePort, m_localPort);

	if (!m_connected)
		++m_maxTries;

	return m_connected;
}
bool ClientDatabase::Disconnect()
{
	if(m_connected)
		m_client.Disconnect();

	return true;
}

void ClientDatabase::Update(float dt)
{
	while (m_client.PopAndExecutePacket() > 0) {}
	m_client.Update(dt);
}

void ClientDatabase::AddToDatabase(int _port, bool _pwProtected)
{
	//m_client.SetIncomingPort(_port + 1);
	//m_client.Connect();

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("ADD_TO_DATABASE");
	ph->WriteInt(id, _port);
	ph->WriteByte(id, _pwProtected);
	auto packet = ph->EndPack(id);
	m_client.Send(packet);

	//m_client.Disconnect();

}

void ClientDatabase::SetGameStarted(bool _started)
{
	//m_client.Connect();

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("GAME_STARTED");
	ph->WriteByte(id, _started);
	auto packet = ph->EndPack(id);
	m_client.Send(packet);

	//m_client.Disconnect();

}
void ClientDatabase::SetPasswordProtected(bool _protected)
{
	//m_client.Connect();

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("IS_PASSWORD_PROTECTED");
	ph->WriteByte(id, _protected);
	auto packet = ph->EndPack(id);
	m_client.Send(packet);

	//m_client.Disconnect();
}
void ClientDatabase::SetServerPort(int _port)
{
	//m_client.Connect();

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("SET_SERVER_PORT");
	ph->WriteInt(id, _port);
	auto packet = ph->EndPack(id);
	m_client.Send(packet);

	//m_client.Disconnect();
}

void ClientDatabase::IncreaseMaxNoPlayers()
{
	//m_client.Connect();

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("MAX_PLAYER_COUNT_INCREASED");
	auto packet = ph->EndPack(id);
	m_client.Send(packet);

	//m_client.Disconnect();

}

void ClientDatabase::IncreaseNoPlayers()
{
	//m_client.Connect();

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("PLAYER_COUNT_INCREASED");
	auto packet = ph->EndPack(id);
	m_client.Send(packet);

	//m_client.Disconnect();

}

void ClientDatabase::DecreaseNoPlayers()
{
	//m_client.Connect();

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("PLAYER_COUNT_DECREASED");
	auto packet = ph->EndPack(id);
	m_client.Send(packet);

	//m_client.Disconnect();

}

void ClientDatabase::IncreaseNoSpectators()
{
	//m_client.Connect();

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("SPECTATOR_COUNT_INCREASED");
	auto packet = ph->EndPack(id);
	m_client.Send(packet);

	//m_client.Disconnect();

}

void ClientDatabase::DecreaseNoSpectators()
{
	//m_client.Connect();

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("SPECTATOR_COUNT_DECREASED");
	auto packet = ph->EndPack(id);
	m_client.Send(packet);

	//m_client.Disconnect();

}

void ClientDatabase::RequestServerList()
{
	//m_client.Connect();

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("GET_SERVER_LIST");
	auto packet = ph->EndPack(id);
	m_client.Send(packet);

	//m_client.Disconnect();
}

void ClientDatabase::HookOnGetServerList(Network::NetMessageHook& _hook)
{
	m_client.AddNetworkHook("GET_SERVER_LIST", _hook);
}