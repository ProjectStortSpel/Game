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

	int y = 0;  
	while (std::getline(file, line))
	{
		for (int i = 0; i < line.size(); i++)
			map.push_back((int)line[i]);
		y++;
	}
	file.close();

	int x = 0;
	if ( y != 0 ) 
		 x = map.size() / y;

	LuaEmbedder::PushInt(x);
	LuaEmbedder::PushInt(y);
	LuaEmbedder::PushIntArray(map.data(), map.size());
	return 3;
}