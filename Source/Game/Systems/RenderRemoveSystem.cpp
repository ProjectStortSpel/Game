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

	SetEntitiesRemovedTaskCount(1);

	/*	Rendersystem wants Position, Scale, Rotation and Render	*/
	AddComponentTypeToFilter("Render", ECSL::FilterType::Mandatory);

}

void RenderRemoveSystem::EntitiesRemoved(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	for (auto entityId : _entities)
	{
		/*	Tell Graphics to disable model	*/
		int modelId = *(int*)GetComponent(entityId, "Render", "ModelId");
		m_graphics->RemoveModel(modelId);
	}
}