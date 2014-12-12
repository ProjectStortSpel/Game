#include "LuaComponent.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "ECSL/Framework/Components/ComponentType.h"

namespace LuaBridge
{
	LuaComponent::LuaComponent()
	{
		m_dataLocation = nullptr;
		m_system = nullptr;
	}

	LuaComponent::LuaComponent(ECSL::DataLocation dataLocation, ECSL::System* system,
		unsigned int entityId, const std::string& componentName)
	{
		m_dataLocation = dataLocation;
		m_system = system;
		m_entityId = entityId;
		m_componentName = componentName;
	}

	LuaComponent::~LuaComponent() { }

	void LuaComponent::Embed()
	{
		LuaEmbedder::EmbedClass<LuaComponent>("Component");

		LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "GetFloat", &LuaComponent::GetFloat);
		LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "GetFloat2", &LuaComponent::GetFloat2);
		LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "GetFloat3", &LuaComponent::GetFloat3);
		LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "GetFloat4", &LuaComponent::GetFloat4);
		LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "SetFloat", &LuaComponent::SetFloat);
		LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "SetFloat2", &LuaComponent::SetFloat2);
		LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "SetFloat3", &LuaComponent::SetFloat3);
		LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "SetFloat4", &LuaComponent::SetFloat4);
		LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "GetInt", &LuaComponent::GetInt);
		LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "GetInt2", &LuaComponent::GetInt2);
		LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "GetInt3", &LuaComponent::GetInt3);
		LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "GetInt4", &LuaComponent::GetInt4);
		LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "SetInt", &LuaComponent::SetInt);
		LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "SetInt2", &LuaComponent::SetInt2);
		LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "SetInt3", &LuaComponent::SetInt3);
		LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "SetInt4", &LuaComponent::SetInt4);
		LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "GetBool", &LuaComponent::GetBool);
		LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "SetBool", &LuaComponent::SetBool);
		LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "GetString", &LuaComponent::GetString);
		LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "SetString", &LuaComponent::SetString);
		LuaEmbedder::EmbedClassFunction<LuaComponent>("Component", "SetModel", &LuaComponent::SetModel);

		LuaEmbedder::EmbedClassProperty<LuaComponent>("Component", "Float", &LuaComponent::GetFloat, &LuaComponent::SetFloat);
		LuaEmbedder::EmbedClassProperty<LuaComponent>("Component", "Int", &LuaComponent::GetInt, &LuaComponent::SetInt);
		LuaEmbedder::EmbedClassProperty<LuaComponent>("Component", "Bool", &LuaComponent::GetBool, &LuaComponent::SetBool);
		LuaEmbedder::EmbedClassProperty<LuaComponent>("Component", "String", &LuaComponent::GetString, &LuaComponent::SetString);
	}

	int LuaComponent::GetFloat()
	{
		assert(m_dataLocation);
		if (LuaEmbedder::IsInt(1))
			LuaEmbedder::PushFloat((float)(((float*)m_dataLocation)[LuaEmbedder::PullInt(1)]));
		else
			LuaEmbedder::PushFloat((float)((float*)m_dataLocation)[0]);
		return 1;
	}
	int LuaComponent::GetFloat2()
	{
		assert(m_dataLocation);
		LuaEmbedder::PushFloat((float)(((float*)m_dataLocation)[0]));
		LuaEmbedder::PushFloat((float)(((float*)m_dataLocation)[1]));
		return 2;
	}
	int LuaComponent::GetFloat3()
	{
		assert(m_dataLocation);
		LuaEmbedder::PushFloat((float)(((float*)m_dataLocation)[0]));
		LuaEmbedder::PushFloat((float)(((float*)m_dataLocation)[1]));
		LuaEmbedder::PushFloat((float)(((float*)m_dataLocation)[2]));
		return 3;
	}
	int LuaComponent::GetFloat4()
	{
		assert(m_dataLocation);
		LuaEmbedder::PushFloat((float)(((float*)m_dataLocation)[0]));
		LuaEmbedder::PushFloat((float)(((float*)m_dataLocation)[1]));
		LuaEmbedder::PushFloat((float)(((float*)m_dataLocation)[2]));
		LuaEmbedder::PushFloat((float)(((float*)m_dataLocation)[3]));
		return 4;
	}
	int LuaComponent::SetFloat()
	{
		assert(m_dataLocation);
		if (LuaEmbedder::IsInt(2))
			((float*)m_dataLocation)[LuaEmbedder::PullInt(2)] = LuaEmbedder::PullFloat(1);
		else
			((float*)m_dataLocation)[0] = LuaEmbedder::PullFloat(1);
		m_system->ComponentHasChanged(m_entityId, m_componentName);
		return 0;
	}
	int LuaComponent::SetFloat2()
	{
		assert(m_dataLocation);
		((float*)m_dataLocation)[0] = LuaEmbedder::PullFloat(1);
		((float*)m_dataLocation)[1] = LuaEmbedder::PullFloat(2);
		m_system->ComponentHasChanged(m_entityId, m_componentName);
		return 0;
	}
	int LuaComponent::SetFloat3()
	{
		assert(m_dataLocation);
		((float*)m_dataLocation)[0] = LuaEmbedder::PullFloat(1);
		((float*)m_dataLocation)[1] = LuaEmbedder::PullFloat(2);
		((float*)m_dataLocation)[2] = LuaEmbedder::PullFloat(3);
		m_system->ComponentHasChanged(m_entityId, m_componentName);
		return 0;
	}
	int LuaComponent::SetFloat4()
	{
		assert(m_dataLocation);
		((float*)m_dataLocation)[0] = LuaEmbedder::PullFloat(1);
		((float*)m_dataLocation)[1] = LuaEmbedder::PullFloat(2);
		((float*)m_dataLocation)[2] = LuaEmbedder::PullFloat(3);
		((float*)m_dataLocation)[3] = LuaEmbedder::PullFloat(4);
		m_system->ComponentHasChanged(m_entityId, m_componentName);
		return 0;
	}

	int LuaComponent::GetInt()
	{
		assert(m_dataLocation);
		if (LuaEmbedder::IsInt(1))
			LuaEmbedder::PushInt((int)(((int*)m_dataLocation)[LuaEmbedder::PullInt(1)]));
		else
			LuaEmbedder::PushInt((int)((int*)m_dataLocation)[0]);
		return 1;
	}
	int LuaComponent::GetInt2()
	{
		assert(m_dataLocation);
		LuaEmbedder::PushInt((int)(((int*)m_dataLocation)[0]));
		LuaEmbedder::PushInt((int)(((int*)m_dataLocation)[1]));
		return 2;
	}
	int LuaComponent::GetInt3()
	{
		assert(m_dataLocation);
		LuaEmbedder::PushInt((int)(((int*)m_dataLocation)[0]));
		LuaEmbedder::PushInt((int)(((int*)m_dataLocation)[1]));
		LuaEmbedder::PushInt((int)(((int*)m_dataLocation)[2]));
		return 3;
	}
	int LuaComponent::GetInt4()
	{
		assert(m_dataLocation);
		LuaEmbedder::PushInt((int)(((int*)m_dataLocation)[0]));
		LuaEmbedder::PushInt((int)(((int*)m_dataLocation)[1]));
		LuaEmbedder::PushInt((int)(((int*)m_dataLocation)[2]));
		LuaEmbedder::PushInt((int)(((int*)m_dataLocation)[3]));
		return 4;
	}
	int LuaComponent::SetInt()
	{
		assert(m_dataLocation);
		if (LuaEmbedder::IsInt(2))
			((int*)m_dataLocation)[LuaEmbedder::PullInt(2)] = LuaEmbedder::PullInt(1);
		else
			((int*)m_dataLocation)[0] = LuaEmbedder::PullInt(1);
		m_system->ComponentHasChanged(m_entityId, m_componentName);
		return 0;
	}
	int LuaComponent::SetInt2()
	{
		assert(m_dataLocation);
		((int*)m_dataLocation)[0] = LuaEmbedder::PullInt(1);
		((int*)m_dataLocation)[1] = LuaEmbedder::PullInt(2);
		m_system->ComponentHasChanged(m_entityId, m_componentName);
		return 0;
	}
	int LuaComponent::SetInt3()
	{
		assert(m_dataLocation);
		((int*)m_dataLocation)[0] = LuaEmbedder::PullInt(1);
		((int*)m_dataLocation)[1] = LuaEmbedder::PullInt(2);
		((int*)m_dataLocation)[2] = LuaEmbedder::PullInt(3);
		m_system->ComponentHasChanged(m_entityId, m_componentName);
		return 0;
	}
	int LuaComponent::SetInt4()
	{
		assert(m_dataLocation);
		((int*)m_dataLocation)[0] = LuaEmbedder::PullInt(1);
		((int*)m_dataLocation)[1] = LuaEmbedder::PullInt(2);
		((int*)m_dataLocation)[2] = LuaEmbedder::PullInt(3);
		((int*)m_dataLocation)[3] = LuaEmbedder::PullInt(4);
		m_system->ComponentHasChanged(m_entityId, m_componentName);
		return 0;
	}

	int LuaComponent::GetBool()
	{
		assert(m_dataLocation);
		if (LuaEmbedder::IsInt(1))
			LuaEmbedder::PushBool((bool)(((bool*)m_dataLocation)[LuaEmbedder::PullInt(1)]));
		else
			LuaEmbedder::PushBool((bool)((bool*)m_dataLocation)[0]);
		return 1;
	}
	int LuaComponent::SetBool()
	{
		assert(m_dataLocation);
		if (LuaEmbedder::IsInt(2))
			((bool*)m_dataLocation)[LuaEmbedder::PullInt(2)] = LuaEmbedder::PullBool(1);
		else
			((bool*)m_dataLocation)[0] = LuaEmbedder::PullBool(1);
		m_system->ComponentHasChanged(m_entityId, m_componentName);
		return 0;
	}

	int LuaComponent::GetString()
	{
		assert(m_dataLocation);
		LuaEmbedder::PushString(std::string((char*)m_dataLocation));
		return 1;
	}
	int LuaComponent::SetString()
	{
		assert(m_dataLocation);
		m_dataLocation = (char*)LuaEmbedder::PullString(1).c_str();
		m_system->ComponentHasChanged(m_entityId, m_componentName);
		return 0;
	}

	int LuaComponent::SetModel()
	{
		assert(m_dataLocation);
		std::string modelName = LuaEmbedder::PullString(1);
		std::string folderName = LuaEmbedder::PullString(2);
		for (int i = 0; i < modelName.size(); i++)
			m_dataLocation[i] = modelName[i];
		m_dataLocation[modelName.size()] = '\0';
		for (int i = 0; i < folderName.size(); i++)
			m_dataLocation[i + CHARSIZE] = folderName[i];
		m_dataLocation[folderName.size() + CHARSIZE] = '\0';
		m_system->ComponentHasChanged(m_entityId, m_componentName);
		return 0;
	}
}