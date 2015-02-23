#include <string>
#include <vector>
#include "FileSystem/MD5.h"

namespace ResourceManager
{
	struct Resource
	{
		std::string File;
		std::string Location;
		FileSystem::MD5::MD5Data MD5;
		int Size;
	};

	bool CreateResource(std::string _path, Resource &_r);

	int AddGamemodeResource(std::string _resource);
	int AddContentResource(std::string _resource);

	std::vector<Resource>* GetGamemodeResources();
	std::vector<Resource>* GetContentResources();

	void Clear();
}