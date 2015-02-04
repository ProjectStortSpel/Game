#ifndef SLERPROTATIONSYSTEM_H
#define SLERPROTATIONSYSTEM_H

#include "ECSL/Interfaces/System.h"
#if defined(__ANDROID__) || defined(__IOS__)
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif
#include "Input/InputWrapper.h"
#include "Game/Quaternion.h"

class SlerpRotationSystem : public ECSL::System
{
public:
	SlerpRotationSystem();
	~SlerpRotationSystem();

	void Update(float _dt);
	void Initialize();

	void EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);

private:
	unsigned int m_rotationId;
};

#endif