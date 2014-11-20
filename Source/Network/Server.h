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

	void Broadcast(PacketHandler::Packet _packet);

private:
	void ReceivePackets(void);

private:

};

#endif