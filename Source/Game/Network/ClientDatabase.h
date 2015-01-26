#ifndef CLIENTDATABASE_H
#define CLIENTDATABASE_H

#include "Network/ClientNetwork.h"
#include <ctime>

class ClientDatabase
{

	struct ServerInfo
	{
		int TimeId;
		std::string Name;
		std::string IpAddress;
		short Port;
		short NoUsers;
		short NoSpectators;
		short MaxUsers;
		bool GameStarted;
		bool PasswordProtected;

		ServerInfo()
		{
			TimeId = (int)std::time(0);
			Name = "UntitledServer";
			IpAddress = "127.0.0.1";
			Port = 0;
			NoUsers = 0;
			NoSpectators = 0;
			MaxUsers = 0;
			GameStarted = false;
			PasswordProtected = false;
		}

	};

public:
	~ClientDatabase();
	ClientDatabase();

	bool Connect();
	bool Disconnect();

	void Update(float dt);

	void AddToDatabase();
	void SetGameStarted(bool _started);
	void SetPasswordProtected(bool _protected);
	void SetServerPort(int _port);

	void IncreaseNoPlayers();
	void DecreaseNoPlayers();
	void IncreaseNoSpectators();
	void DecreaseNoSpectators();

	void IncreaseMaxNoPlayers();

	void RequestServerList();
	std::vector<ServerInfo>& GetServerList() { return m_serverList; }

private:
	
	void OnGetServerList(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);

private:
	std::vector<ServerInfo> m_serverList;

	Network::ClientNetwork m_client;

	std::string m_ipAddress;
	std::string m_password;
	int m_remotePort;
	int m_localPort;

};


#endif