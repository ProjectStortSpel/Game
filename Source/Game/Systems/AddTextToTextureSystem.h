#ifndef ADDTEXTOTEXTURESYSTEM_H
#define ADDTEXTOTEXTURESYSTEM_H

#include "GraphicalSystem.h"
#if defined(__ANDROID__) || defined(__IOS__)
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif


class AddTextToTextureSystem : public GraphicalSystem
{
public:
	AddTextToTextureSystem(Renderer::GraphicDevice* _graphics);

	void Initialize();
	void PostInitialize();

	void EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);
};

#endif