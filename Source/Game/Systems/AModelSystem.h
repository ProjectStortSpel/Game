#ifndef AMODELSYSTEM_H
#define AMODELSYSTEM_H

#include "GraphicalSystem.h"
#if defined(__ANDROID__) || defined(__IOS__)
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif


class AModelSystem : public GraphicalSystem
{
public:
	AModelSystem(Renderer::GraphicDevice* _graphics, bool _isClient);
	~AModelSystem();

	void Initialize();

	void EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);

private:
	bool m_IsClient;

};

#endif