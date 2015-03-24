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

		bool EXPORT IsBinary(std::string _path);

		bool EXPORT Create(std::string _path);
		bool EXPORT Append(std::string _path, SDL_RWops** _file);
		bool EXPORT Open(std::string _path, SDL_RWops** _file);

		bool EXPORT Exist(std::string _path);

		bool EXPORT Delete(std::string _path);

		void EXPORT Close(SDL_RWops* _file);

		Sint64 EXPORT GetFileSize(SDL_RWops* _file);
		Sint64 EXPORT GetPosition(SDL_RWops* _file);
		Sint64 EXPORT GetRemainingBytes(SDL_RWops* _file);

		EXPORT char* Read(SDL_RWops* _file, int length = 1);
		std::string EXPORT ReadLine(SDL_RWops* _file);

		void EXPORT Write(SDL_RWops* _file, std::string _text);
		void EXPORT Write(SDL_RWops* _file, const char* data, int size);
		void EXPORT WriteLine(SDL_RWops* _file, std::string _text);

	}
}


#endif