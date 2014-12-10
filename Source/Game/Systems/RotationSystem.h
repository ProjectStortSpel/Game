#ifndef ROTATIONSYSTEM_H
#define ROTATIONSYSTEM_H

#include "ECSL/Interfaces/System.h"
#include "Renderer/GraphicDevice.h"
#include "Input/InputWrapper.h"


class RotationSystem : public ECSL::System
{
public:
	RotationSystem();
	~RotationSystem();

	void Update(float _dt);
	void Initialize();

	void OnEntityAdded(unsigned int _entityId);
	void OnEntityRemoved(unsigned int _entityId);

private:
};

#endif