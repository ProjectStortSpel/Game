#include "RotationSystem.h"

RotationSystem::RotationSystem()
{

}
RotationSystem::~RotationSystem()
{

}

void RotationSystem::Initialize()
{
	/*	Rendersystem wants Position, Scale, Rotation and Render	*/
	AddComponentTypeToFilter("Spin", ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Rotation", ECSL::FilterType::Mandatory);


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
		Spin = (float*)GetComponent(entity, "Spin", 0);
		Rotation = (float*)GetComponent(entity, "Rotation", 0);

		for (int n = 0; n < 3; ++n)
			Rotation[n] += Spin[n] * _dt;
	}

}

void RotationSystem::OnEntityAdded(unsigned int _entityId)
{
}

void RotationSystem::OnEntityRemoved(unsigned int _entityId)
{
}