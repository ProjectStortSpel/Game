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
	/*	Rendersystem wants Position, Scale, Rotation and Render	*/
	AddComponentTypeToFilter("Model", ECSL::FilterType::Mandatory);


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

	CreateComponentAndAddTo("Render", _entityId);
	glm::mat4*	Matrix;
	Matrix = (glm::mat4*)GetComponent(_entityId, "Render", "Mat");
	int* ModelId = (int*)GetComponent(_entityId, "Render", "ModelId");
	*ModelId = m_graphics->LoadModel(ModelPath, ModelName, Matrix);

	RemoveComponentFrom("Model", _entityId);
}

void ModelSystem::OnEntityRemoved(unsigned int _entityId)
{
	int a = 2;
}