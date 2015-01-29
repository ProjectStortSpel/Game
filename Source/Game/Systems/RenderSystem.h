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

	void Update(float _dt);
	void Initialize();

	void OnEntityAdded(unsigned int _entityId);
	void OnEntityRemoved(unsigned int _entityId);

private:
	void UpdateMatrix(unsigned int _entityId);
	ECSL::BitSet::DataType* m_bitMask;
	unsigned int m_numberOfBitSets;
	unsigned int m_componentId;

	unsigned int m_positionId, m_rotationId;
	unsigned int m_scaleId;
	unsigned int m_renderId, m_renderOffset;

	unsigned int m_parentId;
	unsigned int m_isparentId;

	Renderer::GraphicDevice* m_graphics;
};

#endif