#ifndef ROTATIONSYSTEM_H
#define ROTATIONSYSTEM_H

#include "ECSL/Interfaces/System.h"
#if defined(__ANDROID__) || defined(__IOS__)
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif
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
	unsigned int m_rotationId, m_spinId;
};

#endif