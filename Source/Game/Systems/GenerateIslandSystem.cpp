#include "GenerateIslandSystem.h"
#include "Game/HomePath.h"
#include "FileSystem/File.h"
#include "Game/Quaternion.h"
#include "Game/MeshCreator/MeshCreator.h"

GenerateIslandSystem::GenerateIslandSystem(Renderer::GraphicDevice* _graphics, bool _isClient)
{
	m_graphics = _graphics;
	m_isClient = _isClient;
}

void GenerateIslandSystem::Initialize()
{
	SetSystemName("GenerateIslandSystem");

	SetEntitiesAddedTaskCount(1);

	AddComponentTypeToFilter("GenerateIsland", ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Render", ECSL::FilterType::Excluded);

	m_generateIslandId = ECSL::ComponentTypeManager::GetInstance().GetTableId("GenerateIsland");
	m_generateIslandMapId = ECSL::ComponentTypeManager::GetInstance().GetComponentType(m_generateIslandId)->GetVariables()->at("Map").GetOffset();
	m_generateIslandSizeXId = ECSL::ComponentTypeManager::GetInstance().GetComponentType(m_generateIslandId)->GetVariables()->at("SizeX").GetOffset();
	m_generateIslandSizeZId = ECSL::ComponentTypeManager::GetInstance().GetComponentType(m_generateIslandId)->GetVariables()->at("SizeZ").GetOffset();
	m_generateIslandOffsetXId = ECSL::ComponentTypeManager::GetInstance().GetComponentType(m_generateIslandId)->GetVariables()->at("OffsetX").GetOffset();
	m_generateIslandOffsetZId = ECSL::ComponentTypeManager::GetInstance().GetComponentType(m_generateIslandId)->GetVariables()->at("OffsetZ").GetOffset();
	
	m_renderId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Render");
}

void GenerateIslandSystem::EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	for (unsigned int entityId : _entities)
	{
		std::string map = GetString(entityId, m_generateIslandId, m_generateIslandMapId);
		int sizeX = *((int*)GetComponent(entityId, m_generateIslandId, m_generateIslandSizeXId));
		int sizeZ = *((int*)GetComponent(entityId, m_generateIslandId, m_generateIslandSizeZId));
		int offsetX, offsetZ;
		memcpy(&offsetX, GetComponent(entityId, m_generateIslandId, m_generateIslandOffsetXId), sizeof(int));
		memcpy(&offsetZ, GetComponent(entityId, m_generateIslandId, m_generateIslandOffsetZId), sizeof(int));

		std::vector<std::string> stringMap;
		for (int z = 0; z < sizeZ; ++z)
		{
			std::string	line;
			for (int x = 0; x < sizeX; ++x)
				line.push_back((char)map[z*sizeX + x]);
			stringMap.push_back(line);
		}

		CreateComponentAndAddTo("Render", entityId);
		glm::mat4* modelMatrix = (glm::mat4*)GetComponent(entityId, m_renderId, "Mat");
		float* color = (float*)GetComponent(entityId, m_renderId, "ColorX");
		int* modelId = (int*)GetComponent(entityId, m_renderId, "ModelId");

		*modelMatrix = glm::translate(glm::vec3((float)offsetX, 0.15f, (float)offsetZ));
		color[0] = 0.0f;
		color[1] = 0.0f;
		color[2] = 0.0f;

		MeshCreator meshcreator;
		meshcreator.CreateMesh(stringMap);
		Renderer::ModelToLoadFromSource model;
		model.key = "meshcreator";
		model.positions = meshcreator.position;
		model.normals = meshcreator.normals;
		model.tangents = meshcreator.tangents;
		model.bitangents = meshcreator.bitangents;
		model.texCoords = meshcreator.uvs;
		model.diffuseTextureFilepath = "content/textures/dirt.png";
		model.normalTextureFilepath = "content/textures/normalPixel.png";
		model.specularTextureFilepath = "content/textures/blackPixel.png";
		model.RenderType = 0;
		model.Color = color;
		model.MatrixPtr = modelMatrix;
		model.CastShadow = false;
		*modelId = m_graphics->LoadModel(&model);
	}
}