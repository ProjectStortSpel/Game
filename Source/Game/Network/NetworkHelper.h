#ifndef NETWORKHELPER_H
#define NETWORKHELPER_H

#include "Network/PacketHandler.h"
#include "ECSL/Framework/World.h"
#include <vector>

class NetworkHelper
{
private:

	ECSL::World** m_world;

	std::map<unsigned int, unsigned int> m_NtoH;
	std::map<unsigned int, unsigned int> m_HtoN;

	void WriteComponents(Network::PacketHandler* _ph, uint64_t _id, unsigned int _e, std::vector<unsigned int>& _components, bool _data = true);

	int	 StartReceiveEntity(Network::PacketHandler* _ph, uint64_t _id, std::vector<unsigned int>& _components);
	void ReceiveComponents(Network::PacketHandler* _ph, uint64_t _id, unsigned int _e, std::vector<unsigned int>& _components, bool _data = true);
	void EndReceiveEntity(unsigned int _e, std::vector<unsigned int>& _components);

public:

	NetworkHelper(ECSL::World** _world);
	~NetworkHelper();

	void ResetNetworkMaps();

	Network::Packet* WriteEntityAll(Network::PacketHandler* _ph, unsigned int _e);
	Network::Packet* WriteEntityDelta(Network::PacketHandler* _ph, unsigned int _e, std::vector<unsigned int>& _componentsChanged);
	Network::Packet* WriteEntityKill(Network::PacketHandler* _ph, unsigned int _e);

	void ReceiveEntityAll(Network::PacketHandler* _ph, uint64_t _id, Network::NetConnection _nc);
	void ReceiveEntityDelta(Network::PacketHandler* _ph, uint64_t _id, Network::NetConnection _nc);

	void ReceiveEntityKill(Network::PacketHandler* _ph, uint64_t _id, Network::NetConnection _nc);

	unsigned int NetToHost(unsigned int _idN);
	unsigned int HostToNet(unsigned int _idH);

};


#endif