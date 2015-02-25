#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include "GraphicalSystem.h"
#if defined(__ANDROID__) || defined(__IOS__)
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif
#include "Input/InputWrapper.h"


class CameraSystem : public GraphicalSystem
{
public:
	CameraSystem(Renderer::GraphicDevice* _graphics);
	~CameraSystem();

	void Initialize();
	void Update(const ECSL::RuntimeInfo& _runtime);
private:
	Input::InputWrapper* m_input;
};

#endif