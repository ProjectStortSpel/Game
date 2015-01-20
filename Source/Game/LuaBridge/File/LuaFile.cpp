#include "LuaFile.h"
#include "LuaEmbedder/LuaEmbedder.h"

#include <fstream>
#include <sstream>
#include <string>
#include <SDL/SDL.h>



namespace LuaBridge
{

	namespace LuaFile
	{
		int Create();
		int Append();
		int Open();

		int Delete();

		int Close();

		int Read();
		int ReadLine();
		int Write();

		int WriteLine();


		void Embed()
		{
			LuaEmbedder::AddFunction("Create", &Create, "File");
			LuaEmbedder::AddFunction("Append", &Append, "File");
			LuaEmbedder::AddFunction("Open", &Open, "File");

			//LuaEmbedder::AddFunction("Delete", &Delete, "File");

			LuaEmbedder::AddFunction("Close", &Close, "File");

			LuaEmbedder::AddFunction("Read", &Read, "File");
			LuaEmbedder::AddFunction("ReadLine", &ReadLine, "File");
			LuaEmbedder::AddFunction("Write", &Write, "File");
			LuaEmbedder::AddFunction("WriteLine", &WriteLine, "File");

		}


		int Create()
		{
			std::string filepath = LuaEmbedder::PullString(1);

			std::ostringstream ss;
			ss << "content\\data\\";
			ss << filepath.c_str();

			SDL_RWops* file = SDL_RWFromFile(ss.str().c_str(), "w");
			SDL_RWclose(file);

			return 0;
		}

		int Append()
		{
			std::string filepath = LuaEmbedder::PullString(1);

			std::ostringstream ss1;
			ss1 << "content\\data\\";
			ss1 << filepath.c_str();

			SDL_RWops* file = SDL_RWFromFile(ss1.str().c_str(), "a");

			std::ostringstream ss2;
			ss2 << file;
			LuaEmbedder::PushString(ss2.str().c_str());

			return 1;
		}

		int Open()
		{
			std::string filepath = LuaEmbedder::PullString(1);

			std::ostringstream ss1;
			ss1 << "content\\data\\";
			ss1 << filepath.c_str();

			SDL_RWops* file = SDL_RWFromFile(ss1.str().c_str(), "r");
			SDL_RWseek(file, 0, RW_SEEK_SET);

			std::ostringstream ss2;
			ss2 << file;
			LuaEmbedder::PushString(ss2.str().c_str());

			return 1;
		}

		int Delete()
		{

			return 0;
		}

		int Close()
		{
			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			//uint64_t id = strtoull(sId.c_str(), &end, 16);

			SDL_RWops* file = (SDL_RWops*)strtoull(sId.c_str(), &end, 16);

			SDL_RWclose(file);
			return 0;
		}


		int Read()
		{
			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			SDL_RWops* file = (SDL_RWops*)strtoull(sId.c_str(), &end, 16);


			int length = 1;
			
			if (LuaEmbedder::IsInt(2))
				length = LuaEmbedder::PullInt(2);

			char* data = new char[length + 1];
			int endpos = SDL_RWread(file, data, 1, length);
			data[endpos] = '\0';

			std::string text = data;

			delete data;

			LuaEmbedder::PushString(text);

			return 1;
		}

		int ReadLine()
		{
			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			SDL_RWops* file = (SDL_RWops*)strtoull(sId.c_str(), &end, 16);


			std::ostringstream ss;

			char* data = new char[11];

			while (true)
			{
				Sint64 pos = SDL_RWseek(file, 0, RW_SEEK_CUR);
				int endpos = SDL_RWread(file, data, 1, 10);
				data[endpos] = '\0';

				if (endpos == 0)
					break;

				char* first = strchr(data, '\n');

				if (first)
				{
					*first = '\0';
					ss << data;
					int offset = (first - data) + 1;
					SDL_RWseek(file, pos + offset, RW_SEEK_SET);
					break;
				}

				ss << data;
			}


			std::string text = ss.str();

			delete data;

			LuaEmbedder::PushString(text);

			return 1;
		}

		int Write()
		{
			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			SDL_RWops* file = (SDL_RWops*)strtoull(sId.c_str(), &end, 16);

			std::string text = LuaEmbedder::PullString(2);

			SDL_RWwrite(file, text.c_str(), 1, text.size());
			
			return 0;
		}

		int WriteLine()
		{
			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			SDL_RWops* file = (SDL_RWops*)strtoull(sId.c_str(), &end, 16);

			std::string text = LuaEmbedder::PullString(2);

			std::ostringstream ss;
			ss << text.c_str();
			ss << "\r\n";

			text = ss.str();

			SDL_RWwrite(file, text.c_str(), 1, text.size());

			return 0;
		}

		

	}
}
