#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <string>
#include <vector>

#if defined(WIN32)
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#else
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#endif

namespace FileSystem
{
    enum Type
    {
        _Directory,
        _File
    };
    
    struct Entry
    {
        std::string name;
        Type type;
    };
    
	namespace Directory
	{
		bool EXPORT Create(std::string _path);

		bool EXPORT Exist(std::string _path);
        
        EXPORT std::vector<Entry> GetEntries(std::string _path);
        EXPORT std::vector<std::string> GetSubDirectories(std::string _path);
        EXPORT std::vector<std::string> GetFiles(std::string _path);
        EXPORT std::vector<std::string> GetAllFiles(std::string _path);
        
	}
}


#endif