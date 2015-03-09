#ifndef GENERATEISLANDSYSTEM_H
#define GENERATEISLANDSYSTEM_H

#include "GraphicalSystem.h"
#if defined(__ANDROID__) || defined(__IOS__)
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif

class GenerateIslandSystem : public GraphicalSystem
{
public:
	GenerateIslandSystem(Renderer::GraphicDevice* _graphics, bool _isClient);

	void Initialize();

	void EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);

private:
	bool m_isClient;

	unsigned int m_generateIslandId;
	unsigned int m_generateIslandMapId;
	unsigned int m_generateIslandSizeXId;
	unsigned int m_generateIslandSizeZId;

	unsigned int m_renderId;
};

#endif