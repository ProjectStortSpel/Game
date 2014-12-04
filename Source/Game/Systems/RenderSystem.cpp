#include "RenderSystem.h"

RenderSystem::RenderSystem(Renderer::GraphicDevice* _graphics)
{
	m_graphics = _graphics;

}
RenderSystem::~RenderSystem()
{

}

void RenderSystem::Initialize()
{
	/*	Rendersystem wants Position, Scale, Rotation and Render	*/
	AddComponentTypeToFilter("Position",	ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Rotation",	ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Scale",		ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Render",		ECSL::FilterType::Mandatory);


	printf("RenderSystem initialized!\n");
}

void RenderSystem::Update(float _dt)
{
	auto entities = *GetEntities();

	/*	TODO: Some logic to not update matrix every frame	*/
	for (auto entity : entities)
		UpdateMatrix(entity);

}

void RenderSystem::OnEntityAdded(unsigned int _entityId)
{
	/*	Tell Graphics to enable model	*/
	//m_graphics->



	float*		Scale;
	
	glm::mat4*	Matrix;

	Scale		=	(float*)GetComponent(_entityId, "Scale", 0);
	Scale[0] = 1.0f;
	Scale[1] = 1.0f;
	Scale[2] = 1.0f;
	Matrix		=	(glm::mat4*)GetComponent(_entityId, "Render", "Mat");
	

	/*	Update the matrix	*/
	UpdateMatrix(_entityId);
}

void RenderSystem::OnEntityRemoved(unsigned int _entityId)
{
	/*	Tell Graphics to disable model	*/
	//m_graphics->
}


void RenderSystem::UpdateMatrix(unsigned int _entityId)
{
	float*		Position;
	float*		Rotation;
	float*		Scale;
	glm::mat4*	Matrix;

	Position	=	(float*)GetComponent(_entityId, "Position", 0);
	Rotation	=	(float*)GetComponent(_entityId, "Rotation", 0);
	Scale		=	(float*)GetComponent(_entityId, "Scale", 0);
	Matrix		=	(glm::mat4*)GetComponent(_entityId, "Render", "Mat");

	*Matrix = glm::scale(glm::vec3(Scale[0], Scale[1], Scale[2]));
	//*Matrix *= glm::rotate(glm::vec3(Rotation[0], Rotation[1], Rotation[2])); // quaternions?????
	*Matrix *= glm::translate(glm::vec3(Position[0], Position[1], Position[2]));
}