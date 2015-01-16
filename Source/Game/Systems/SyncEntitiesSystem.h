#ifndef SYNCENTITIESSYSTEM_H
#define SYNCENTITIESSYSTEM_H

#include "ECSL/Interfaces/System.h"
#include "ECSL/Framework/World.h"

class SyncEntitiesSystem : public ECSL::System
{
	ECSL::World* m_world;
	float m_timer;

public:
	SyncEntitiesSystem();
	~SyncEntitiesSystem();

	void Update(float _dt);
	void Initialize();

	void OnEntityAdded(unsigned int _entityId);
	void OnEntityRemoved(unsigned int _entityId);

private:

	unsigned int m_componentId;
	unsigned int m_numberOfBitSets;
};

#endif