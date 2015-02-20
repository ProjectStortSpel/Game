#ifndef CBG_FILE_H
#define CBG_FILE_H

#include <string>
#include <vector>

namespace HomePath
{

	std::string GetHomePath();
	std::string GetGameModePath();

	std::string GetSecondaryHomePath();
	std::string GetSecondaryGameModePath();

	std::string GetDownloadHomePath();
	std::string GetDownloadGameModePath();

	void SetGameMode(std::string _gamemode);

	std::vector<std::string> GetPaths();

	std::vector<std::string> GetGameModePaths();
	std::vector<std::string> GetHomePaths();
}


#endif