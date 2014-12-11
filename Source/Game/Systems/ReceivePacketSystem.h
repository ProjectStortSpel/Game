#ifndef RECEIVEPACKETSYSTEM_H
#define RECEIVEPACKETSYSTEM_H

#include "ECSL/Interfaces/System.h"

class ReceivePacketSystem : public ECSL::System
{
public:
	ReceivePacketSystem();
	~ReceivePacketSystem();

	void Update(float _dt);
	void Initialize();

	void OnEntityAdded(unsigned int _entityId);
	void OnEntityRemoved(unsigned int _entityId);

private:

};

#endif