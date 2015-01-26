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
	SetSystemName("ResetChangedSystem");

	SetUpdateTaskCount(1);
	SetEntitiesAddedTaskCount(0);
	SetEntitiesRemovedTaskCount(0);

	/*	Rendersystem wants Network	*/
	//AddComponentTypeToFilter("ChangedComponents", ECSL::FilterType::Mandatory);

	m_componentId = ECSL::ComponentTypeManager::GetInstance().GetTableId("ChangedComponents");
	m_componentByteSize = ECSL::ComponentTypeManager::GetInstance().GetComponentType(m_componentId)->GetByteSize();
}

void ResetChangedSystem::Update(const ECSL::RuntimeInfo& _runtime)
{
	memset(GetComponent(0, m_componentId, 0), 0, m_componentByteSize * GetEntityCountLimit());
}