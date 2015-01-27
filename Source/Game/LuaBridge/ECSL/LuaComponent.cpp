#include "LuaComponent.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "ECSL/Framework/Components/ComponentType.h"

namespace LuaBridge
{
	LuaComponent::LuaComponent(lua_State* L)
	{
		m_dataLocation = nullptr;
		m_system = nullptr;
		m_world = nullptr;
	}

	LuaComponent::LuaComponent(ECSL::DataLocation dataLocation, ECSL::System* system,
		unsigned int entityId, const std::string& componentName)
	{
		m_dataLocation = dataLocation;
		m_system = system;
		m_world = nullptr;
		m_entityId = entityId;
		m_componentName = componentName;
	}
	
	LuaComponent::LuaComponent(ECSL::DataLocation dataLocation, ECSL::World* world,
		unsigned int entityId, const std::string& componentName)
	{
		m_dataLocation = dataLocation;
		m_system = nullptr;
		m_world = world;
		m_entityId = entityId;
		m_componentName = componentName;
	}

	LuaComponent::~LuaComponent() { }

	void LuaComponent::Embed(lua_State* L)
	{
		LuaEmbedder::EmbedClass<LuaComponent>(L, "Component");

		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "GetFloat", &LuaComponent::GetFloat);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "GetFloat2", &LuaComponent::GetFloat2);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "GetFloat3", &LuaComponent::GetFloat3);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "GetFloat4", &LuaComponent::GetFloat4);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "GetFloat5", &LuaComponent::GetFloat5);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "SetFloat", &LuaComponent::SetFloat);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "SetFloat2", &LuaComponent::SetFloat2);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "SetFloat3", &LuaComponent::SetFloat3);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "SetFloat4", &LuaComponent::SetFloat4);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "SetFloat5", &LuaComponent::SetFloat5);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "GetInt", &LuaComponent::GetInt);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "GetInt2", &LuaComponent::GetInt2);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "GetInt3", &LuaComponent::GetInt3);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "GetInt4", &LuaComponent::GetInt4);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "SetInt", &LuaComponent::SetInt);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "SetInt2", &LuaComponent::SetInt2);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "SetInt3", &LuaComponent::SetInt3);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "SetInt4", &LuaComponent::SetInt4);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "GetBool", &LuaComponent::GetBool);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "SetBool", &LuaComponent::SetBool);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "GetString", &LuaComponent::GetString);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "SetString", &LuaComponent::SetString);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "SetModel", &LuaComponent::SetModel);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "SetPointlight", &LuaComponent::SetPointlight);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "GetPointlight", &LuaComponent::GetPointlight);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "SetDirectionalLight", &LuaComponent::SetDirectionalLight);
		LuaEmbedder::EmbedClassFunction<LuaComponent>(L, "Component", "GetDirectionalLight", &LuaComponent::GetDirectionalLight);
		
		LuaEmbedder::EmbedClassProperty<LuaComponent>(L, "Component", "Float", &LuaComponent::GetFloat, &LuaComponent::SetFloat);
		LuaEmbedder::EmbedClassProperty<LuaComponent>(L, "Component", "Int", &LuaComponent::GetInt, &LuaComponent::SetInt);
		LuaEmbedder::EmbedClassProperty<LuaComponent>(L, "Component", "Bool", &LuaComponent::GetBool, &LuaComponent::SetBool);
		LuaEmbedder::EmbedClassProperty<LuaComponent>(L, "Component", "String", &LuaComponent::GetString, &LuaComponent::SetString);
	}

	int LuaComponent::GetFloat(lua_State* L)
	{
		assert(m_dataLocation);
		if (LuaEmbedder::IsInt(L, 1))
			LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[LuaEmbedder::PullInt(L, 1)]));
		else
			LuaEmbedder::PushFloat(L, (float)((float*)m_dataLocation)[0]);
		return 1;
	}
	int LuaComponent::GetFloat2(lua_State* L)
	{
		assert(m_dataLocation);
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[0]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[1]));
		return 2;
	}
	int LuaComponent::GetFloat3(lua_State* L)
	{
		assert(m_dataLocation);
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[0]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[1]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[2]));
		return 3;
	}
	int LuaComponent::GetFloat4(lua_State* L)
	{
		assert(m_dataLocation);
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[0]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[1]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[2]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[3]));
		return 4;
	}
	int LuaComponent::GetFloat5(lua_State* L)
	{
		assert(m_dataLocation);
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[0]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[1]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[2]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[3]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[4]));
		return 5;
	}
	int LuaComponent::SetFloat(lua_State* L)
	{
		assert(m_dataLocation);
		bool notifyNetwork = true;

		if (LuaEmbedder::IsInt(L, 2))
		{
			((float*)m_dataLocation)[LuaEmbedder::PullInt(L, 2)] = LuaEmbedder::PullFloat(L, 1);

			if (LuaEmbedder::IsBool(L, 3))
				notifyNetwork = LuaEmbedder::PullBool(L, 3);
		}

		else
		{
			((float*)m_dataLocation)[0] = LuaEmbedder::PullFloat(L, 1);

			if (LuaEmbedder::IsBool(L, 2))
				notifyNetwork = LuaEmbedder::PullBool(L, 2);
		}
		ComponentHasChanged(notifyNetwork);
		return 0;
	}
	int LuaComponent::SetFloat2(lua_State* L)
	{
		assert(m_dataLocation);
		((float*)m_dataLocation)[0] = LuaEmbedder::PullFloat(L, 1);
		((float*)m_dataLocation)[1] = LuaEmbedder::PullFloat(L, 2);
		bool notifyNetwork = true;
		if (LuaEmbedder::IsBool(L, 3))
			notifyNetwork = LuaEmbedder::PullBool(L, 3);

		ComponentHasChanged(notifyNetwork);
		return 0;
	}
	int LuaComponent::SetFloat3(lua_State* L)
	{
		assert(m_dataLocation);
		((float*)m_dataLocation)[0] = LuaEmbedder::PullFloat(L, 1);
		((float*)m_dataLocation)[1] = LuaEmbedder::PullFloat(L, 2);
		((float*)m_dataLocation)[2] = LuaEmbedder::PullFloat(L, 3);
		bool notifyNetwork = true;
		if (LuaEmbedder::IsBool(L, 4))
			notifyNetwork = LuaEmbedder::PullBool(L, 4);

		ComponentHasChanged(notifyNetwork);
		return 0;
	}
	int LuaComponent::SetFloat4(lua_State* L)
	{
		assert(m_dataLocation);
		((float*)m_dataLocation)[0] = LuaEmbedder::PullFloat(L, 1);
		((float*)m_dataLocation)[1] = LuaEmbedder::PullFloat(L, 2);
		((float*)m_dataLocation)[2] = LuaEmbedder::PullFloat(L, 3);
		((float*)m_dataLocation)[3] = LuaEmbedder::PullFloat(L, 4);
		bool notifyNetwork = true;
		if (LuaEmbedder::IsBool(L, 5))
			notifyNetwork = LuaEmbedder::PullBool(L, 5);

		ComponentHasChanged(notifyNetwork);
		return 0;
	}
	int LuaComponent::SetFloat5(lua_State* L)
	{
		assert(m_dataLocation);
		((float*)m_dataLocation)[0] = LuaEmbedder::PullFloat(L, 1);
		((float*)m_dataLocation)[1] = LuaEmbedder::PullFloat(L, 2);
		((float*)m_dataLocation)[2] = LuaEmbedder::PullFloat(L, 3);
		((float*)m_dataLocation)[3] = LuaEmbedder::PullFloat(L, 4);
		((float*)m_dataLocation)[4] = LuaEmbedder::PullFloat(L, 5);
		bool notifyNetwork = true;
		if (LuaEmbedder::IsBool(L, 6))
			notifyNetwork = LuaEmbedder::PullBool(L, 6);

		ComponentHasChanged(notifyNetwork);
		return 0;
	}

	int LuaComponent::GetInt(lua_State* L)
	{
		assert(m_dataLocation);
		if (LuaEmbedder::IsInt(L, 1))
			LuaEmbedder::PushInt(L, (int)(((int*)m_dataLocation)[LuaEmbedder::PullInt(L, 1)]));
		else
			LuaEmbedder::PushInt(L, (int)((int*)m_dataLocation)[0]);
		return 1;
	}
	int LuaComponent::GetInt2(lua_State* L)
	{
		assert(m_dataLocation);
		LuaEmbedder::PushInt(L, (int)(((int*)m_dataLocation)[0]));
		LuaEmbedder::PushInt(L, (int)(((int*)m_dataLocation)[1]));
		return 2;
	}
	int LuaComponent::GetInt3(lua_State* L)
	{
		assert(m_dataLocation);
		LuaEmbedder::PushInt(L, (int)(((int*)m_dataLocation)[0]));
		LuaEmbedder::PushInt(L, (int)(((int*)m_dataLocation)[1]));
		LuaEmbedder::PushInt(L, (int)(((int*)m_dataLocation)[2]));
		return 3;
	}
	int LuaComponent::GetInt4(lua_State* L)
	{
		assert(m_dataLocation);
		LuaEmbedder::PushInt(L, (int)(((int*)m_dataLocation)[0]));
		LuaEmbedder::PushInt(L, (int)(((int*)m_dataLocation)[1]));
		LuaEmbedder::PushInt(L, (int)(((int*)m_dataLocation)[2]));
		LuaEmbedder::PushInt(L, (int)(((int*)m_dataLocation)[3]));
		return 4;
	}
	int LuaComponent::SetInt(lua_State* L)
	{
		assert(m_dataLocation);
		bool notifyNetwork = true;

		if (LuaEmbedder::IsInt(L, 2))
		{
			((int*)m_dataLocation)[LuaEmbedder::PullInt(L, 2)] = LuaEmbedder::PullInt(L, 1);

			if (LuaEmbedder::IsBool(L, 3))
				notifyNetwork = LuaEmbedder::PullBool(L, 3);
		}
		else
		{
			((int*)m_dataLocation)[0] = LuaEmbedder::PullInt(L, 1);

			if (LuaEmbedder::IsBool(L, 2))
				notifyNetwork = LuaEmbedder::PullBool(L, 2);
		}

		ComponentHasChanged(notifyNetwork);
		return 0;
	}
	int LuaComponent::SetInt2(lua_State* L)
	{
		assert(m_dataLocation);
		((int*)m_dataLocation)[0] = LuaEmbedder::PullInt(L, 1);
		((int*)m_dataLocation)[1] = LuaEmbedder::PullInt(L, 2);

		bool notifyNetwork = true;
		if (LuaEmbedder::IsBool(L, 3))
			notifyNetwork = LuaEmbedder::PullBool(L, 3);

		ComponentHasChanged(notifyNetwork);
		return 0;
	}
	int LuaComponent::SetInt3(lua_State* L)
	{
		assert(m_dataLocation);
		((int*)m_dataLocation)[0] = LuaEmbedder::PullInt(L, 1);
		((int*)m_dataLocation)[1] = LuaEmbedder::PullInt(L, 2);
		((int*)m_dataLocation)[2] = LuaEmbedder::PullInt(L, 3);
		bool notifyNetwork = true;
		if (LuaEmbedder::IsBool(L, 4))
			notifyNetwork = LuaEmbedder::PullBool(L, 4);

		ComponentHasChanged(notifyNetwork);
		return 0;
	}
	int LuaComponent::SetInt4(lua_State* L)
	{
		assert(m_dataLocation);
		((int*)m_dataLocation)[0] = LuaEmbedder::PullInt(L, 1);
		((int*)m_dataLocation)[1] = LuaEmbedder::PullInt(L, 2);
		((int*)m_dataLocation)[2] = LuaEmbedder::PullInt(L, 3);
		((int*)m_dataLocation)[3] = LuaEmbedder::PullInt(L, 4);
		bool notifyNetwork = true;
		if (LuaEmbedder::IsBool(L, 5))
			notifyNetwork = LuaEmbedder::PullBool(L, 5);

		ComponentHasChanged(notifyNetwork);
		return 0;
	}

	int LuaComponent::GetBool(lua_State* L)
	{
		assert(m_dataLocation);
		if (LuaEmbedder::IsInt(L, 1))
			LuaEmbedder::PushBool(L, (bool)(((bool*)m_dataLocation)[LuaEmbedder::PullInt(L, 1)]));
		else
			LuaEmbedder::PushBool(L, (bool)((bool*)m_dataLocation)[0]);
		return 1;
	}
	int LuaComponent::SetBool(lua_State* L)
	{
		assert(m_dataLocation);
		bool notifyNetwork = true;

		if (LuaEmbedder::IsInt(L, 2))
		{
			((bool*)m_dataLocation)[LuaEmbedder::PullInt(L, 2)] = LuaEmbedder::PullBool(L, 1);

			if (LuaEmbedder::IsBool(L, 3))
				notifyNetwork = LuaEmbedder::PullBool(L, 3);
		}
		else
		{
			((bool*)m_dataLocation)[0] = LuaEmbedder::PullBool(L, 1);
			if (LuaEmbedder::IsBool(L, 1))
				notifyNetwork = LuaEmbedder::PullBool(L, 1);
		}


		ComponentHasChanged(notifyNetwork);
		return 0;
	}

	int LuaComponent::GetString(lua_State* L)
	{
		assert(m_dataLocation);
		LuaEmbedder::PushString(L, std::string((char*)m_dataLocation));
		return 1;
	}
	int LuaComponent::SetString(lua_State* L)
	{
		assert(m_dataLocation);
		std::string text = LuaEmbedder::PullString(L, 1);
		for (int i = 0; i < (int)text.size(); i++)
		  m_dataLocation[i] = text[i];
		m_dataLocation[(int)text.size()] = '\0';

		bool notifyNetwork = true;
		if (LuaEmbedder::IsBool(L, 2))
			notifyNetwork = LuaEmbedder::PullBool(L, 2);

		ComponentHasChanged(notifyNetwork);
		return 0;
	}

	int LuaComponent::SetModel(lua_State* L)
	{
		assert(m_dataLocation);
		std::string modelName = LuaEmbedder::PullString(L, 1);
		std::string folderName = LuaEmbedder::PullString(L, 2);
		int renderType = LuaEmbedder::PullInt(L, 3);

		int modelComponentId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Model");
		int modelNameOffset = ECSL::ComponentTypeManager::GetInstance().GetComponentType(modelComponentId)->GetVariables()->at("ModelName").GetOffset();
		int modelPathOffset = ECSL::ComponentTypeManager::GetInstance().GetComponentType(modelComponentId)->GetVariables()->at("ModelPath").GetOffset();
		int modelRenderTypeOffset = ECSL::ComponentTypeManager::GetInstance().GetComponentType(modelComponentId)->GetVariables()->at("RenderType").GetOffset();

		for (int i = 0; i < modelName.size(); i++)
			m_dataLocation[modelNameOffset + i] = modelName[i];
		m_dataLocation[modelNameOffset + modelName.size()] = '\0';

		for (int i = 0; i < folderName.size(); i++)
			m_dataLocation[modelPathOffset + i] = folderName[i];
		m_dataLocation[modelPathOffset + folderName.size()] = '\0';

		*(int*)(m_dataLocation + modelRenderTypeOffset) = renderType;

		bool notifyNetwork = true;
		if (LuaEmbedder::IsBool(L, 4))
			notifyNetwork = LuaEmbedder::PullBool(L, 4);

		ComponentHasChanged(notifyNetwork);
		return 0;
	}
	
	int LuaComponent::SetPointlight(lua_State* L)
	{
		assert(m_dataLocation);
		((float*)m_dataLocation)[0] = LuaEmbedder::PullFloat(L, 1);
		((float*)m_dataLocation)[1] = LuaEmbedder::PullFloat(L, 2);
		((float*)m_dataLocation)[2] = LuaEmbedder::PullFloat(L, 3);
		((float*)m_dataLocation)[3] = LuaEmbedder::PullFloat(L, 4);
		((float*)m_dataLocation)[4] = LuaEmbedder::PullFloat(L, 5);
		((float*)m_dataLocation)[5] = LuaEmbedder::PullFloat(L, 6);
		((float*)m_dataLocation)[6] = LuaEmbedder::PullFloat(L, 7);
		((float*)m_dataLocation)[7] = LuaEmbedder::PullFloat(L, 8);
		((float*)m_dataLocation)[8] = LuaEmbedder::PullFloat(L, 9);
		((float*)m_dataLocation)[9] = LuaEmbedder::PullFloat(L, 10);
		bool notifyNetwork = true;
		if (LuaEmbedder::IsBool(L, 11))
			notifyNetwork = LuaEmbedder::PullBool(L, 11);

		ComponentHasChanged(notifyNetwork);
		return 0;
	}

	int LuaComponent::GetPointlight(lua_State* L)
	{
		assert(m_dataLocation);
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[0]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[1]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[2]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[3]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[4]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[5]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[6]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[7]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[8]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[9]));
		return 10;
	}

	int LuaComponent::GetDirectionalLight(lua_State* L)
	{
		assert(m_dataLocation);
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[0]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[1]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[2]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[3]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[4]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[5]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[6]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[7]));
		LuaEmbedder::PushFloat(L, (float)(((float*)m_dataLocation)[8]));
		return 9;
	}
	int LuaComponent::SetDirectionalLight(lua_State* L)
	{
		assert(m_dataLocation);
		((float*)m_dataLocation)[0] = LuaEmbedder::PullFloat(L, 1);
		((float*)m_dataLocation)[1] = LuaEmbedder::PullFloat(L, 2);
		((float*)m_dataLocation)[2] = LuaEmbedder::PullFloat(L, 3);
		((float*)m_dataLocation)[3] = LuaEmbedder::PullFloat(L, 4);
		((float*)m_dataLocation)[4] = LuaEmbedder::PullFloat(L, 5);
		((float*)m_dataLocation)[5] = LuaEmbedder::PullFloat(L, 6);
		((float*)m_dataLocation)[6] = LuaEmbedder::PullFloat(L, 7);
		((float*)m_dataLocation)[7] = LuaEmbedder::PullFloat(L, 8);
		((float*)m_dataLocation)[8] = LuaEmbedder::PullFloat(L, 9);

		bool notifyNetwork = true;
		if (LuaEmbedder::IsBool(L, 10))
			notifyNetwork = LuaEmbedder::PullBool(L, 10);

		ComponentHasChanged(notifyNetwork);
		return 0;
	}
	
	void LuaComponent::ComponentHasChanged(bool _notifyNetwork)
	{
		if (m_system != nullptr)
			m_system->ComponentHasChanged(m_entityId, m_componentName, _notifyNetwork);
		else if (m_world != nullptr)
			m_world->ComponentHasChanged(m_entityId, m_componentName, _notifyNetwork);
	}
}