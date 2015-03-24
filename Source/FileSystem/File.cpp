#include "FileSystem/File.h"
#include <sstream>

#include <sys/stat.h>
#include <cstdio>

#include "FileSystem/Directory.h"

namespace FileSystem
{
	namespace File
	{

		struct Extension 
		{
			static const char* binary[];
			static const char* ascii[];
		};

		const int numBinaryExtensions = 5;
		const int numAsciiExtensions = 7;
		const char* Extension::binary[] = { "png", "mesh", "amesh", "db", "exe" };
		const char* Extension::ascii[] = { "txt", "meshOLD", "lua", "object", "ajoints", "anim", "aobject" };

		bool IsBinary(std::string _path)
		{
			unsigned int found = _path.find_last_of('.');

			if (found == std::string::npos)
			{
				return false;
			}

			std::string extension = _path.substr(found + 1);

			for (int i = 0; i < numBinaryExtensions; ++i)
			{
				if (extension == Extension::binary[i])
				{
					return true;
				}
			}

			for (int i = 0; i < numAsciiExtensions; ++i)
			{
				if (extension == Extension::ascii[i])
				{
					return false;
				}
			}

			if (Exist(_path))
			{
				//open file and read data
				SDL_RWops* file;
				if (Open(_path, &file))
				{
					Sint64 length = GetFileSize(file);
					char* data = Read(file, length);
					Close(file);
					for (int i = 0; i < length; ++i)
					{
						if (data[i] == '\0')
						{
							delete [] data;
							return true;
						}
					}
					delete [] data;
					return false;
				}
			}
			return false;
		}

		bool Create(std::string _path)
		{
            //Create Directory
			unsigned found = _path.find_last_of("/\\");

			if (found != 0 && found != std::string::npos)
			{
				Directory::Create(_path.substr(0, found));
			}

			SDL_RWops* file = SDL_RWFromFile(_path.c_str(), "w");
			if (!file)
				return false;
			SDL_RWclose(file);
			return true;
		}

		bool Append(std::string _path, SDL_RWops** _file)
		{
			*_file = SDL_RWFromFile(_path.c_str(), "a");
			return *_file != NULL;
		}

		bool Open(std::string _path, SDL_RWops** _file)
		{
            const char* str = _path.c_str();
			*_file = SDL_RWFromFile(_path.c_str(), "r");

			if (!*_file)
            {
                SDL_Log("Failed to open file. Error: %s", SDL_GetError());
                return false;
            }
			SDL_RWseek(*_file, 0, RW_SEEK_SET);
			return true;
		}

		bool Exist(std::string _path)
		{
			if (_path.at(_path.size() - 1) == '/')
				_path = _path.substr(0, _path.size() - 1);

			#if !defined(__ANDROID__) && !defined(__IOS__)
			struct stat info;
			if (stat(_path.c_str(), &info) != 0)
				return false;
			else if (info.st_mode & S_IFREG)  // S_ISREG() doesn't exist on my windows 
				return true;
			#else
			SDL_RWops* rw = SDL_RWFromFile(_path.c_str(), "r");
			if (rw != NULL)
			{
				SDL_RWclose(rw);
				return true;
			}
			#endif
			return false;
		}

		bool Delete(std::string _path)
		{
            return remove(_path.c_str()) == 0;
		}

		void Close(SDL_RWops* _file)
		{
			if (!_file)
				return;
			SDL_RWclose(_file);
		}

		Sint64 GetFileSize(SDL_RWops* _file)
		{
			if (!_file)
				return 0;
			Sint64 offset = SDL_RWtell(_file);
			Sint64 length = SDL_RWseek(_file, 0, RW_SEEK_END);
			SDL_RWseek(_file, offset, RW_SEEK_SET);
			return length;
		}

		Sint64 GetPosition(SDL_RWops* _file)
		{
			if (!_file)
				return 0;
			Sint64 offset = SDL_RWtell(_file);
			return offset;
		}

		Sint64 GetRemainingBytes(SDL_RWops* _file)
		{
			if (!_file)
				return 0;
			Sint64 offset = SDL_RWtell(_file);
			Sint64 length = SDL_RWseek(_file, offset, RW_SEEK_END);
			SDL_RWseek(_file, offset, RW_SEEK_SET);
			return length;
		}

		char* Read(SDL_RWops* _file, int _length)
		{
			if (!_file)
				return NULL;

			char* data = new char[_length];
			int endpos = SDL_RWread(_file, data, 1, _length);
						
			return data;
		}

		std::string ReadLine(SDL_RWops* _file)
		{
			if (!_file)
				return "";

			std::ostringstream ss;

			char* data = new char[11];

			while (true)
			{
				Sint64 pos = SDL_RWseek(_file, 0, RW_SEEK_CUR);
				int endpos = SDL_RWread(_file, data, 1, 10);
				data[endpos] = '\0';

				if (endpos == 0)
					break;

				char* first = strchr(data, '\n');

				if (first)
				{
					*first = '\0';
					ss << data;
					int offset = (first - data) + 1;
					SDL_RWseek(_file, pos + offset, RW_SEEK_SET);
					break;
				}

				ss << data;
			}


			std::string text = ss.str();

			delete data;

			return text;
		}

		void Write(SDL_RWops* _file, std::string _text)
		{
			if (!_file)
				return;
			SDL_RWwrite(_file, _text.c_str(), 1, _text.size());
		}

		void Write(SDL_RWops* _file, const char* data, int size)
		{
			if (!_file)
				return;
			SDL_RWwrite(_file, data, 1, size);
		}

		void WriteLine(SDL_RWops* _file, std::string _text)
		{
			if (!_file)
				return;
			std::ostringstream ss;
			ss << _text.c_str();

#ifdef WIN32
			//ss << "\r\n";
			ss << "\n";
#else
			ss << "\n";
#endif

			_text = ss.str();

			SDL_RWwrite(_file, _text.c_str(), 1, _text.size());
		}

	}
}
