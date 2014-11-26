#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <vector>

namespace ECSL
{
	namespace FileHelper
	{
		void GetFilesInDirectory(std::vector<std::string>& _out, const std::string& _directory, const std::string& _extension);
	}
}

#endif