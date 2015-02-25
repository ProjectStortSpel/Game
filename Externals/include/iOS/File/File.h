#ifndef FILE_H
#define FILE_H

#include <string>
#include <SDL/SDL.h>

namespace File
{
	char* Read(const std::string& filepath, unsigned int* size = nullptr)
	{
		// Open file
		SDL_RWops* file = SDL_RWFromFile(filepath.c_str(), "r");
		if (file == NULL)
		{
			SDL_Log("File %s not found", filepath.c_str());
			return nullptr;
		}
		// Get file length
		Sint64 length = SDL_RWseek(file, 0, RW_SEEK_END);
		if (length <= 0)
		{
			SDL_Log("Length of file %s lower than or equal to zero", filepath.c_str());
			return nullptr;
		}
		SDL_RWseek(file, 0, RW_SEEK_SET);
		// Read data
		char* data = new char[length + 1];
		SDL_RWread(file, data, length, 1);
		data[length] = '\0';
		// Close file
		SDL_RWclose(file);
		// Set size
		if (size != nullptr)
			*size = (unsigned int)length;
		return data;
	}
}

#endif