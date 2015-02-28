#ifndef SYNCENTITIESSYSTEM_H
#define SYNCENTITIESSYSTEM_H

#include "ECSL/Interfaces/System.h"
#include "ECSL/Framework/World.h"

class SyncEntitiesSystem : public ECSL::System
{
public:
	SyncEntitiesSystem();
	~SyncEntitiesSystem();

	void Initialize();
	void Update(const ECSL::RuntimeInfo& _runtime);

	void EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);
	void EntitiesRemoved(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);

private:
	float* m_timer;
	unsigned int m_componentId;
	unsigned int m_numberOfBitSets;
};

#endif