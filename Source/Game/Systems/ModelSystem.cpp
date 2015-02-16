#include "ModelSystem.h"

ModelSystem::ModelSystem(Renderer::GraphicDevice* _graphics)
{
	m_graphics = _graphics;

}
ModelSystem::~ModelSystem()
{

}

void ModelSystem::Initialize()
{
	SetSystemName("ModelSystem");

	SetEntitiesAddedTaskCount(1);

	m_positionId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Position");
	m_rotationId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Rotation");
	m_scaleId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Scale");
	m_colorId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Color");

	AddComponentTypeToFilter("Model", ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Render", ECSL::FilterType::Excluded);

}

void ModelSystem::EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	for (auto entityId : _entities)
	{
		char* ModelData;
		ModelData = (char*)GetComponent(entityId, "Model", "ModelName");
		std::string ModelName = std::string(ModelData);
		ModelName.append(".object");
		ModelData = (char*)GetComponent(entityId, "Model", "ModelPath");
		std::string ModelPath = "content/models/";
		ModelPath.append(std::string(ModelData));
		ModelPath.append("/");

		int RenderType = *(int*)GetComponent(entityId, "Model", "RenderType");

		CreateComponentAndAddTo("Render", entityId);
		glm::mat4*	Matrix;
		Matrix = (glm::mat4*)GetComponent(entityId, "Render", "Mat");
		int* ModelId = (int*)GetComponent(entityId, "Render", "ModelId");

		//if (!HasComponent(entityId, m_positionId))
		//{ 
		//	CreateComponentAndAddTo("Position", entityId);
		//	float* Position = (float*)GetComponent(entityId, "Position", "X");
		//	Position[0] = 0.0f;
		//	Position[1] = 0.0f;
		//	Position[2] = 0.0f;
		//}
		//if (!HasComponent(entityId, m_rotationId))
		//{
		//	CreateComponentAndAddTo("Rotation", entityId);
		//	float* Rotation = (float*)GetComponent(entityId, "Rotation", "X");
		//	Rotation[0] = 0.0f;
		//	Rotation[1] = 0.0f;
		//	Rotation[2] = 0.0f;
		//}
		//if (!HasComponent(entityId, m_scaleId))
		//{
		//	CreateComponentAndAddTo("Scale", entityId);
		//	float* Scale = (float*)GetComponent(entityId, "Scale", "X");
		//	Scale[0] = 0.0f;
		//	Scale[1] = 0.0f;
		//	Scale[2] = 0.0f;
		//}
		//float* Color;
		//if (!HasComponent(entityId, m_colorId))
		//{ 
		//	CreateComponentAndAddTo("Color", entityId);
		//	float* Color = (float*)GetComponent(entityId, "Color", "X");
		//	Color[0] = 0.0f;
		//	Color[1] = 0.0f;
		//	Color[2] = 0.0f;
		//}

		float* renderColor = (float*)GetComponent(entityId, "Render", "ColorX");

		*ModelId = m_graphics->LoadModel(ModelPath, ModelName, Matrix, RenderType, renderColor);
	}
}