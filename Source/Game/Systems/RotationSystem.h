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

	void Initialize();
	void Update(const ECSL::RuntimeInfo& _runtime);

private:
	unsigned int m_rotationId, m_spinId;
};

#endif