#ifndef LUACOMPONENT_H
#define LUACOMPONENT_H

#include "ECSL/Framework/Components/Tables/DataTable.h"
#include "ECSL/Interfaces/System.h"
#include "ECSL/Framework/World.h"
#include <LuaEmbedder/LuaEmbedder.h>

#include <string>
#include <map>

namespace LuaBridge
{
	class LuaComponent
	{
	public:
		LuaComponent(lua_State* L);
		LuaComponent(ECSL::DataLocation dataLocation, ECSL::System* system,
			unsigned int entityId, const std::string& componentName);
		LuaComponent(ECSL::DataLocation dataLocation, ECSL::World* world,
			unsigned int entityId, const std::string& componentName);
		~LuaComponent();

		static void Embed(lua_State* L);

	private:
		int GetFloat(lua_State* L);
		int GetFloat2(lua_State* L);
		int GetFloat3(lua_State* L);
		int GetFloat4(lua_State* L);
		int GetFloat5(lua_State* L);
		int SetFloat(lua_State* L);
		int SetFloat2(lua_State* L);
		int SetFloat3(lua_State* L);
		int SetFloat4(lua_State* L);
		int SetFloat5(lua_State* L);


		int GetInt(lua_State* L);
		int GetInt2(lua_State* L);
		int GetInt3(lua_State* L);
		int GetInt4(lua_State* L);
		int SetInt(lua_State* L);
		int SetInt2(lua_State* L);
		int SetInt3(lua_State* L);
		int SetInt4(lua_State* L);

		int GetBool(lua_State* L);
		int SetBool(lua_State* L);

		int GetString(lua_State* L);
		int SetString(lua_State* L);

		int SetModel(lua_State* L);
		
		int GetPointlight(lua_State* L);
		int SetPointlight(lua_State* L);
		
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