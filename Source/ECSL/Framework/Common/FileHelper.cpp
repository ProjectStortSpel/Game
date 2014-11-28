#include "FileHelper.h"

#ifdef _WINDOWS
#include <windows.h>
#include <direct.h>
#define GetCurrentDir _getcwd
#include "StringConverter.h"
#else
#include <dirent.h>
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

using namespace ECSL;

void FileHelper::GetFilesInDirectory(std::vector<std::string>& _out, const std::string& _directory, const std::string& _extension)
{
#ifdef _WINDOWS
	HANDLE dir;
	WIN32_FIND_DATA file_data;
	char* buf = 0;
	buf = GetCurrentDir(0, 0);
	std::string currentDirectory(buf);
	delete buf;
	std::wstring directory = StringConverter::StringToWString(currentDirectory + "/" + _directory + "/*");
	if ((dir = FindFirstFile(directory.c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return; /* No files found */

	do {
		const std::wstring file_name = file_data.cFileName;
		const std::wstring full_file_name = StringConverter::StringToWString(_directory + "/" + StringConverter::WStringToString(file_name));
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.')
			continue;

		size_t extensionStart = file_name.find_last_of('.');
		std::wstring extension = StringConverter::StringToWString(_extension);
		extension.pop_back();
		if (!is_directory && _extension != "*" && file_name.substr(extensionStart + 1, file_name.length() - extensionStart - 1) != extension)
			continue;

		if (is_directory)
			continue;

		_out.push_back(StringConverter::WStringToString(full_file_name));
	} while (FindNextFile(dir, &file_data));

	FindClose(dir);
#else
	DIR *dir;
	class dirent *ent;
	class stat st;

	dir = opendir(directory);
	while ((ent = readdir(dir)) != NULL) {
		const string file_name = ent->d_name;
		const string full_file_name = _directory + "/" + file_name;

		if (file_name[0] == '.')
			continue;

		if (stat(full_file_name.c_str(), &st) == -1)
			continue;

		const bool is_directory = (st.st_mode & S_IFDIR) != 0;

		if (is_directory)
			continue;

		_out.push_back(full_file_name);
	}
	closedir(dir);
#endif
}