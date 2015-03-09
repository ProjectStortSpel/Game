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
	ModelSystem(Renderer::GraphicDevice* _graphics, bool _isClient);
	~ModelSystem();

	void Initialize();

	unsigned int m_colorId;
	unsigned int m_modelId;
	unsigned int m_renderId;

	unsigned int m_positionId;
	unsigned int m_rotationId;
	unsigned int m_scaleId;
	unsigned int m_staticModelId;

	void EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);

private:
	bool m_IsClient;
};

#endif