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

		float* Color;
		if (!HasComponent(entityId, m_colorId))
		{ 
			CreateComponentAndAddTo("Color", entityId);
			Color = (float*)GetComponent(entityId, "Color", "X");
			Color[0] = 0.0f;
			Color[1] = 0.0f;
			Color[2] = 0.0f;
		}
		Color = (float*)GetComponent(entityId, "Color", "X");

		*ModelId = m_graphics->LoadModel(ModelPath, ModelName, Matrix, RenderType, Color);
	}
}