#include "ModelSystem.h"
#include "Game/HomePath.h"
#include "FileSystem/File.h"
#include "Game/Quaternion.h"

ModelSystem::ModelSystem(Renderer::GraphicDevice* _graphics, bool _isClient)
{
	m_graphics = _graphics;
	m_IsClient = _isClient;

}
ModelSystem::~ModelSystem()
{

}

void ModelSystem::Initialize()
{
	SetSystemName("ModelSystem");

	SetEntitiesAddedTaskCount(1);

	m_colorId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Color");
	m_modelId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Model");
	m_renderId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Render");

	m_positionId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Position");
	m_rotationId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Rotation");
	m_scaleId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Scale");
	m_staticModelId = ECSL::ComponentTypeManager::GetInstance().GetTableId("StaticModel");
	m_noShadowId = ECSL::ComponentTypeManager::GetInstance().GetTableId("NoShadow");

	AddComponentTypeToFilter("Model", ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Render", ECSL::FilterType::Excluded);

}

void ModelSystem::EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	for (auto entityId : _entities)
	{
		char* ModelData;
		ModelData = (char*)GetComponent(entityId, m_modelId, "ModelName");
		std::string ModelName = std::string(ModelData);
		ModelName.append(".object");

		ModelData = (char*)GetComponent(entityId, m_modelId, "ModelPath");

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

		
		int RenderType = *(int*)GetComponent(entityId, m_modelId, "RenderType");

		bool noShadow = false;
		if (HasComponent(entityId, m_noShadowId))
		{
			noShadow = true;
		}

		CreateComponentAndAddTo("Render", entityId);
		glm::mat4*	Matrix;
		Matrix = (glm::mat4*)GetComponent(entityId, m_renderId, "Mat");
		int* ModelId = (int*)GetComponent(entityId, m_renderId, "ModelId");

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

		if (!HasComponent(entityId, m_colorId))
		{
			CreateComponentAndAddTo("Color", entityId);
			float* _Color = (float*)GetComponent(entityId, m_colorId, "X");
			_Color[0] = 0.0f;
			_Color[1] = 0.0f;
			_Color[2] = 0.0f;
		}

		float* Color = (float*)GetComponent(entityId, m_renderId, "ColorX");

		bool isStatic = false;
		#if defined(__ANDROID__) || defined(__IOS__)
		if (HasComponent(entityId, m_staticModelId))
		{
			isStatic = true;

			float* position = (float*)GetComponent(entityId, m_positionId, 0);
			float* rotation = (float*)GetComponent(entityId, m_rotationId, 0);
			float* scale = (float*)GetComponent(entityId, m_scaleId, 0);

			*Matrix = glm::mat4(1.0f);
			*Matrix *= glm::translate(glm::vec3(position[0], position[1], position[2]));
			Quaternion q_rotation;
			q_rotation.EulerToQuaternion(rotation[0], rotation[1], rotation[2]);
			*Matrix *= q_rotation.QuaternionToMatrix();
			*Matrix *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

			float* _Color = (float*)GetComponent(entityId, m_colorId, "X");
			Color[0] = _Color[0];
			Color[1] = _Color[1];
			Color[2] = _Color[2];
		}
		#endif

		*ModelId = m_graphics->LoadModel(paths, ModelName, Matrix, RenderType, Color, !noShadow, isStatic);
		
	}
}