#include "LuaFileLoader.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include <fstream>
#include <string>
#include <SDL/SDL.h>
#include <Game/MeshCreator/MeshCreator.h>
#include <Game/LuaBridge/Renderer/LuaGraphicDevice.h>

int g_meshCreatorIndex = -1;
float g_meshCreatorColor[3] = { 0.0f, 0.0f, 0.0f };
glm::mat4 g_meshCreatorMatrix = glm::translate(glm::vec3(0.0f, 0.15f, 0.0f));

int LoadMap(lua_State* L)  
{
	std::string filepath = LuaEmbedder::PullString(L, 1);
	
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
	char* data = new char[length];
	SDL_RWread(file, data, length, 1);
	// Close file
	SDL_RWclose(file);
	
	std::string source = std::string(data, length);
	//printf(source.c_str());
	delete data;
	std::string line;
	size_t prevFileIndex = 0, nextFileIndex = std::string::npos;

	std::vector<int> map;

	int x = 0;
	int y = 0;  
	while (true)
	{
		nextFileIndex = source.find('\n', prevFileIndex);
		if (nextFileIndex == std::string::npos)
			nextFileIndex = source.size() - 1;
		line = source.substr(prevFileIndex, nextFileIndex - prevFileIndex);
		line.erase(std::remove(line.begin(), line.end(), 13), line.end());
		prevFileIndex = nextFileIndex + 1;
		if (prevFileIndex >= source.size())
			prevFileIndex = std::string::npos;
	  
		if (x < line.size())
		{
			x = line.size();
			std::cout << x << std::endl;
		}
		for (int i = 0; i < line.size(); i++)
			map.push_back((int)line[i]);
		y++;

		if (prevFileIndex == std::string::npos)
			break;
	}

	int* array = new int[map.size()];
	memcpy(array, map.data(), map.size() * sizeof(int));

	LuaEmbedder::PushInt(L, x);
	LuaEmbedder::PushInt(L, y);
	LuaEmbedder::PushIntArray(L, array, map.size());

	return 3;
}