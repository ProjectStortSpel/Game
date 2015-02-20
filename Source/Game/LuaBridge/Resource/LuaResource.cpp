#include "LuaResource.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "Game/ResourceManager.h"

namespace LuaBridge
{
	namespace LuaResource
	{
		lua_State* ServerLuaState;

		int Add(lua_State* L);


		void SetServerState(lua_State* L)
		{
			ServerLuaState = L;
		}

		void Embed(lua_State* L)
		{
			LuaEmbedder::AddFunction(L, "Add", &Add, "Resource");
		}

		int Add(lua_State* L)
		{
			lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
			if (ServerLuaState != parent)
			{
				LuaEmbedder::PushInt(L, 0);
				return 1;
			}

			std::string file = LuaEmbedder::PullString(L, 1);
			
			LuaEmbedder::PushInt(L, ResourceManager::AddContentResource(file));
			return 1;
		}

	}
}