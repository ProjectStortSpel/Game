#include "RenderSystem.h"

RenderSystem::RenderSystem()
{


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

}

void RenderSystem::OnEntityAdded(unsigned int _entityId)
{

}

void RenderSystem::OnEntityRemoved(unsigned int _entityId)
{

}