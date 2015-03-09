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

	std::vector<std::string> stringMap;
	for (int i = 0; i < x; i++)
	{
		std::string line;
		for (int j = 0; j < y; j++)
		{
			line.push_back((char)map[i * y + j]);
		}
		stringMap.push_back(line);
	}

	if (g_meshCreatorIndex != -1)
		LuaBridge::LuaGraphicDevice::GetGraphicDevice()->RemoveModel(g_meshCreatorIndex);

	MeshCreator meshcreator;
	meshcreator.CreateMesh(stringMap);
	Renderer::ModelToLoadFromSource model;
	model.key = "meshcreator";
	model.positions = meshcreator.position;
	model.normals = meshcreator.normals;
	model.tangents = meshcreator.tangents;
	model.bitangents = meshcreator.bitangents;
	model.texCoords = meshcreator.uvs;
	model.diffuseTextureFilepath = "content/textures/dirt.png";
	model.normalTextureFilepath = "content/textures/normalPixel.png";
	model.specularTextureFilepath = "content/textures/blackPixel.png";
	model.RenderType = 0;
	model.Color = g_meshCreatorColor;
	model.MatrixPtr = &g_meshCreatorMatrix;
	model.CastShadow = false;
	g_meshCreatorIndex = LuaBridge::LuaGraphicDevice::GetGraphicDevice()->LoadModel(&model);

	return 3;
}

int GenerateIslandMesh(lua_State* L)  
{
	int			sizeX		=	LuaEmbedder::PullInt(L, 1);
	int			sizeZ		=	LuaEmbedder::PullInt(L, 2);
	std::string	stringMap	=	LuaEmbedder::PullString(L, 3);

	std::vector<std::string>	strVector	=	std::vector<std::string>();

	for( int Z = 0; Z < sizeZ; ++Z)
	{
		std::string	line;
		for( int X = 0; X < sizeX; ++X)
			line.push_back((char)stringMap[Z*sizeX + X]);	

		strVector.push_back(line);
	}

	if (g_meshCreatorIndex != -1)
		LuaBridge::LuaGraphicDevice::GetGraphicDevice()->RemoveModel(g_meshCreatorIndex);

	MeshCreator meshcreator;
	meshcreator.CreateMesh(strVector);
	Renderer::ModelToLoadFromSource model;
	model.key = "meshcreator";
	model.positions = meshcreator.position;
	model.normals = meshcreator.normals;
	model.tangents = meshcreator.tangents;
	model.bitangents = meshcreator.bitangents;
	model.texCoords = meshcreator.uvs;
	model.diffuseTextureFilepath = "content/textures/dirt.png";
	model.normalTextureFilepath = "content/textures/normalPixel.png";
	model.specularTextureFilepath = "content/textures/blackPixel.png";
	model.RenderType = 0;
	model.Color = g_meshCreatorColor;
	model.MatrixPtr = &g_meshCreatorMatrix;
	model.CastShadow = false;
	g_meshCreatorIndex = LuaBridge::LuaGraphicDevice::GetGraphicDevice()->LoadModel(&model);

	return 3;
}