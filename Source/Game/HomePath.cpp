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
#if defined(_DEBUG) && !defined(__ANDROID__) && !defined(__APPLE__)
		return "../../../Externals/content/";
#endif
		return "content/";
	}

	std::string GetGameModePath()
	{
		return GetGameModePath(gamemode);
	}

	std::string GetGameModePath(std::string _gamemode)
	{
		//std::string path = "content/gamemodes/";

#if defined(_DEBUG) && !defined(__ANDROID__) && !defined(__APPLE__)
		std::string path = "../../../Externals/content/scripting/";
#else
		std::string path = "content/scripting/";
#endif
		path.append(_gamemode);
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
		return GetSecondaryGameModePath(gamemode);
	}

	std::string GetSecondaryGameModePath(std::string _gamemode)
	{
		std::string path = GetSecondaryHomePath();
		//path.append("gamemodes/");
		path.append("scripting/");
		path.append(_gamemode);
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
		return GetDownloadGameModePath(gamemode);
	}

	std::string GetDownloadGameModePath(std::string _gamemode)
	{
		std::string path = GetDownloadHomePath();
		//path.append("downloads/gamemodes/");
		path.append("scripting/");
		path.append(_gamemode);
		path.append("/");
		return path;
	}

	void SetGameMode(std::string _gamemode)
	{
		gamemode = _gamemode;
	}

	std::vector<std::string> GetPaths(Type _type)
	{
		return GetPaths(_type, gamemode);
	}

	std::vector<std::string> GetPaths(Type _type, std::string _gamemode)
	{
		std::vector<std::string> paths;
		if (_type == Server)
		{
			//Gamemode paths
#if defined(_DEBUG) && !defined(__ANDROID__) && !defined(__APPLE__)
			paths.push_back(GetGameModePath(_gamemode));
			paths.push_back(GetSecondaryGameModePath(_gamemode));
#else
			paths.push_back(GetSecondaryGameModePath(_gamemode));
			paths.push_back(GetGameModePath(_gamemode));
#endif
			paths.push_back(GetDownloadGameModePath(_gamemode));

			//Homepaths
#if defined(_DEBUG) && !defined(__ANDROID__) && !defined(__APPLE__)
			paths.push_back(GetHomePath());
			paths.push_back(GetSecondaryHomePath());
#else
			paths.push_back(GetSecondaryHomePath());
			paths.push_back(GetHomePath());
#endif
			paths.push_back(GetDownloadHomePath());
		}
		else if (_type == Client)
		{
			//Gamemode paths
			paths.push_back(GetDownloadGameModePath(_gamemode));
			paths.push_back(GetGameModePath(_gamemode));
			paths.push_back(GetSecondaryGameModePath(_gamemode));

			//Homepaths
			paths.push_back(GetDownloadHomePath());
			paths.push_back(GetHomePath());
			paths.push_back(GetSecondaryHomePath());
		}
		return paths;
	}

	std::vector<std::string> GetGameModePaths(Type _type)
	{
		return GetGameModePaths(_type, gamemode);
	}

	std::vector<std::string> GetGameModePaths(Type _type, std::string _gamemode)
	{
		std::vector<std::string> paths;
		if (_type == Server)
		{
			//Gamemode paths
#if defined(_DEBUG) && !defined(__ANDROID__) && !defined(__APPLE__)
			paths.push_back(GetGameModePath(_gamemode));
			paths.push_back(GetSecondaryGameModePath(_gamemode));
#else
			paths.push_back(GetSecondaryGameModePath(_gamemode));
			paths.push_back(GetGameModePath(_gamemode));
#endif
			paths.push_back(GetDownloadGameModePath(_gamemode));
		}
		else if (_type == Client)
		{
			//Gamemode paths
			paths.push_back(GetDownloadGameModePath(_gamemode));
			paths.push_back(GetGameModePath(_gamemode));
			paths.push_back(GetSecondaryGameModePath(_gamemode));
		}
		return paths;
	}

	std::vector<std::string> GetHomePaths(Type _type)
	{
		std::vector<std::string> paths;
		if (_type == Server)
		{
			//Homepaths
#if defined(_DEBUG) && !defined(__ANDROID__) && !defined(__APPLE__)
			paths.push_back(GetHomePath());
			paths.push_back(GetSecondaryHomePath());
#else
			paths.push_back(GetSecondaryHomePath());
			paths.push_back(GetHomePath());
#endif
			paths.push_back(GetDownloadHomePath());
		}
		else if (_type == Client)
		{
			//Homepaths
			paths.push_back(GetDownloadHomePath());
			paths.push_back(GetHomePath());
			paths.push_back(GetSecondaryHomePath());
		}
		return paths;
	}
}