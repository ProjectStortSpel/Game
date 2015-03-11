#include "LuaLoadingScreen.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "Game/LoadingScreen.h"
#include "Game/HomePath.h"

namespace LuaBridge
{

	namespace LuaLoadingScreen
	{
		int Remove(lua_State* L);
		int SetAccessLevel(lua_State* L);
		int SetText(lua_State* L);
		int SetTextColor(lua_State* L);
		int SetBackground(lua_State* L);

		void Embed(lua_State* L)
		{
			LuaEmbedder::AddFunction(L, "Remove", &Remove, "LoadingScreen");
			LuaEmbedder::AddFunction(L, "SetAccessLevel", &SetAccessLevel, "LoadingScreen");
			LuaEmbedder::AddFunction(L, "SetText", &SetText, "LoadingScreen");
			LuaEmbedder::AddFunction(L, "SetTextColor", &SetTextColor, "LoadingScreen");
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

		int SetTextColor(lua_State* L)
		{
			LoadingScreen::GetInstance().SetTextColor(LuaEmbedder::PullFloat(L, 1), LuaEmbedder::PullFloat(L, 2), LuaEmbedder::PullFloat(L, 3));
			return 0;
		}

		int SetBackground(lua_State* L)
		{
			std::vector<std::string> paths;

			if (LuaEmbedder::PullBool(L, "Client"))
				paths = HomePath::GetPaths(HomePath::Type::Client);
			else if (LuaEmbedder::PullBool(L, "Server"))
				paths = HomePath::GetPaths(HomePath::Type::Server);

			LoadingScreen::GetInstance().SetBackground(paths, LuaEmbedder::PullString(L, 1));
			return 0;
		}
		

	}
}
