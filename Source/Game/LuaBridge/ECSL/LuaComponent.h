#ifndef LUACOMPONENT_H
#define LUACOMPONENT_H

#include "ECSL/Framework/Components/Tables/DataTable.h"
#include "ECSL/Interfaces/System.h"
#include "ECSL/Framework/World.h"

#include <string>
#include <map>

namespace LuaBridge
{
	class LuaComponent
	{
	public:
		LuaComponent();
		LuaComponent(ECSL::DataLocation dataLocation, ECSL::System* system,
			unsigned int entityId, const std::string& componentName);
		LuaComponent(ECSL::DataLocation dataLocation, ECSL::World* world,
			unsigned int entityId, const std::string& componentName);
		~LuaComponent();

		static void Embed();

	private:
		int GetFloat();
		int GetFloat2();
		int GetFloat3();
		int GetFloat4();
		int GetFloat5();
		int SetFloat();
		int SetFloat2();
		int SetFloat3();
		int SetFloat4();
		int SetFloat5();


		int GetInt();
		int GetInt2();
		int GetInt3();
		int GetInt4();
		int SetInt();
		int SetInt2();
		int SetInt3();
		int SetInt4();

		int GetBool();
		int SetBool();

		int GetString();
		int SetString();

		int SetModel();
		
		int GetPointlight();
		int SetPointlight();

		int GetDirectionalLight();
		int SetDirectionalLight();
		
		void ComponentHasChanged();
		
	private:
		ECSL::DataLocation m_dataLocation;
		ECSL::System* m_system;
		ECSL::World* m_world;
		unsigned int m_entityId;
		std::string m_componentName;
	};
}

#endif