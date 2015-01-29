#ifndef DIRECTIONALLIGHTSYTEM_H
#define DIRECTIONALLIGHTSYTEM_H

#include "ECSL/Interfaces/System.h"
#if defined(__ANDROID__) || defined(__IOS__)
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif


class DirectionalLightSystem : public ECSL::System
{
public:
	DirectionalLightSystem(Renderer::GraphicDevice* _graphics);
	~DirectionalLightSystem();
	void Initialize();

	void Update(const ECSL::RuntimeInfo& _runtime);

	void EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);
	void EntitiesRemoved(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);

private:
	void UpdateDirectionalLight();

	ECSL::BitSet::DataType* m_bitMask;
	unsigned int m_numberOfBitSets;
	unsigned int m_changedComponentId;
	unsigned int m_dirLightId;;

	Renderer::GraphicDevice* m_graphics;
};

#endif