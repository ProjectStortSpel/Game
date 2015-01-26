#ifndef CLIENTDATABASE_H
#define CLIENTDATABASE_H

#include "Network/ClientNetwork.h"

class ClientDatabase
{
public:
	~ClientDatabase();
	ClientDatabase();

	bool Connect();
	bool Disconnect();

	void Update(float dt);

	void SetGameStarted(bool _started);
	void SetPasswordProtected(bool _protected);
	void SetServerPort(int _port);

	void IncreaseNoPlayers();
	void DecreaseNoPlayers();
	void IncreaseNoSpectators();
	void DecreaseNoSpectators();

	void IncreaseMaxNoPlayers();

private:
	

	Network::ClientNetwork m_client;

	std::string m_ipAddress;
	std::string m_password;
	int m_remotePort;
	int m_localPort;

};


#endif