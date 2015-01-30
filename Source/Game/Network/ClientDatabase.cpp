#include "ClientDatabase.h"
#include "Game/NetworkInstance.h"
#include "Game/Logger/Logger.h"

ClientDatabase& ClientDatabase::GetInstance()
{
	static ClientDatabase* instance = new ClientDatabase();
	return *instance;
}

ClientDatabase::ClientDatabase()
	:m_ipAddress("194.47.150.44"), m_password("DefaultMasterPassword"), m_remotePort(5509), m_localPort(0), m_connected(false)
{
	Logger::GetInstance().AddGroup("MasterServer", false);

	m_client.SetMaxTimeOutIntervall(5);
	m_client.SetMaxTimeOutCounter(2);


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
	m_client.SetTimeOutValue(10000);

	if (!m_connected)
		m_connected = m_client.Connect(m_ipAddress.c_str(), m_password.c_str(), m_remotePort, m_localPort);

	m_client.SetTimeOutValue(0);

	return m_connected;
}
bool ClientDatabase::Disconnect()
{
	m_client.Disconnect();
	return true;
}

void ClientDatabase::Update(float dt)
{
	m_client.Update(dt);
	while (m_client.PopAndExecutePacket() > 0) {}
}

void ClientDatabase::AddToDatabase()
{
	if (!m_client.IsConnected())
	{
		Logger::GetInstance().Log("MasterServer", Info, "Tried to send \"ADD_TO_DATABASE\", but is not connected to MasterServer");
		return;
	}

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("ADD_TO_DATABASE");
	auto packet = ph->EndPack(id);

	m_client.Send(packet);

}

void ClientDatabase::SetGameStarted(bool _started)
{
	if (!m_client.IsConnected())
	{
		Logger::GetInstance().Log("MasterServer", Info, "Tried to send \"GAME_STARTED\", but is not connected to MasterServer");
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
		Logger::GetInstance().Log("MasterServer", Info, "Tried to send \"IS_PASSWORD_PROTECTED\", but is not connected to MasterServer");
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
		Logger::GetInstance().Log("MasterServer", Info, "Tried to send \"SET_SERVER_PORT\", but is not connected to MasterServer");
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
		Logger::GetInstance().Log("MasterServer", Info, "Tried to send \"MAX_PLAYER_COUNT_INCREASED\", but is not connected to MasterServer");
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
		Logger::GetInstance().Log("MasterServer", Info, "Tried to send \"PLAYER_COUNT_INCREASED\", but is not connected to MasterServer");
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
		Logger::GetInstance().Log("MasterServer", Info, "Tried to send \"PLAYER_COUNT_DECREASED\", but is not connected to MasterServer");
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
		Logger::GetInstance().Log("MasterServer", Info, "Tried to send \"SPECTATOR_COUNT_INCREASED\", but is not connected to MasterServer");
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
		Logger::GetInstance().Log("MasterServer", Info, "Tried to send \"SPECTATOR_COUNT_DECREASED\", but is not connected to MasterServer");
		return;
	}

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("SPECTATOR_COUNT_DECREASED");
	auto packet = ph->EndPack(id);

	m_client.Send(packet);

}

void ClientDatabase::RequestServerList()
{
	if (!m_client.IsConnected())
	{
		Logger::GetInstance().Log("MasterServer", Info, "Tried to send \"GET_SERVER_LIST\", but is not connected to MasterServer");
		return;
	}

	int hest = 0;
	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("GET_SERVER_LIST");
	auto packet = ph->EndPack(id);

	m_client.Send(packet);
}

void ClientDatabase::HookOnGetServerList(Network::NetMessageHook& _hook)
{
	m_client.AddNetworkHook("GET_SERVER_LIST", _hook);
	
//	SDL_Log("Server List:");
//	for (int i = 0; i < m_serverList.size(); ++i)
//	{
////		127.0.0.1:6112 0 / 5 (0) Running PasswordProtected
//			SDL_Log("%s:%i %i/%i (%i)", m_serverList[i].IpAddress.c_str(), m_serverList[i].Port,
//				m_serverList[i].NoUsers, m_serverList[i].MaxUsers, m_serverList[i].NoSpectators);
//	}

}