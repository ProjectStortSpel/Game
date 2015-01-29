#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include "ECSL/Interfaces/System.h"
#if defined(__ANDROID__) || defined(__IOS__)
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif
#include "Input/InputWrapper.h"


class CameraSystem : public ECSL::System
{
public:
	CameraSystem(Renderer::GraphicDevice* _graphics);
	~CameraSystem();

	void Initialize();
	void Update(const ECSL::RuntimeInfo& _runtime);

private:
	Renderer::GraphicDevice* m_graphics;
	Input::InputWrapper* m_input;
};

#endif