#include "PointlightSystem.h"
#include "ECSL/Framework/Common/BitSet.h"

PointlightSystem::PointlightSystem(Renderer::GraphicDevice* _graphics)
{
	m_graphics = _graphics;

}
PointlightSystem::~PointlightSystem()
{

}

void PointlightSystem::Initialize()
{
	SetSystemName("Pointlight System");

	/*	Rendersystem wants Position, Scale, Rotation and Render	*/
	AddComponentTypeToFilter("Pointlight", ECSL::FilterType::Mandatory);


	std::vector<unsigned int> bitsetComponents;
	bitsetComponents.push_back(ECSL::ComponentTypeManager::GetInstance().GetTableId("Pointlight"));

	m_bitMask = ECSL::BitSet::BitSetConverter::ArrayToBitSet(bitsetComponents, ECSL::ComponentTypeManager::GetInstance().GetComponentTypeCount());
	m_numberOfBitSets = ECSL::BitSet::GetDataTypeCount(ECSL::ComponentTypeManager::GetInstance().GetComponentTypeCount());
	m_changedComponentId = ECSL::ComponentTypeManager::GetInstance().GetTableId("ChangedComponents");
	m_pointLightId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Pointlight");

	printf("Pointlight initialized!\n");
}

void PointlightSystem::Update(float _dt)
{
	auto entities = *GetEntities();

	/*	TODO: Some logic to not update matrix every frame	*/
	bool needsUpdate = false;
	for (auto entity : entities)
	{
		ECSL::BitSet::DataType* eBitMask = (ECSL::BitSet::DataType*)GetComponent(entity, m_changedComponentId, 0);
		
		for (unsigned int n = 0; n < m_numberOfBitSets; ++n)
		{

			if ((m_bitMask[n] & eBitMask[n]) != 0)
			{
				needsUpdate = true;
				break;
			}

		}

		if (needsUpdate)
		{
			UpdatePointLights();
			break;
		}
	}


}

void PointlightSystem::OnEntityAdded(unsigned int _entityId)
{
	UpdatePointLights();
}

void PointlightSystem::OnEntityRemoved(unsigned int _entityId)
{
	UpdatePointLights();
}


void PointlightSystem::UpdatePointLights()
{
	auto ePointlights = *GetEntities();

	if (ePointlights.size() == 0)
	{
		float** tempPointer = new float*[1];
		m_graphics->BufferPointlights(ePointlights.size(), tempPointer);
		delete tempPointer;
		return;
	}

	float** lightPointers = new float*[ePointlights.size()];
	for (int n = 0; n < ePointlights.size(); ++n)
		lightPointers[n] = (float*)GetComponent(ePointlights.at(n), m_pointLightId, 0);

	m_graphics->BufferPointlights(ePointlights.size(), lightPointers);

	delete lightPointers;
}