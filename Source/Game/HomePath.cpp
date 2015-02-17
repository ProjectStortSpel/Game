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
		std::string path = "content/gamemodes/";
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
		path.append("gamemodes/");
		path.append(gamemode);
		path.append("/");
		return path;
	}

	void SetGameMode(std::string _gamemode)
	{
		gamemode = _gamemode;
	}

}