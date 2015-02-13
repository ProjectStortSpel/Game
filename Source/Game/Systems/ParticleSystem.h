#ifndef GAME_PARTICLESYSTEM_H
#define GAME_PARTICLESYSTEM_H

#include "GraphicalSystem.h"
#if defined(__ANDROID__) || defined(__IOS__)
#include "AndroidRenderer/GraphicDevice.h"
#else
#include "Renderer/GraphicDevice.h"
#endif


class ParticleSystem : public GraphicalSystem
{
public:
	ParticleSystem(Renderer::GraphicDevice* _graphics);
	~ParticleSystem();

	void Initialize();
	void Update(const ECSL::RuntimeInfo& _runtime);

	void EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);
	void EntitiesRemoved(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);

private:
	void UpdateParticleSystem(unsigned int _entityId);
	ECSL::BitSet::DataType* m_bitMask;
	unsigned int m_numberOfBitSets;
	unsigned int m_componentId;

	unsigned int m_positionId, m_colorId;
	unsigned int m_particleId;

};

#endif