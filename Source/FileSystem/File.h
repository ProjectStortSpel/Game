#ifndef FILE_H
#define FILE_H

#include <string>
#include <SDL/SDL.h>

#if defined(WIN32)
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#else
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#endif

namespace FileSystem
{
	namespace File
	{

		bool EXPORT Create(std::string _path);
		bool EXPORT Append(std::string _path, SDL_RWops** _file);
		bool EXPORT Open(std::string _path, SDL_RWops** _file);

		bool EXPORT Delete(std::string _path);

		void EXPORT Close(SDL_RWops* _file);

		std::string EXPORT Read(SDL_RWops* _file, int length = 1);
		std::string EXPORT ReadLine(SDL_RWops* _file);

		void EXPORT Write(SDL_RWops* _file, std::string _text);
		void EXPORT WriteLine(SDL_RWops* _file, std::string _text);

	}
}


#endif