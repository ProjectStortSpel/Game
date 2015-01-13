#include "LuaFileLoader.h"
#include "LuaEmbedder/LuaEmbedder.h"

#include <fstream>
#include <string>
#include <SDL/SDL.h>

int LoadMap()  
{
	std::string filepath = LuaEmbedder::PullString(1);
	
	// Open file
	SDL_RWops* file = SDL_RWFromFile(filepath.c_str(), "r");
	if (file == NULL)
		SDL_Log("File %s not found", filepath.c_str());
	// Get file length
	Sint64 length = SDL_RWseek(file, 0, RW_SEEK_END);
	if (length <= 0)
		SDL_Log("Length of file %s lower than or equal to zero", filepath.c_str());
	SDL_RWseek(file, 0, RW_SEEK_SET);
	// Read data
	char* data = new char[length + 1];
	SDL_RWread(file, data, length, 1);
	data[length] = '\0';
	// Close file
	SDL_RWclose(file);
	
	std::string source = std::string(data);
	delete data;
	std::string line;
	size_t prevFileIndex = 0, nextFileIndex = std::string::npos;

	std::vector<int> map;
	
	int y = 0;  
	while (true)
	{
		nextFileIndex = source.find('\n', prevFileIndex);
		if (nextFileIndex == std::string::npos)
		  break;
		line = source.substr(prevFileIndex, nextFileIndex - prevFileIndex);
		line.erase(std::remove(line.begin(), line.end(), 13), line.end());
		prevFileIndex = nextFileIndex + 1;
	  
		for (int i = 0; i < line.size(); i++)
			map.push_back((int)line[i]);
		y++;
	}

	int x = 0;
	if ( y != 0 ) 
		 x = map.size() / y;
	
	int* array = new int[map.size()];
	memcpy(array, map.data(), map.size() * sizeof(int));

	LuaEmbedder::PushInt(x);
	LuaEmbedder::PushInt(y);
	LuaEmbedder::PushIntArray(array, map.size());
	return 3;
}