#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "ECSL/Interfaces/System.h"
#ifdef __ANDROID__
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif


class RenderSystem : public ECSL::System
{
public:
	RenderSystem(Renderer::GraphicDevice* _graphics);
	~RenderSystem();

	void Update(const ECSL::RuntimeInfo& _runtime);
	void Initialize();

	void EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);
	void EntitiesRemoved(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);

private:
	void UpdateMatrix(unsigned int _entityId);

	ECSL::BitSet::DataType* m_bitMask;
	unsigned int m_numberOfBitSets;
	unsigned int m_componentId;

	unsigned int m_positionId, m_rotationId;
	unsigned int m_scaleId;
	unsigned int m_renderId, m_renderOffset;

	Renderer::GraphicDevice* m_graphics;
};

#endif