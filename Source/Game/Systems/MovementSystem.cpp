#include "MovementSystem.h"

MovementSystem::MovementSystem()
{

}
MovementSystem::~MovementSystem()
{

}

void MovementSystem::Initialize()
{
	/*	Rendersystem wants Position and Velocity	*/
	AddComponentTypeToFilter("Position", ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Velocity", ECSL::FilterType::Mandatory);


	printf("MovementSystem initialized!\n");
}

void MovementSystem::Update(float _dt)
{
	auto entities = *GetEntities();

	/*	TODO: Some logic to not update matrix every frame	*/
	for (auto entity : entities)
	{
		float*		Position;
		float*		Velocity;

		Position	=	(float*)GetComponent(entity, "Position", 0);
		Velocity	=	(float*)GetComponent(entity, "Velocity", 0);

		Position[0] += Velocity[0] * _dt;
		Position[1] += Velocity[1] * _dt;
		Position[2] += Velocity[2] * _dt;
	}

}

void MovementSystem::OnEntityAdded(unsigned int _entityId)
{

}

void MovementSystem::OnEntityRemoved(unsigned int _entityId)
{

}