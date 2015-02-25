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

	void Initialize();
	void Update(const ECSL::RuntimeInfo& _runtime);

	void EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);
	void EntitiesRemoved(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);

private:

	unsigned int m_componentId;
	unsigned int m_numberOfBitSets;
};

#endif