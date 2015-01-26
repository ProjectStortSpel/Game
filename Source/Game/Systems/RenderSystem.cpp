#include "RenderSystem.h"
#include "ECSL/Framework/Common/BitSet.h"
#include "Game/Quaternion.h"

RenderSystem::RenderSystem(Renderer::GraphicDevice* _graphics)
{
	m_graphics = _graphics;

}
RenderSystem::~RenderSystem()
{

}

void RenderSystem::Initialize()
{
	SetSystemName("Render System");

	/*	Rendersystem wants Position, Scale, Rotation and Render	*/
	AddComponentTypeToFilter("Position",	ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Rotation",	ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Scale",		ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Render",		ECSL::FilterType::Mandatory);

	AddComponentTypeToFilter("Hide",		ECSL::FilterType::Excluded);


	std::vector<unsigned int> bitsetComponents;
	bitsetComponents.push_back(ECSL::ComponentTypeManager::GetInstance().GetTableId("Position"));
	bitsetComponents.push_back(ECSL::ComponentTypeManager::GetInstance().GetTableId("Rotation"));
	bitsetComponents.push_back(ECSL::ComponentTypeManager::GetInstance().GetTableId("Scale"));

	m_bitMask = ECSL::BitSet::BitSetConverter::ArrayToBitSet(bitsetComponents, ECSL::ComponentTypeManager::GetInstance().GetComponentTypeCount());
	m_numberOfBitSets = ECSL::BitSet::GetIntCount(ECSL::ComponentTypeManager::GetInstance().GetComponentTypeCount());
	m_componentId = ECSL::ComponentTypeManager::GetInstance().GetTableId("ChangedComponents");

	m_positionId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Position");
	m_rotationId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Rotation");
	m_scaleId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Scale");
	m_renderId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Render");
	m_renderOffset = ECSL::ComponentTypeManager::GetInstance().GetComponentType(m_renderId)->GetVariables()->at("Mat").GetOffset();


	printf("RenderSystem initialized!\n");
}



void RenderSystem::Update(float _dt)
{
	auto entities = *GetEntities();
	
	for (auto entity : entities)
	{
		ECSL::BitSet::DataType* eBitMask = (ECSL::BitSet::DataType*)GetComponent(entity, m_componentId, 0);
		bool needsUpdate = false;
		for (unsigned int n = 0; n < m_numberOfBitSets; ++n)
		{

			if ((m_bitMask[n] & eBitMask[n]) != 0)
			{
				needsUpdate = true;
				break;
			}

		}

		if (needsUpdate)
			UpdateMatrix(entity);
	}
		
}

void RenderSystem::OnEntityAdded(unsigned int _entityId)
{
	int modelId = *(int*)GetComponent(_entityId, "Render", "ModelId");
	m_graphics->ActiveModel(modelId, true);

	/*	Update the matrix	*/
	UpdateMatrix(_entityId);
}

void RenderSystem::OnEntityRemoved(unsigned int _entityId)
{
	/*	Tell Graphics to disable model	*/
	int modelId = *(int*)GetComponent(_entityId, "Render", "ModelId");
	m_graphics->ActiveModel(modelId, false);
}



void RenderSystem::UpdateMatrix(unsigned int _entityId)
{
	float*		Position;
	float*		Rotation;
	float*		Scale;
	glm::mat4*	Matrix;

	Position = (float*)GetComponent(_entityId, m_positionId, 0);
	Rotation = (float*)GetComponent(_entityId, m_rotationId, 0);
	Scale = (float*)GetComponent(_entityId, m_scaleId, 0);
	Matrix = (glm::mat4*)GetComponent(_entityId, m_renderId, m_renderOffset);

	*Matrix = glm::translate(glm::vec3(Position[0], Position[1], Position[2]));

	/*Convert to quaternions*/
	Quaternion q_rotation;

	q_rotation.EulerToQuaternion(Rotation[0], Rotation[1], Rotation[2]);

	*Matrix *= q_rotation.QuaternionToMatrix();

	*Matrix *= glm::scale(glm::vec3(Scale[0], Scale[1], Scale[2]));

	ComponentHasChanged(_entityId, m_renderId);
}