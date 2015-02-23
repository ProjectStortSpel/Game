#include <string>
#include <map>
#include "FileSystem/MD5.h"
#include "Game/HomePath.h"

namespace ResourceManager
{
	struct Resource
	{
		std::string File;
		std::string Location;
		FileSystem::MD5::MD5Data MD5;
		int Size;
	};

	bool CreateResource(std::string _path, Resource &_r, HomePath::Type _type);
	bool CreateResource(std::string _gamemode, std::string _path, Resource &_r, HomePath::Type _type);

	int AddGamemodeResource(std::string _resource);
	int AddContentResource(std::string _resource);

	std::map<std::string, Resource>* GetGamemodeResources();
	std::map<std::string, Resource>* GetContentResources();

	void Clear();
}