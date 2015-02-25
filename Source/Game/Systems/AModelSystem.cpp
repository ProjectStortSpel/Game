#include "AModelSystem.h"
#include "Game/HomePath.h"
#include "FileSystem/File.h"

AModelSystem::AModelSystem(Renderer::GraphicDevice* _graphics, bool _isClient)
{
	m_graphics = _graphics;
	m_IsClient = _isClient;

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

		std::vector<std::string> paths;

		if (m_IsClient)
			paths = HomePath::GetPaths(HomePath::Type::Client);
		else
			paths = HomePath::GetPaths(HomePath::Type::Server);

		for (int i = 0; i < paths.size(); ++i)
		{
			paths[i].append("models/");
			paths[i].append(std::string(ModelData));
			paths[i].append("/");
		}

		
		int RenderType = 1;

		CreateComponentAndAddTo("Render", entityId);
		glm::mat4*	Matrix;
		Matrix = (glm::mat4*)GetComponent(entityId, "Render", "Mat");
		int* ModelId = (int*)GetComponent(entityId, "Render", "ModelId");
		*ModelId = m_graphics->LoadModel(paths, ModelName, Matrix, RenderType);
			
	}
}