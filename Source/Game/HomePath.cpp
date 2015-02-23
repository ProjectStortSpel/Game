#include "Game/HomePath.h"
#include <cstdlib>

#ifdef __ANDROID__
#include <SDL/SDL.h>
#endif

namespace HomePath
{

	std::string gamemode;

	std::string GetHomePath()
	{
		return "content/";
	}

	std::string GetGameModePath()
	{
		//std::string path = "content/gamemodes/";
		std::string path = "content/scripting/";
		path.append(gamemode);
		path.append("/");
		return path;
	}

	std::string GetSecondaryHomePath()
	{
		static std::string path = "";

		if (path == "")
		{
			#if  defined(__IOS__)
				path = getenv("HOME");
				path.append("/Library/Caches/");
			#elif defined(__ANDROID__)
                path = SDL_AndroidGetInternalStoragePath();
				path.append("/Library/");
			#elif defined(__OSX__)
				path = getenv("HOME");
				path.append("/Library/Preferences/StoraSpel/");
			#else
				path = "content/";
			#endif
		}

		return path;
	}

	std::string GetSecondaryGameModePath()
	{
		std::string path = GetSecondaryHomePath();
		//path.append("gamemodes/");
		path.append("scripting/");
		path.append(gamemode);
		path.append("/");
		return path;
	}

	std::string GetDownloadHomePath()
	{
		std::string path = GetSecondaryHomePath();
		path.append("downloads/");
		return path;
	}

	std::string GetDownloadGameModePath()
	{
		std::string path = GetSecondaryHomePath();
		//path.append("downloads/gamemodes/");
		path.append("downloads/scripting/");
		path.append(gamemode);
		path.append("/");
		return path;
	}

	void SetGameMode(std::string _gamemode)
	{
		gamemode = _gamemode;
	}

	std::vector<std::string> GetPaths()
	{
		std::vector<std::string> paths;
		//Gamemode paths
		paths.push_back(GetGameModePath());
		paths.push_back(GetSecondaryGameModePath());
		paths.push_back(GetDownloadGameModePath());

		//Homepaths
		paths.push_back(GetHomePath());
		paths.push_back(GetSecondaryHomePath());
		paths.push_back(GetDownloadHomePath());
		return paths;
	}

	std::vector<std::string> GetGameModePaths()
	{
		std::vector<std::string> paths;
		//Gamemode paths
		paths.push_back(GetGameModePath());
		paths.push_back(GetSecondaryGameModePath());
		paths.push_back(GetDownloadGameModePath());
		return paths;
	}

	std::vector<std::string> GetHomePaths()
	{
		std::vector<std::string> paths;
		//Homepaths
		paths.push_back(GetHomePath());
		paths.push_back(GetSecondaryHomePath());
		paths.push_back(GetDownloadHomePath());
		return paths;
	}
}