#include "Game/ResourceManager.h"
#include "FileSystem/File.h"
#include "FileSystem/MD5.h"

namespace ResourceManager
{
	std::map<std::string, Resource> GamemodeResources;
	std::map<std::string, Resource> ContentResources;

	bool CreateResource(std::string _path, Resource &_r, HomePath::Type _type)
	{
		std::vector<std::string> paths = HomePath::GetPaths(_type);
		std::string path;
		bool foundFile = false;
		int i;
		for (i = 0; i < paths.size(); ++i)
		{
			path = paths[i];
			path.append(_path);
			if (FileSystem::File::Exist(path))
			{
				foundFile = true;
				break;
			}
		}

		if (!foundFile)
		{
			return false;
		}
		_r.File = _path;
		_r.Location = path;

		_r.MD5 = FileSystem::MD5::MD5_File(path);

		SDL_RWops* file;
		FileSystem::File::Open(path, &file);
		_r.Size = (int)FileSystem::File::GetFileSize(file);
		FileSystem::File::Close(file);

		return true;
	}

	bool CreateResource(std::string _gamemode, std::string _path, Resource &_r, HomePath::Type _type)
	{
		std::vector<std::string> paths = HomePath::GetPaths(_type, _gamemode);
		std::string path;
		bool foundFile = false;
		int i;
		for (i = 0; i < paths.size(); ++i)
		{
			path = paths[i];
			path.append(_path);
			if (FileSystem::File::Exist(path))
			{
				foundFile = true;
				break;
			}
		}

		if (!foundFile)
		{
			return false;
		}
		_r.File = _path;
		_r.Location = path;

		_r.MD5 = FileSystem::MD5::MD5_File(path);

		SDL_RWops* file;
		FileSystem::File::Open(path, &file);
		_r.Size = (int)FileSystem::File::GetFileSize(file);
		FileSystem::File::Close(file);
		
		return true;
	}

	bool ResourceExist(std::string _file, std::map<std::string, Resource>* _list)
	{
		for (auto it = _list->begin(); it != _list->end(); ++it)
		{
			if (it->second.File == _file)
				return true;
		}
		return false;
	}

	int AddGamemodeResource(std::string _path)
	{
		Resource r;
		if (!ResourceExist(_path, &GamemodeResources) && CreateResource(_path, r, HomePath::Type::Server))
		{
			GamemodeResources[_path] = r;
			return 1;
		}
		return 0;
	}

	int AddContentResource(std::string _path)
	{
		Resource r;
		if (!ResourceExist(_path, &ContentResources) && CreateResource(_path, r, HomePath::Type::Server))
		{
			ContentResources[_path]= r;
			return 1;
		}
		return 0;
	}

	std::map<std::string, Resource>* GetGamemodeResources()
	{
		return &GamemodeResources;
	}

	std::map<std::string, Resource>* GetContentResources()
	{
		return &ContentResources;
	}

	void Clear()
	{
		GamemodeResources.clear();
		ContentResources.clear();
	}
}