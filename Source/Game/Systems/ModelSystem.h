#ifndef MODELSYSTEM_H
#define MODELSYSTEM_H

#include "ECSL/Interfaces/System.h"
#if defined(__ANDROID__) || defined(__IOS__)
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif


class ModelSystem : public ECSL::System
{
public:
	ModelSystem(Renderer::GraphicDevice* _graphics);
	~ModelSystem();

	void Initialize();

	void EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);

private:
	Renderer::GraphicDevice* m_graphics;
};

#endif