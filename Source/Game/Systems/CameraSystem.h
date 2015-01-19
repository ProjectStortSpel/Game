#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include "ECSL/Interfaces/System.h"
#ifdef __ANDROID__
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

	void Update(float _dt);
	void Initialize();

	void OnEntityAdded(unsigned int _entityId);
	void OnEntityRemoved(unsigned int _entityId);

private:
	Renderer::GraphicDevice* m_graphics;
	Input::InputWrapper* m_input;
};

#endif