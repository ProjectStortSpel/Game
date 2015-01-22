#ifndef CLIENTDATABASE_H
#define CLIENTDATABASE_H

#include "Network/ClientNetwork.h"

class ClientDatabase
{
public:
	~ClientDatabase();
	static ClientDatabase& GetInstance();

	bool Connect();
	bool Disconnect();

	void Update(float dt);

private:
	ClientDatabase();

	void OnPlayerConnected(Network::NetConnection& _nc, const char* _msg);
	void OnPlayerDisconnected(Network::NetConnection& _nc, const char* _msg);
	void OnPlayerTimedOut(Network::NetConnection& _nc, const char* _msg);

	Network::ClientNetwork m_client;

	std::string m_ipAddress;
	std::string m_password;
	int m_remotePort;
	int m_localPort;
};


#endif