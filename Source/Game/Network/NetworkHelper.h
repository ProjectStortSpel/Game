#ifndef NETWORKHELPER_H
#define NETWORKHELPER_H

#include "Network/PacketHandler.h"
#include "ECSL/Framework/World.h"

class NetworkHelper
{
private:

public:

	static void WriteEntity(Network::PacketHandler* _ph, uint64_t _id, ECSL::World* _world, unsigned int _e);
	static void ReadEntity(Network::PacketHandler* _ph, uint64_t _id, ECSL::World* _world, unsigned int _e);


};


#endif