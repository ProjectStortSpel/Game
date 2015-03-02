#ifndef SPINSYSTEM_H
#define SPINSYSTEM_H

#include "ECSL/Interfaces/System.h"
#include "ECSL/Framework/World.h"

class SpinSystem : public ECSL::System
{
public:
	SpinSystem();
	~SpinSystem();

	void Initialize();
	void Update(const ECSL::RuntimeInfo& _runtime);

private:
	unsigned int m_spinId;
	unsigned int m_rotationId;
};

#endif