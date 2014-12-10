#ifndef NETWORKHELPER_H
#define NETWORKHELPER_H

#include "Network/PacketHandler.h"
#include "ECSL/Framework/World.h"
#include <vector>

class NetworkHelper
{
private:

	ECSL::World* m_world;

	std::map<unsigned int, unsigned int> m_NtoH;
	std::map<unsigned int, unsigned int> m_HtoN;

public:

	NetworkHelper(ECSL::World* _world);
	~NetworkHelper();


	Network::Packet* WriteEntity(Network::PacketHandler* _ph, unsigned int _e);
	void ReceiveEntity(Network::PacketHandler* _ph, uint64_t _id, Network::NetConnection _nc);
	void ReceiveEntityDelta(Network::PacketHandler* _ph, uint64_t _id, Network::NetConnection _nc);
	void ReceiveEntityKill(Network::PacketHandler* _ph, uint64_t _id, Network::NetConnection _nc);


};


#endif