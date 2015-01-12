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
	SetSystemName("Initialize System");

	AddComponentTypeToFilter("Model", ECSL::FilterType::Mandatory);


	AddComponentTypeToFilter("Render", ECSL::FilterType::Excluded);


	printf("ModelSystem initialized!\n");
}

void ModelSystem::Update(float _dt)
{
}

void ModelSystem::OnEntityAdded(unsigned int _entityId)
{
	char* ModelData;
	ModelData = (char*)GetComponent(_entityId, "Model", "ModelName");
	std::string ModelName = std::string(ModelData);
	ModelName.append(".object");
	ModelData = (char*)GetComponent(_entityId, "Model", "ModelPath");
	std::string ModelPath = "content/models/";
	ModelPath.append(std::string(ModelData));
	ModelPath.append("/");



	int RenderType = *(int*)GetComponent(_entityId, "Model", "RenderType");
	int ViewSpace = *(int*)GetComponent(_entityId, "Model", "ViewSpace");

	if (ModelName == "ply1.object")
		int a = 2;

	CreateComponentAndAddTo("Render", _entityId);
	glm::mat4*	Matrix;
	Matrix = (glm::mat4*)GetComponent(_entityId, "Render", "Mat");
	int* ModelId = (int*)GetComponent(_entityId, "Render", "ModelId");
	*ModelId = m_graphics->LoadModel(ModelPath, ModelName, Matrix, RenderType, ViewSpace);
}

void ModelSystem::OnEntityRemoved(unsigned int _entityId)
{
}