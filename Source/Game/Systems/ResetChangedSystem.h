#ifndef RESETCHANGEDSYSTEM_H
#define RESETCHANGEDSYSTEM_H

#include "ECSL/Interfaces/System.h"
#include "ECSL/Framework/World.h"

class ResetChangedSystem : public ECSL::System
{
public:
	ResetChangedSystem();
	~ResetChangedSystem();

	void Update(float _dt);
	void Initialize();

	void OnEntityAdded(unsigned int _entityId);
	void OnEntityRemoved(unsigned int _entityId);

private:

};

#endif