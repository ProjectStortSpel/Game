#include "LuaFileLoader.h"
#include "LuaEmbedder/LuaEmbedder.h"

#include <fstream>
#include <string>

int LoadMap()  
{
	std::string filepath = LuaEmbedder::PullString(1);
	std::ifstream file(filepath);
	std::string line;

	std::vector<int> map;

	int x = 0;
	int y = 0;  
	while (std::getline(file, line))
	{
		if (x < line.size())
		{
			x = line.size();
			std::cout << x << std::endl;
		}
		for (int i = 0; i < line.size(); i++)
			map.push_back((int)line[i]);
		y++;
	}
	file.close();

	int* array = new int[map.size()];
	memcpy(array, map.data(), map.size() * sizeof(int));

	LuaEmbedder::PushInt(x);
	LuaEmbedder::PushInt(y);
	LuaEmbedder::PushIntArray(array, map.size());
	return 3;
}