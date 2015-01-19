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
		int Write();


		void Embed()
		{
			LuaEmbedder::AddFunction("Create", &Create, "File");
			LuaEmbedder::AddFunction("Append", &Append, "File");
			LuaEmbedder::AddFunction("Open", &Append, "File");

			//LuaEmbedder::AddFunction("Delete", &Delete, "File");

			LuaEmbedder::AddFunction("Close", &Close, "File");

			LuaEmbedder::AddFunction("Read", &Read, "File");
			LuaEmbedder::AddFunction("Write", &Write, "File");

		}


		int Create()
		{
			std::string filepath = LuaEmbedder::PullString(1);
			SDL_RWops* file = SDL_RWFromFile(filepath.c_str(), "w");
			SDL_RWclose(file);

			return 0;
		}

		int Append()
		{
			std::string filepath = LuaEmbedder::PullString(1);
			SDL_RWops* file = SDL_RWFromFile(filepath.c_str(), "a");

			std::ostringstream ss;
			ss << file;
			LuaEmbedder::PushString(ss.str().c_str());

			return 1;
		}

		int Open()
		{
			std::string filepath = LuaEmbedder::PullString(1);
			SDL_RWops* file = SDL_RWFromFile(filepath.c_str(), "r");

			std::ostringstream ss;
			ss << file;
			LuaEmbedder::PushString(ss.str().c_str());

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

			int position = LuaEmbedder::PullInt(2);

			SDL_RWseek(file, position, RW_SEEK_SET);

			int length = LuaEmbedder::PullInt(3);

			char* data = new char[length + 1];
			int endpos = SDL_RWread(file, data, length, 1);
			data[endpos] = '\0';
			std::string text = data;

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

		

	}
}
