#ifndef RESETCHANGEDSYSTEM_H
#define RESETCHANGEDSYSTEM_H

#include "ECSL/Interfaces/System.h"
#include "ECSL/Framework/World.h"

class ResetChangedSystem : public ECSL::System
{
public:
	ResetChangedSystem();
	~ResetChangedSystem();

	void Initialize();
	void Update(const ECSL::RuntimeInfo& _runtime);

private:
	unsigned int m_componentId;
	unsigned int m_componentByteSize;
};

#endif