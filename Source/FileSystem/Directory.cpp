#include "Directory.h"
#include <cstring>

#if defined(WIN32)
#include <sys/types.h>
#include <sys/stat.h>
#include<windows.h>
#elif defined(__IOS__) || defined(__OSX__)
#include <sys/types.h>
#include <sys/stat.h>
#elif defined(__OSX__)

#elif defined(__ANDROID__)

#else
//#include <sys/stat.h>
#endif

#ifdef WIN32
std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
#endif

#include <sstream>
#include <algorithm>
namespace FileSystem
{

	namespace Directory
	{
		bool CreateFolder2(std::string _path)
		{
			#if defined(WIN32)
				return CreateDirectory(s2ws(_path).c_str(), NULL);
			#elif defined(__IOS__) || defined(__OSX__)
                return mkdir(_path.c_str(), 0775) == 0;
			#elif defined(__OSX__)

			#elif defined(__ANDROID__)

			#else
						//return mkdir(_path.c_str(), 0775) == 0;
			#endif
			return false;
		}

		bool CreateFolder(std::string _path)
		{
			if (_path.size() == 0)
				return true;

#ifdef WIN32
			std::replace(_path.begin(), _path.end(), '\\', '/');
#endif
			char* path = new char[_path.size() + 1];
			memcpy(path, _path.c_str(), _path.size());
			path[_path.size()] = '\0';

			bool result = true;
			char* end = NULL;
			char* start = path + 1;
			do
			{
				end = strchr(start, '/');
				if (end)
					*end = '\0';

				if (!DirectoryExist(path))
				{
					if (!CreateFolder2(path))
					{
						result = false;
						break;
					}
				}

				if (end)
					*end = '/';

				start = end + 1;

			} while (end);
			delete[] path;
			return result;

		}

		bool DirectoryExist(std::string _path)
		{
			if (_path.at(_path.size() - 1) == '/')
				_path = _path.substr(0, _path.size() - 1);

			#if defined(WIN32) || defined(__IOS__) || defined(__OSX__)
				struct stat info;
				if (stat(_path.c_str(), &info) != 0)
					return false;
				else if (info.st_mode & S_IFDIR)  // S_ISDIR() doesn't exist on my windows 
					return true;
				else
					return false;
			#endif
			return false;
		}
	}
}
