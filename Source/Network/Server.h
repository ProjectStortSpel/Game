#ifndef PROJEKTSERVER_H
#define PROJEKTSERVER_H

#include "Network/BaseNetwork.h"

class DECLSPEC Server : public BaseNetwork
{

public:
	Server();
	~Server();

	void Start();
	void Stop();

	void Broadcast(PacketHandler::Packet _packet, NetConnection *_exclude = NULL);
	void Send(PacketHandler::Packet _packet, NetConnection _connection);

	void KickClient(NetConnection* _connection, const char* _reason);
	void BanClient(NetConnection* _connection, const char* _reason, unsigned int _milliseconds = 0);
	void UnbanClient(NetConnection* _connection);

private:
	void ReceivePackets(void);

private:
	int m_maxConnections;


};

#endif