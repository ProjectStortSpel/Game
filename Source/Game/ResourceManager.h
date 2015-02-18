#include <string>
#include <vector>

namespace ResourceManager
{
	struct Resource
	{
		std::string File;
		std::string Location;
		std::string MD5;
		int Size;
	};

	int AddGamemodeResource(std::string _resource);
	int AddContentResource(std::string _resource);

	std::vector<Resource>* GetGamemodeResources();
	std::vector<Resource>* GetContentResources();

	void Clear();
}