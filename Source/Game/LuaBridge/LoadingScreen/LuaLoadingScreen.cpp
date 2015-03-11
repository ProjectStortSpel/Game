#include "LuaLoadingScreen.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "Game/LoadingScreen.h"

namespace LuaBridge
{

	namespace LuaLoadingScreen
	{
		int Remove(lua_State* L);
		int SetAccessLevel(lua_State* L);
		int SetText(lua_State* L);
		int SetBackground(lua_State* L);

		void Embed(lua_State* L)
		{
			LuaEmbedder::AddFunction(L, "Remove", &Remove, "LoadingScreen");
			LuaEmbedder::AddFunction(L, "SetAccessLevel", &SetAccessLevel, "LoadingScreen");
			LuaEmbedder::AddFunction(L, "SetText", &SetText, "LoadingScreen");
			LuaEmbedder::AddFunction(L, "SetBackground", &SetBackground, "LoadingScreen");

		}


		int Remove(lua_State* L)
		{
			LoadingScreen::GetInstance().SetInactive(LuaEmbedder::PullInt(L, 1));
			return 0;
		}

		int SetAccessLevel(lua_State* L)
		{
			LoadingScreen::GetInstance().SetNeedForce(LuaEmbedder::PullInt(L, 1));
			return 0;
		}

		int SetText(lua_State* L)
		{
			LoadingScreen::GetInstance().SetLoadingText(LuaEmbedder::PullString(L, 1));
			return 0;
		}

		//TODO
		int SetBackground(lua_State* L)
		{
			//LuaEmbedder::PullString(L, 1)
			return 0;
		}
		

	}
}
