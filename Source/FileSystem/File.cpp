#include "FileSystem/File.h"
#include <sstream>

#include <sys/stat.h>

#include "FileSystem/Directory.h"

namespace FileSystem
{
	namespace File
	{

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

			#if !defined(__ANDROID__)
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
			return false;
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

		void Write(SDL_RWops* _file, const unsigned char* data, int size)
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
			ss << "\r\n";

			_text = ss.str();

			SDL_RWwrite(_file, _text.c_str(), 1, _text.size());
		}

	}
}
