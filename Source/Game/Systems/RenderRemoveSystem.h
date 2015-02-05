#ifndef RENDERREMOVESYSTEM_H
#define RENDERREMOVESYSTEM_H

#include "GraphicalSystem.h"
#if defined(__ANDROID__) || defined(__IOS__)
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif


class RenderRemoveSystem : public GraphicalSystem
{
public:
	RenderRemoveSystem(Renderer::GraphicDevice* _graphics);
	~RenderRemoveSystem();

	void Initialize();

	void EntitiesRemoved(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);

private:
	void UpdateMatrix(unsigned int _entityId);
};

#endif