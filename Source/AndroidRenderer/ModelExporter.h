#ifndef MODELEXPORTER_H
#define MODELEXPORTER_H

//#include <string>
#include <vector>
#include <SDL/SDL.h>

class ModelExporter{
public:
	SDL_RWops* fileIn;
	bool OpenFileForRead(const char* fileDir)
	{
		std::string fp = std::string(fileDir);
		fp.erase(std::remove(fp.begin(), fp.end(), 13), fp.end());
		fileIn = SDL_RWFromFile(fp.c_str(), "rb");
		if (fileIn == NULL)
		  return false;
		return true;
	}

	bool OpenFileForWrite(const char* fileDir)
	{
        return false;
	}
	void CloseFile()
	{
		SDL_RWclose(fileIn);
		//file.close();
		//ifile.close();
	}

	void AddDataToFile(std::vector<float> data)
	{
	}

	std::vector<float> ReadDataFromFile()
	{
		std::vector<float> data;
		int size = 0;
		int result = SDL_RWread(fileIn, (char*)&size, sizeof(int), 1);
		if (result != 0)
		{
			data.resize(size / sizeof(float));
			SDL_RWread(fileIn, (char*)&data[0], size, 1);
		}
		return data;
	}
};
#endif