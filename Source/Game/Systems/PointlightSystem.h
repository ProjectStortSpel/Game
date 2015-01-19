#ifndef POINTLIGHTSYSTEM_H
#define POINTLIGHTSYSTEM_H

#include "ECSL/Interfaces/System.h"
#ifdef __ANDROID__
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif


class PointlightSystem : public ECSL::System
{
public:
	PointlightSystem(Renderer::GraphicDevice* _graphics);
	~PointlightSystem();

	void Update(float _dt);
	void Initialize();

	void OnEntityAdded(unsigned int _entityId);
	void OnEntityRemoved(unsigned int _entityId);

private:
	void UpdatePointLights();

	ECSL::BitSet::DataType* m_bitMask;
	unsigned int m_numberOfBitSets;
	unsigned int m_changedComponentId;
	unsigned int m_pointLightId;;

	Renderer::GraphicDevice* m_graphics;
};

#endif