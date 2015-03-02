#include "RotationSystem.h"

SpinSystem::SpinSystem()
{

}
SpinSystem::~SpinSystem()
{

}

void SpinSystem::Initialize()
{
	SetSystemName("Spin System");

	SetUpdateTaskCount(1);

	/*	Rendersystem wants Position, Scale, Rotation and Render	*/
	AddComponentTypeToFilter("Spin", ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Rotation", ECSL::FilterType::Mandatory);

	m_rotationId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Rotation");
	m_spinId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Spin");

}

void SpinSystem::Update(const ECSL::RuntimeInfo& _runtime)
{
   // return;
	auto entities = *GetEntities();

	for (auto entity : entities)
	{
		float*	Spin;
		float*	Rotation;
		Spin = (float*)GetComponent(entity, m_spinId, 0);
		Rotation = (float*)GetComponent(entity, m_rotationId, 0);

		for (int n = 0; n < 3; ++n)
			Rotation[n] += Spin[n] * _runtime.Dt;

		ComponentHasChanged(entity, m_rotationId, false);
	}
}