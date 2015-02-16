#ifndef CBG_FILE_H
#define CBG_FILE_H

#include <string>

namespace HomePath
{

	std::string GetHomePath();
	std::string GetGameModePath();

	std::string GetSecondaryHomePath();
	std::string GetSecondaryGameModePath();

	void SetGameMode(std::string _gamemode);

}


#endif