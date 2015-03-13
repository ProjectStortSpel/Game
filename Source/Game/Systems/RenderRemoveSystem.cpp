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

	SetEntitiesAddedTaskCount(1);
	SetEntitiesRemovedTaskCount(1);

	/*	Rendersystem wants Position, Scale, Rotation and Render	*/
	AddComponentTypeToFilter("Render", ECSL::FilterType::Mandatory);

	
	m_addedToRendererId = ECSL::ComponentTypeManager::GetInstance().GetTableId("AddedToRenderer");
}

void RenderRemoveSystem::EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	for (auto entityId : _entities)
	{
		if (!HasComponent(entityId, m_addedToRendererId))
			CreateComponentAndAddTo(m_addedToRendererId, entityId);
	}
}

void RenderRemoveSystem::EntitiesRemoved(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	for (auto entityId : _entities)
	{
		if (HasComponent(entityId, m_addedToRendererId))
			RemoveComponentFrom(m_addedToRendererId, entityId);

		/*	Tell Graphics to disable model	*/
		int modelId = *(int*)GetComponent(entityId, "Render", "ModelId");
		m_graphics->RemoveModel(modelId);
	}
}