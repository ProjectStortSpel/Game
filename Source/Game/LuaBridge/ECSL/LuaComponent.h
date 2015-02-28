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

		LuaComponent(ECSL::DataLocation dataLocation, ECSL::World* world,
			unsigned int entityId, const std::string& componentName, const std::string& variableName);
		LuaComponent(ECSL::DataLocation dataLocation, ECSL::World* world,
			unsigned int entityId, const std::string& componentName, unsigned int index);
		~LuaComponent();

		static void Embed(lua_State* L);

	private:
		int GetFloat(lua_State* L);
		int GetFloat2(lua_State* L);
		int GetFloat3(lua_State* L);
		int GetFloat4(lua_State* L);
		int GetFloat5(lua_State* L);
		int GetFloat6(lua_State* L);
		int GetFloat7(lua_State* L);
		int GetFloat8(lua_State* L);
		int SetFloat(lua_State* L);
		int SetFloat2(lua_State* L);
		int SetFloat3(lua_State* L);
		int SetFloat4(lua_State* L);
		int SetFloat5(lua_State* L);
		int SetFloat6(lua_State* L);
		int SetFloat7(lua_State* L);
		int SetFloat8(lua_State* L);


		int GetInt(lua_State* L);
		int GetInt2(lua_State* L);
		int GetInt3(lua_State* L);
		int GetInt4(lua_State* L);
		int GetInt5(lua_State* L);
		int GetInt6(lua_State* L);
		int SetInt(lua_State* L);
		int SetInt2(lua_State* L);
		int SetInt3(lua_State* L);
		int SetInt4(lua_State* L);
		int SetInt5(lua_State* L);
		int SetInt6(lua_State* L);

		int GetBool(lua_State* L);
		int SetBool(lua_State* L);

		int GetText(lua_State* L);
		int SetText(lua_State* L);

		int GetString(lua_State* L);
		int SetString(lua_State* L);


		int SetModel(lua_State* L);
		
		int GetPointlight(lua_State* L);
		int SetPointlight(lua_State* L);

		int GetDirectionalLight(lua_State* L);
		int SetDirectionalLight(lua_State* L);

		
		
		void ComponentHasChanged(bool _notifyNetwork);
		
	private:
		ECSL::DataLocation m_dataLocation;
		ECSL::World* m_world;
		unsigned int m_entityId;
		std::string m_componentName;
		unsigned int m_offset;
	};
}

#endif