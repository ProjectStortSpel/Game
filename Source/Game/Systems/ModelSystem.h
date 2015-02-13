#ifndef MODELSYSTEM_H
#define MODELSYSTEM_H

#include "GraphicalSystem.h"
#if defined(__ANDROID__) || defined(__IOS__)
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif


class ModelSystem : public GraphicalSystem
{
public:
	ModelSystem(Renderer::GraphicDevice* _graphics);
	~ModelSystem();

	void Initialize();

	unsigned int m_colorId;

	void EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);

};

#endif