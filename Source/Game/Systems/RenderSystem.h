#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "GraphicalSystem.h"
#if defined(__ANDROID__) || defined(__IOS__)
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif


class RenderSystem : public GraphicalSystem
{
public:
	RenderSystem(Renderer::GraphicDevice* _graphics);
	~RenderSystem();

	void Initialize();
	void Update(const ECSL::RuntimeInfo& _runtime);

	void EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);
	void EntitiesRemoved(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);

private:
	void UpdateMatrix(unsigned int _entityId);
	ECSL::BitSet::DataType* m_bitMask;
	unsigned int m_numberOfBitSets;
	unsigned int m_componentId;

	unsigned int m_positionId, m_rotationId;
	unsigned int m_scaleId;
	unsigned int m_colorId;
	unsigned int m_renderId, m_renderModel, m_renderOffset, m_colorOffset;
	unsigned int m_worldToViewSpaceId;
	unsigned int m_staticModelId;

	unsigned int m_parentId;
	unsigned int m_parentColorId;
	unsigned int m_parentNoMatrixId;
	unsigned int m_parentJointId;
	unsigned int m_isparentId;

	std::map<unsigned int, std::vector<unsigned int>> m_Parents;
};

#endif