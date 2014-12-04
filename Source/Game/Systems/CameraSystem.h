#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include "ECSL/Interfaces/System.h"
#include "Renderer/GraphicDevice.h"


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
};

#endif