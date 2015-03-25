#include "DirectionalLightSystem.h"
#include "ECSL/Framework/Common/BitSet.h"

DirectionalLightSystem::DirectionalLightSystem(Renderer::GraphicDevice* _graphics)
{
	m_graphics = _graphics;
	
	m_bitMask = NULL;

}
DirectionalLightSystem::~DirectionalLightSystem()
{
	if (m_bitMask)
		free(m_bitMask);
}

void DirectionalLightSystem::Initialize()
{
	SetSystemName("DirectionalLight System");

	SetUpdateTaskCount(1);
	SetEntitiesAddedTaskCount(1);
	SetEntitiesRemovedTaskCount(1);

	AddComponentTypeToFilter("DirectionalLight", ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Hide", ECSL::FilterType::Excluded);


	std::vector<unsigned int> bitsetComponents;
	bitsetComponents.push_back(ECSL::ComponentTypeManager::GetInstance().GetTableId("DirectionalLight"));

	m_bitMask = ECSL::BitSet::BitSetConverter::ArrayToBitSet(bitsetComponents, ECSL::ComponentTypeManager::GetInstance().GetComponentTypeCount());
	m_numberOfBitSets = ECSL::BitSet::GetDataTypeCount(ECSL::ComponentTypeManager::GetInstance().GetComponentTypeCount());
	m_changedComponentId = ECSL::ComponentTypeManager::GetInstance().GetTableId("ChangedComponents");
	m_dirLightId = ECSL::ComponentTypeManager::GetInstance().GetTableId("DirectionalLight");
}

void DirectionalLightSystem::Update(const ECSL::RuntimeInfo& _runtime)
{
	auto entities = *GetEntities();

	/*	TODO: Some logic to not update matrix every frame	*/
	bool needsUpdate = false;
	for (auto entity : entities)
	{
		ECSL::BitSet::DataType* eBitMask = (ECSL::BitSet::DataType*)GetComponent(entity, m_changedComponentId, 0);
		
		for (unsigned int n = 0; n < m_numberOfBitSets; ++n)
		{

			if (m_bitMask[n] != 0 && (m_bitMask[n] & eBitMask[n]) != 0)
			{
				needsUpdate = true;
				break;
			}

		}

		if (needsUpdate)
		{
			UpdateDirectionalLight();
			break;
		}
	}


}

void DirectionalLightSystem::EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	UpdateDirectionalLight();
}

void DirectionalLightSystem::EntitiesRemoved(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	bool	doRebuff = false;
	for (auto entityId : _entities)
	{
		if (HasComponent(entityId, "Hide"))
			RemoveComponentFrom("Hide", entityId);
		else
			doRebuff = true;
	}

	if (doRebuff)
		UpdateDirectionalLight();
}


void DirectionalLightSystem::UpdateDirectionalLight()
{
	auto eDirLights = *GetEntities();

	if (eDirLights.size() == 0)
	{
		m_graphics->BufferDirectionalLight(0);
		return;
	}

	float* lightPointer = (float*)GetComponent(eDirLights.at(0), m_dirLightId, 0);

	m_graphics->BufferDirectionalLight(lightPointer);
}