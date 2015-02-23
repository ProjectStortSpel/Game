#ifndef CBG_FILE_H
#define CBG_FILE_H

#include <string>
#include <vector>

namespace HomePath
{

	std::string GetHomePath();
	std::string GetGameModePath();
	std::string GetGameModePath(std::string _gamemode);

	std::string GetSecondaryHomePath();
	std::string GetSecondaryGameModePath();
	std::string GetSecondaryGameModePath(std::string _gamemode);

	std::string GetDownloadHomePath();
	std::string GetDownloadGameModePath();
	std::string GetDownloadGameModePath(std::string _gamemode);

	void SetGameMode(std::string _gamemode);

	enum Type
	{
		Server, Client
	};

	std::vector<std::string> GetPaths(Type _type);
	std::vector<std::string> GetPaths(Type _type, std::string _gamemode);

	std::vector<std::string> GetGameModePaths(Type _type);
	std::vector<std::string> GetGameModePaths(Type _type, std::string _gamemode);
	std::vector<std::string> GetHomePaths(Type _type);
}


#endif