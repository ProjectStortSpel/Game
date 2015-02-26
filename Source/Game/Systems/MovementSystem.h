#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "ECSL/Interfaces/System.h"


class MovementSystem : public ECSL::System
{
public:
	MovementSystem();
	~MovementSystem();

	void Update(float _dt);
	void Initialize();

	void OnEntityAdded(unsigned int _entityId);
	void OnEntityRemoved(unsigned int _entityId);

private:
};

#endif