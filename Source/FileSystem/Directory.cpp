#include "Directory.h"
#include <cstring>

#include <sys/types.h>
#include <sys/stat.h>

#include <SDL/SDL.h>

#if defined(WIN32)

#include "FileSystem/dirent.h"
#include <windows.h>
#else
#include <dirent.h>
#endif

#ifdef __ANDROID__
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
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

#ifdef __ANDROID__
extern "C"
{
	AAssetManager* g_assetManager = NULL;
	JNIEnv* g_env = NULL;
	JNIEXPORT void JNICALL Java_org_libsdl_app_SDLActivity_onCreateJNI(JNIEnv* env, void* reserved, jobject assetManager)
	{
		g_env = env;
		g_assetManager = AAssetManager_fromJava(env, assetManager);
	}
};
#endif

namespace FileSystem
{

	namespace Directory
	{
		bool CreateFolder2(std::string _path)
		{
			#if defined(WIN32)
				return CreateDirectory(s2ws(_path).c_str(), NULL);

			// defined(__ANDROID__)

			#else
				return mkdir(_path.c_str(), 0775) == 0;
			#endif
			return false;
		}

		bool Create(std::string _path)
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

				if (!Exist(path))
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

		bool Exist(std::string _path)
		{
			if (_path.at(_path.size() - 1) == '/')
				_path = _path.substr(0, _path.size() - 1);

			//#if !defined(__ANDROID__)
				struct stat info;
				if (stat(_path.c_str(), &info) != 0)
					return false;
				else if (info.st_mode & S_IFDIR)  // S_ISDIR() doesn't exist on my windows 
					return true;
			//#endif
			return false;
		}
        
        std::vector<Entry> GetEntries(std::string _path)
        {
            std::vector<Entry> result;
            DIR *dir;
            struct dirent *ent;
            if ((dir = opendir(_path.c_str())) != NULL)
            {
                
                while ((ent = readdir (dir)) != NULL)
                {
                    if (strcmp(ent->d_name, ".") && strcmp(ent->d_name, ".."))
                    {
						Entry e;
						e.name = ent->d_name;
						if (ent->d_type == DT_DIR)
						{
							e.type = _Directory;
							result.push_back(e);
						}
						else if (ent->d_type == DT_REG)
						{
							e.type = _File;
							result.push_back(e);
						}
                    }
                }
                closedir (dir);
            }
            
            return result;
        }
        
        std::vector<std::string> GetSubDirectories(std::string _path)
        {
            std::vector<std::string> result;
            DIR *dir;
            struct dirent *ent;
            if ((dir = opendir(_path.c_str())) != NULL)
            {
                while ((ent = readdir (dir)) != NULL)
                {
                    if (strcmp(ent->d_name, ".") && strcmp(ent->d_name, ".."))
                    {
                        if (ent->d_type == DT_DIR)
                        {
                            result.push_back(ent->d_name);
                        }
                    }
                }
                closedir (dir);
            }
            #ifdef __ANDROID__
	    if (result.empty())
	    {
		    jmethod method = g_env->GetMethodID(env->FindClass("class/path/ListDataClass"), "GetSubDirectories", "(L[class/path/ListDataClass;)V");
		    int count = g_env->GetArrayLength(method);
	    }
	    #endif
            
            return result;
        }
        
        std::vector<std::string> GetFiles(std::string _path)
        {
            std::vector<std::string> result;
            DIR *dir;
            struct dirent *ent;
            if ((dir = opendir(_path.c_str())) != NULL)
            {
                while ((ent = readdir (dir)) != NULL)
                {
                    if (ent->d_type == DT_REG)
                    {
                        result.push_back(ent->d_name);
                    }
                }
                closedir (dir);
            }
#ifdef __ANDROID__
		else
		{
			auto assetDir = AAssetManager_openDir(g_assetManager, path.c_str());
			while (true)
			{
				const char* assetDirString = AAssetDir_getNextFileName(assetDir);
				if (assetDirString == NULL)
					break;
				result.push_back(std::string(assetDirString));
			}
			AAssetDir_close(assetDir);
		}
#endif
            return result;
        }
        
        void GetAllFiles2(std::string _path, std::string _prefix, std::vector<std::string> &_files)
        {
            //Add Files
            std::vector<std::string> files = GetFiles(_path);
            
            for (int i = 0; i < files.size(); ++i)
            {
                std::string file = _prefix;
                file.append(files[i]);
                _files.push_back(file);
            }
            
            //Add Sub dirs
            std::vector<std::string> dirs = GetSubDirectories(_path);
            
            for (int i = 0; i < dirs.size(); ++i)
            {
                std::string path = _path;
                path.append(dirs[i]);
                path.append("/");

                std::string prefix = _prefix;
                prefix.append(dirs[i]);
                prefix.append("/");
                GetAllFiles2(path, prefix, _files);
            }
            
        }
        
        std::vector<std::string> GetAllFiles(std::string _path)
        {
            std::vector<std::string> result;
            GetAllFiles2(_path, "", result);
            return result;
        }
	}
}
