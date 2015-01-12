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
		fileIn = SDL_RWFromFile(fileDir, "rb");
		if (fileIn == NULL)
		  return false;
		return true;
	}

	bool OpenFileForWrite(const char* fileDir)
	{
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
		int size;
		SDL_RWread(fileIn, (char*)&size, sizeof(int), 1);
		std::vector<float> data(size / sizeof(float));
		SDL_RWread(fileIn, (char*)&data[0], size, 1);
		//int size;
		//ifile.read((char*)&size, sizeof(int));
		//std::vector<float> data(size / sizeof(float));
		///ifile.read((char*)&data[0], size);

		return data;
	}
};
#endif