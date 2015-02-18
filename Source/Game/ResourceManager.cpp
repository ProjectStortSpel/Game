#include "Game/ResourceManager.h"
#include "Game/HomePath.h"
#include "FileSystem/File.h"
#include "FileSystem/MD5.h"

namespace ResourceManager
{
	std::vector<Resource> GamemodeResources;
	std::vector<Resource> ContentResources;

	bool CreateResource(std::string _path, Resource &r)
	{
		std::vector<std::string> paths = HomePath::GetPaths();
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
		r.File = _path;
		r.Location = path;

		r.MD5 = FileSystem::MD5::MD5_File(path);

		SDL_RWops* file;
		FileSystem::File::Open(path, &file);
		r.Size = (int)FileSystem::File::GetFileSize(file);
		FileSystem::File::Close(file);
		
		return true;
	}

	bool ResourceExist(std::string _file, std::vector<Resource>* _list)
	{
		for (int i = 0; i < _list->size(); ++i)
		{
			if (_list->at(i).File == _file)
				return true;
		}
		return false;
	}

	int AddGamemodeResource(std::string _path)
	{
		Resource r;
		if (!ResourceExist(_path, &GamemodeResources) && CreateResource(_path, r))
		{
			GamemodeResources.push_back(r);
			return 1;
		}
		return 0;
	}

	int AddContentResource(std::string _path)
	{
		Resource r;
		if (!ResourceExist(_path, &ContentResources) && CreateResource(_path, r))
		{
			ContentResources.push_back(r);
			return 1;
		}
		return 0;
	}

	std::vector<Resource>* GetGamemodeResources()
	{
		return &GamemodeResources;
	}

	std::vector<Resource>* GetContentResources()
	{
		return &ContentResources;
	}

	void Clear()
	{
		GamemodeResources.clear();
		ContentResources.clear();
	}
}