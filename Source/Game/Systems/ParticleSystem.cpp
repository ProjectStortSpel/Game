#include "ParticleSystem.h"

#include "ECSL/Framework/Common/BitSet.h"

ParticleSystem::ParticleSystem(Renderer::GraphicDevice* _graphics)
{
	m_graphics = _graphics;

}
ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::Initialize()
{
	SetSystemName("ParticleSystem");

	SetUpdateTaskCount(1);
	SetEntitiesAddedTaskCount(1);
	SetEntitiesRemovedTaskCount(1);

	/*	Rendersystem wants Position, Scale, Rotation and Render	*/
	AddComponentTypeToFilter("Position", ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Color", ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Particle", ECSL::FilterType::Mandatory);
	std::vector<unsigned int> bitsetComponents;
	bitsetComponents.push_back(ECSL::ComponentTypeManager::GetInstance().GetTableId("Position"));
	bitsetComponents.push_back(ECSL::ComponentTypeManager::GetInstance().GetTableId("Color"));
	bitsetComponents.push_back(ECSL::ComponentTypeManager::GetInstance().GetTableId("Particle"));

	m_bitMask = ECSL::BitSet::BitSetConverter::ArrayToBitSet(bitsetComponents, ECSL::ComponentTypeManager::GetInstance().GetComponentTypeCount());
	m_numberOfBitSets = ECSL::BitSet::GetDataTypeCount(ECSL::ComponentTypeManager::GetInstance().GetComponentTypeCount());
	m_componentId = ECSL::ComponentTypeManager::GetInstance().GetTableId("ChangedComponents");

	m_positionId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Position");
	m_colorId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Color");
	m_particleId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Particle");
}

void ParticleSystem::Update(const ECSL::RuntimeInfo& _runtime)
{
	auto entities = *GetEntities();
	unsigned int startAt, endAt;
	MPL::MathHelper::SplitIterations(startAt, endAt, (unsigned int)entities.size(), _runtime.TaskIndex, _runtime.TaskCount);
	for (unsigned int i = startAt; i < endAt; ++i)
	{
		unsigned int entity = entities[i];
		ECSL::BitSet::DataType* eBitMask = (ECSL::BitSet::DataType*)GetComponent(entity, m_componentId, 0);

		bool needsUpdate = false;
		for (unsigned int n = 0; n < m_numberOfBitSets; ++n)
		{

			if ((m_bitMask[n] & eBitMask[n]) != 0)
			{
				needsUpdate = true;
				break;
			}

		}

		if (needsUpdate)
			UpdateParticleSystem(entity);
	}


}

void ParticleSystem::EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	for (auto entityId : _entities)
	{
		float*		Position;
		float*		Lifetime;
		float*		Color;
		float*		Scale;
		float*		SpriteSize;
		int*		Particles;
		int*		ID;
		std::string	Name;
		std::string	Texture;

		char* ParticleData;
		ParticleData	=	(char*)GetComponent(entityId, "Particle", "Name");
		Name			=	std::string(ParticleData);

		ParticleData	=	(char*)GetComponent(entityId, "Particle", "Texture");
		Texture			=	std::string(ParticleData);

		Position = (float*)GetComponent(entityId, "Position", "X");

		Color = (float*)GetComponent(entityId, "Color", "X");
		Lifetime = (float*)GetComponent(entityId, "Particle", "Lifetime");
		SpriteSize = (float*)GetComponent(entityId, "Particle", "SpriteSize");
		Particles = (int*)GetComponent(entityId, "Particle", "Particles");
		Scale = (float*)GetComponent(entityId, "Particle", "Scale");


		ID = (int*)GetComponent(entityId, "Particle", "Id");
#if  !defined(__ANDROID__) && !defined(__IOS__)
		m_graphics->AddParticleEffect(Name, glm::vec3(Position[0], Position[1], Position[2]), *Particles, *Lifetime, *Scale, *SpriteSize, Texture, glm::vec3(Color[0], Color[1], Color[2]), *ID);
#endif
	}
}

void ParticleSystem::EntitiesRemoved(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	for (auto entityId : _entities)
	{
		int*	ID	= (int*)GetComponent(entityId, "Particle", "Id");

#if  !defined(__ANDROID__) && !defined(__IOS__) 
		m_graphics->RemoveParticleEffect(*ID);
#endif
	}
}


void ParticleSystem::UpdateParticleSystem(unsigned int _entityId)
{

}