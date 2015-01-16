#include "RotationSystem.h"

RotationSystem::RotationSystem()
{

}
RotationSystem::~RotationSystem()
{

}

void RotationSystem::Initialize()
{
	SetSystemName("Rotation Remove System");

	/*	Rendersystem wants Position, Scale, Rotation and Render	*/
	AddComponentTypeToFilter("Spin", ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Rotation", ECSL::FilterType::Mandatory);

	m_rotationId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Rotation");
	m_spinId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Spin");

	printf("RenderSystem initialized!\n");
}

void RotationSystem::Update(float _dt)
{
	auto entities = *GetEntities();

	/*	TODO: Some logic to not update matrix every frame	*/
	for (auto entity : entities)
	{
		float*	Spin;
		float*	Rotation;
		Spin = (float*)GetComponent(entity, m_spinId, 0);
		Rotation = (float*)GetComponent(entity, m_rotationId, 0);

		for (int n = 0; n < 3; ++n)
			Rotation[n] += Spin[n] * _dt;

		ComponentHasChanged(entity, m_rotationId);
	}

}

void RotationSystem::OnEntityAdded(unsigned int _entityId)
{
}

void RotationSystem::OnEntityRemoved(unsigned int _entityId)
{

}