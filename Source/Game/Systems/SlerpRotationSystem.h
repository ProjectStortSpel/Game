#ifndef SLERPROTATIONSYSTEM_H
#define SLERPROTATIONSYSTEM_H

#include "ECSL/Interfaces/System.h"
#ifdef __ANDROID__
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

	void OnEntityAdded(unsigned int _entityId);
	void OnEntityRemoved(unsigned int _entityId);
};

#endif