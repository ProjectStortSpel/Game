#include "ResetChangedSystem.h"
#include "ECSL/Managers/ComponentTypeManager.h"

ResetChangedSystem::ResetChangedSystem()
{
}
ResetChangedSystem::~ResetChangedSystem()
{
}

void ResetChangedSystem::Initialize()
{
	SetSystemName("Reset Changed System");
	/*	Rendersystem wants Network	*/
	//AddComponentTypeToFilter("ChangedComponents", ECSL::FilterType::Mandatory);

	m_componentId = ECSL::ComponentTypeManager::GetInstance().GetTableId("ChangedComponents");
	m_componentByteSize = ECSL::ComponentTypeManager::GetInstance().GetComponentType(m_componentId)->GetByteSize();

	printf("ResetChangedSystem initialized!\n");
}

void ResetChangedSystem::Update(float _dt)
{
	memset(GetComponent(0, m_componentId, 0), 0, m_componentByteSize * GetEntityCountLimit());
}

void ResetChangedSystem::OnEntityAdded(unsigned int _entityId)
{

}

void ResetChangedSystem::OnEntityRemoved(unsigned int _entityId)
{

}