#include "ClientDatabase.h"
#include "Game/Network/NetworkInstance.h"
#include "Logger/Managers/Logger.h"

ClientDatabase& ClientDatabase::GetInstance()
{
	static ClientDatabase* instance = new ClientDatabase();
	return *instance;
}

ClientDatabase::ClientDatabase()
	:m_ipAddress("194.47.150.44"), m_password("DefaultMasterPassword"), m_remotePort(5509), m_localPort(0), m_connected(false), m_tryConnect(true)
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
	if (!m_tryConnect)
		return false;

	if (!m_connected)
		m_connected = m_client.Connect(m_ipAddress.c_str(), m_password.c_str(), m_remotePort, m_localPort);
		m_tryConnect = m_connected;

	return m_connected;
}
bool ClientDatabase::Disconnect()
{
	if(m_connected)
		m_client.Disconnect();
	
	m_connected = false;

	return true;
}

void ClientDatabase::ResetNetworkEvents()
{
	m_client.ResetNetworkEvents();
}

void ClientDatabase::Update(float dt)
{
	while (m_client.PopAndExecutePacket() > 0) {}
	m_client.Update(dt);
}

void ClientDatabase::AddToDatabase(int _port, bool _pwProtected)
{
	if (!m_connected)
	{
		Logger::GetInstance().Log("MasterServer", Info, "Tried to send \"ADD_TO_DATABASE\", but is not connected to MasterServer");
		return;
	}

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("ADD_TO_DATABASE");
	ph->WriteInt(id, _port);
	ph->WriteByte(id, _pwProtected);
	auto packet = ph->EndPack(id);
	m_client.Send(packet);

	//m_client.Disconnect();

}

void ClientDatabase::RemoveFromDatabase()
{
	if (!m_connected)
	{
		Logger::GetInstance().Log("MasterServer", Info, "Tried to send \"REMOVE_FROM_DATABASE\", but is not connected to MasterServer");
		return;
	}

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("REMOVE_FROM_DATABASE");
	auto packet = ph->EndPack(id);
	m_client.Send(packet);
}

void ClientDatabase::SetGameStarted(bool _started)
{
	if (!m_connected)
	{
		Logger::GetInstance().Log("MasterServer", Info, "Tried to send \"GAME_STARTED\", but is not connected to MasterServer");
		return;
	}

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("GAME_STARTED");
	ph->WriteByte(id, _started);
	auto packet = ph->EndPack(id);
	m_client.Send(packet);

	//m_client.Disconnect();

}
void ClientDatabase::SetPasswordProtected(bool _protected)
{
	if (!m_connected)
	{
		Logger::GetInstance().Log("MasterServer", Info, "Tried to send \"IS_PASSWORD_PROTECTED\", but is not connected to MasterServer");
		return;
	}

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("IS_PASSWORD_PROTECTED");
	ph->WriteByte(id, _protected);
	auto packet = ph->EndPack(id);
	m_client.Send(packet);

	//m_client.Disconnect();
}
void ClientDatabase::SetServerPort(int _port)
{
	if (!m_connected)
	{
		Logger::GetInstance().Log("MasterServer", Info, "Tried to send \"SET_SERVER_PORT\", but is not connected to MasterServer");
		return;
	}

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("SET_SERVER_PORT");
	ph->WriteInt(id, _port);
	auto packet = ph->EndPack(id);
	m_client.Send(packet);

	//m_client.Disconnect();
}

void ClientDatabase::IncreaseMaxNoPlayers()
{
	if (!m_connected)
	{
		Logger::GetInstance().Log("MasterServer", Info, "Tried to send \"MAX_PLAYER_COUNT_INCREASED\", but is not connected to MasterServer");
		return;
	}

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("MAX_PLAYER_COUNT_INCREASED");
	auto packet = ph->EndPack(id);
	m_client.Send(packet);

	//m_client.Disconnect();

}

void ClientDatabase::IncreaseNoPlayers()
{
	if (!m_connected)
	{
		Logger::GetInstance().Log("MasterServer", Info, "Tried to send \"PLAYER_COUNT_INCREASED\", but is not connected to MasterServer");
		return;
	}

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("PLAYER_COUNT_INCREASED");
	auto packet = ph->EndPack(id);
	m_client.Send(packet);

	//m_client.Disconnect();

}

void ClientDatabase::DecreaseNoPlayers()
{
	if (!m_connected)
	{
		Logger::GetInstance().Log("MasterServer", Info, "Tried to send \"PLAYER_COUNT_DECREASED\", but is not connected to MasterServer");
		return;
	}

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("PLAYER_COUNT_DECREASED");
	auto packet = ph->EndPack(id);
	m_client.Send(packet);

	//m_client.Disconnect();

}

void ClientDatabase::IncreaseNoSpectators()
{
	if (!m_connected)
	{
		Logger::GetInstance().Log("MasterServer", Info, "Tried to send \"SPECTATOR_COUNT_INCREASED\", but is not connected to MasterServer");
		return;
	}

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("SPECTATOR_COUNT_INCREASED");
	auto packet = ph->EndPack(id);
	m_client.Send(packet);

	//m_client.Disconnect();

}

void ClientDatabase::DecreaseNoSpectators()
{
	if (!m_connected)
	{
		Logger::GetInstance().Log("MasterServer", Info, "Tried to send \"SPECTATOR_COUNT_DECREASED\", but is not connected to MasterServer");
		return;
	}

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("SPECTATOR_COUNT_DECREASED");
	auto packet = ph->EndPack(id);
	m_client.Send(packet);

	//m_client.Disconnect();

}

void ClientDatabase::RequestServerList()
{
	if (!m_connected)
	{
		Logger::GetInstance().Log("MasterServer", Info, "Tried to send \"GET_SERVER_LIST\", but is not connected to MasterServer");
		return;
	}

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("GET_SERVER_LIST");
	auto packet = ph->EndPack(id);
	m_client.Send(packet);

	//m_client.Disconnect();
}

void ClientDatabase::PingServer()
{
	if (!m_connected)
	{
		Logger::GetInstance().Log("MasterServer", Info, "Tried to send \"PING_SERVER\", but is not connected to MasterServer");
		return;
	}

	auto ph = m_client.GetPacketHandler();
	auto id = ph->StartPack("PING_SERVER");
	auto packet = ph->EndPack(id);
	m_client.Send(packet);

}

void ClientDatabase::HookOnGetServerList(Network::NetMessageHook& _hook)
{
	m_client.AddNetworkHook("GET_SERVER_LIST", _hook);
}

void ClientDatabase::HookOnConnectionAccepted(Network::NetEvent _hook)
{
	m_client.SetOnConnectedToServer(_hook);
}