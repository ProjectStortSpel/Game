#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <string>

#if defined(WIN32)
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#else
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#endif

namespace FileSystem
{
	namespace Directory
	{
		bool EXPORT CreateFolder(std::string _path);

		bool EXPORT DirectoryExist(std::string _path);
	}
}


#endif