#include "AModelSystem.h"

AModelSystem::AModelSystem(Renderer::GraphicDevice* _graphics)
{
	m_graphics = _graphics;

}
AModelSystem::~AModelSystem()
{

}

void AModelSystem::Initialize()
{
	SetSystemName("AModelSystem");

	SetEntitiesAddedTaskCount(1);

	AddComponentTypeToFilter("AModel", ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Render", ECSL::FilterType::Excluded);

}

void AModelSystem::EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	for (auto entityId : _entities)
	{
		char* ModelData;
		ModelData = (char*)GetComponent(entityId, "AModel", "ModelName");
		std::string ModelName = std::string(ModelData);
		ModelName.append(".object");
		ModelData = (char*)GetComponent(entityId, "AModel", "ModelPath");
		std::string ModelPath = "content/models/";
		ModelPath.append(std::string(ModelData));
		ModelPath.append("/");

		int RenderType = 1;

		CreateComponentAndAddTo("Render", entityId);
		glm::mat4*	Matrix;
		Matrix = (glm::mat4*)GetComponent(entityId, "Render", "Mat");
		int* ModelId = (int*)GetComponent(entityId, "Render", "ModelId");
		*ModelId = m_graphics->LoadModel(ModelPath, ModelName, Matrix, RenderType);
	}
}