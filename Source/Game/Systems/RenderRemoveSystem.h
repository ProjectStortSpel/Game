#ifndef RENDERREMOVESYSTEM_H
#define RENDERREMOVESYSTEM_H

#include "ECSL/Interfaces/System.h"
#ifdef __ANDROID__
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif


class RenderRemoveSystem : public ECSL::System
{
public:
	RenderRemoveSystem(Renderer::GraphicDevice* _graphics);
	~RenderRemoveSystem();

	void Initialize();

	void EntitiesRemoved(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);

private:
	void UpdateMatrix(unsigned int _entityId);

	Renderer::GraphicDevice* m_graphics;
};

#endif