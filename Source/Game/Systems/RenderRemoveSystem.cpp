#include "RenderRemoveSystem.h"

RenderRemoveSystem::RenderRemoveSystem(Renderer::GraphicDevice* _graphics)
{
	m_graphics = _graphics;

}
RenderRemoveSystem::~RenderRemoveSystem()
{

}

void RenderRemoveSystem::Initialize()
{
	SetSystemName("Render Remove System");

	/*	Rendersystem wants Position, Scale, Rotation and Render	*/
	AddComponentTypeToFilter("Render", ECSL::FilterType::Mandatory);


	

	printf("RenderRemoveSystem initialized!\n");
}

void RenderRemoveSystem::Update(float _dt)
{
}

void RenderRemoveSystem::OnEntityAdded(unsigned int _entityId)
{
}

void RenderRemoveSystem::OnEntityRemoved(unsigned int _entityId)
{
	/*	Tell Graphics to disable model	*/
	int modelId = (int)*GetComponent(_entityId, "Render", "ModelId");
	m_graphics->RemoveModel(modelId);
}