#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "ECSL/Interfaces/System.h"
#include "Renderer/GraphicDevice.h"


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

	Renderer::GraphicDevice* m_graphics;
};

#endif