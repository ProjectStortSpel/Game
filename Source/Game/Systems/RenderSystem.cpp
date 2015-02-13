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
	SetSystemName("RenderSystem");

	SetUpdateTaskCount(GetThreadCount());
	SetEntitiesAddedTaskCount(1);
	SetEntitiesRemovedTaskCount(1);

	/*	Rendersystem wants Position, Scale, Rotation and Render	*/
	AddComponentTypeToFilter("Position",	ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Rotation",	ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Scale",		ECSL::FilterType::Mandatory);
	//AddComponentTypeToFilter("Color",		ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Render",		ECSL::FilterType::RequiresOneOf);
	AddComponentTypeToFilter("Model",		ECSL::FilterType::RequiresOneOf);

	//AddComponentTypeToFilter("Parent",		ECSL::FilterType::RequiresOneOf);

	AddComponentTypeToFilter("Hide",		ECSL::FilterType::Excluded);


	std::vector<unsigned int> bitsetComponents;
	bitsetComponents.push_back(ECSL::ComponentTypeManager::GetInstance().GetTableId("Position"));
	bitsetComponents.push_back(ECSL::ComponentTypeManager::GetInstance().GetTableId("Rotation"));
	bitsetComponents.push_back(ECSL::ComponentTypeManager::GetInstance().GetTableId("Scale"));
	//bitsetComponents.push_back(ECSL::ComponentTypeManager::GetInstance().GetTableId("Color"));

	m_bitMask = ECSL::BitSet::BitSetConverter::ArrayToBitSet(bitsetComponents, ECSL::ComponentTypeManager::GetInstance().GetComponentTypeCount());
	m_numberOfBitSets = ECSL::BitSet::GetDataTypeCount(ECSL::ComponentTypeManager::GetInstance().GetComponentTypeCount());
	m_componentId = ECSL::ComponentTypeManager::GetInstance().GetTableId("ChangedComponents");

	m_positionId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Position");
	m_rotationId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Rotation");
	m_scaleId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Scale");
	m_colorId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Color");
	m_renderId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Render");
	m_renderOffset = ECSL::ComponentTypeManager::GetInstance().GetComponentType(m_renderId)->GetVariables()->at("Mat").GetOffset();
	m_parentId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Parent");
	m_isparentId = ECSL::ComponentTypeManager::GetInstance().GetTableId("IsParent");
}

void RenderSystem::Update(const ECSL::RuntimeInfo& _runtime)
{
	auto entities = *GetEntities();
	unsigned int startAt, endAt;
	MPL::MathHelper::SplitIterations(startAt, endAt, (unsigned int)entities.size(), _runtime.TaskIndex, _runtime.TaskCount);
	for (unsigned int i = startAt; i < endAt; ++i)
	{
		unsigned int entity = entities[i];
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

void RenderSystem::EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	for (auto entityId : _entities)
	{
		int modelId = *(int*)GetComponent(entityId, "Render", "ModelId");
		m_graphics->ActiveModel(modelId, true);

		/*	Update the matrix	*/
		UpdateMatrix(entityId);
	}
}

void RenderSystem::EntitiesRemoved(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	for (auto entityId : _entities)
	{
		/*	Tell Graphics to disable model	*/
		int modelId = *(int*)GetComponent(entityId, "Render", "ModelId");
		m_graphics->ActiveModel(modelId, false);
	}
}


void RenderSystem::UpdateMatrix(unsigned int _entityId)
{
	float*		Position;
	float*		Rotation;
	float*		Scale;
	glm::mat4*	Matrix;

	Position	=	(float*)GetComponent(_entityId, m_positionId , 0);
	Rotation	=	(float*)GetComponent(_entityId, m_rotationId, 0);
	Scale		=	(float*)GetComponent(_entityId, m_scaleId, 0);
	Matrix		=	(glm::mat4*)GetComponent(_entityId, m_renderId, m_renderOffset);

	*Matrix = glm::mat4(1);
	//CHECK IF IT HAS A PARENT
	if (HasComponent(_entityId, m_parentId))
	{
		int* Parent = (int*)GetComponent(_entityId, m_parentId, 0);
		//GET PARENT MATRIX
		*Matrix = *(glm::mat4*)GetComponent(*Parent, m_renderId, m_renderOffset);
	}

	*Matrix *= glm::translate(glm::vec3(Position[0], Position[1], Position[2]));

	/*Convert to quaternions*/
	Quaternion q_rotation;

	q_rotation.EulerToQuaternion(Rotation[0], Rotation[1], Rotation[2]);

	*Matrix *= q_rotation.QuaternionToMatrix();

	*Matrix *= glm::scale(glm::vec3(Scale[0], Scale[1], Scale[2]));

	ComponentHasChanged(_entityId, m_renderId);

	// UPDATE CHIDRENS MATRIX
	/*if (EntityHasComponent(_entityId, m_isparentId))
	{
		int* Parent = (int*)GetComponent(_entityId, m_parentId, 0);
		auto children = *GetEntities(m_isparentId);
	}*/
}