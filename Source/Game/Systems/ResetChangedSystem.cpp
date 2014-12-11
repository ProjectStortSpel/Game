#include "ResetChangedSystem.h"

ResetChangedSystem::ResetChangedSystem()
{
}
ResetChangedSystem::~ResetChangedSystem()
{
}

void ResetChangedSystem::Initialize()
{
	/*	Rendersystem wants Network	*/
	//AddComponentTypeToFilter("ChangedComponents", ECSL::FilterType::Mandatory);

	printf("ResetChangedSystem initialized!\n");
}

void ResetChangedSystem::Update(float _dt)
{
	auto entities = *GetEntities();
	for (auto entity : entities)
	{
		ECSL::BitSet::DataType* data;
		data = (ECSL::BitSet::DataType*)GetComponent(entity, "ChangedComponents", 0);
		for (int n = 0; n < ECSL::BitSet::GetIntCount(ECSL::ComponentTypeManager::GetInstance().GetComponentTypeCount()); ++n)
			data[n] = 0;
	}
}

void ResetChangedSystem::OnEntityAdded(unsigned int _entityId)
{

}

void ResetChangedSystem::OnEntityRemoved(unsigned int _entityId)
{

}