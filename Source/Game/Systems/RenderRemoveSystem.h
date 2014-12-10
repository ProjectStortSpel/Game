#ifndef RENDERREMOVESYSTEM_H
#define RENDERREMOVESYSTEM_H

#include "ECSL/Interfaces/System.h"
#include "Renderer/GraphicDevice.h"


class RenderRemoveSystem : public ECSL::System
{
public:
	RenderRemoveSystem(Renderer::GraphicDevice* _graphics);
	~RenderRemoveSystem();

	void Update(float _dt);
	void Initialize();

	void OnEntityAdded(unsigned int _entityId);
	void OnEntityRemoved(unsigned int _entityId);

private:
	void UpdateMatrix(unsigned int _entityId);

	Renderer::GraphicDevice* m_graphics;
};

#endif