#include "LuaFile.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "FileSystem/File.h"
#include "Game/HomePath.h"

#include <fstream>
#include <sstream>
#include <string>
//#include <SDL/SDL.h>


namespace LuaBridge
{

	namespace LuaFile
	{
		int Create(lua_State* L);
		int Append(lua_State* L);
		int Open(lua_State* L);

		int Delete(lua_State* L);

		int Close(lua_State* L);

		int Read(lua_State* L);
		int ReadLine(lua_State* L);
		int Write(lua_State* L);

		int WriteLine(lua_State* L);


		void Embed(lua_State* L)
		{
			LuaEmbedder::AddFunction(L, "Create", &Create, "File");
			LuaEmbedder::AddFunction(L, "Append", &Append, "File");
			LuaEmbedder::AddFunction(L, "Open", &Open, "File");

			//LuaEmbedder::AddFunction("Delete", &Delete, "File");

			LuaEmbedder::AddFunction(L, "Close", &Close, "File");

			LuaEmbedder::AddFunction(L, "Read", &Read, "File");
			LuaEmbedder::AddFunction(L, "ReadLine", &ReadLine, "File");
			LuaEmbedder::AddFunction(L, "Write", &Write, "File");
			LuaEmbedder::AddFunction(L, "WriteLine", &WriteLine, "File");

		}


		int Create(lua_State* L)
		{
			std::string filepath = LuaEmbedder::PullString(L, 1);

			std::ostringstream ss;
			ss << HomePath::GetSecondaryGameModePath();
			ss << filepath.c_str();

			FileSystem::File::Create(ss.str());

			return 0;
		}

		int Append(lua_State* L)
		{
			std::string filepath = LuaEmbedder::PullString(L, 1);

			std::ostringstream ss1;
			ss1 << HomePath::GetSecondaryGameModePath();
			ss1 << filepath.c_str();

			SDL_RWops* file;
			if (!FileSystem::File::Append(ss1.str(), &file))
			{
				LuaEmbedder::PushString(L, "0");
				return 1;
			}

			std::ostringstream ss2;
			ss2 << file;
			LuaEmbedder::PushString(L, ss2.str().c_str());

			return 1;
		}

		int Open(lua_State* L)
		{
			std::string filepath = LuaEmbedder::PullString(L, 1);

			std::ostringstream ss1;
			ss1 << HomePath::GetSecondaryHomePath();
			ss1 << filepath.c_str();

			SDL_RWops* file;
			if (!FileSystem::File::Open(ss1.str(), &file))
			{
				LuaEmbedder::PushString(L, "0");
				return 1;
			}

			std::ostringstream ss2;
			ss2 << file;
			LuaEmbedder::PushString(L, ss2.str().c_str());

			return 1;
		}

		int Delete(lua_State* L)
		{
			std::string filepath = LuaEmbedder::PullString(L, 1);

			std::ostringstream ss1;
			ss1 << HomePath::GetSecondaryGameModePath();
			ss1 << filepath.c_str();

			LuaEmbedder::PushBool(L, FileSystem::File::Delete(ss1.str()));
			return 1;
		}

		int Close(lua_State* L)
		{
			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			//uint64_t id = strtoull(sId.c_str(), &end, 16);

			SDL_RWops* file = (SDL_RWops*)strtoull(sId.c_str(), &end, 16);

			FileSystem::File::Close(file);

			return 0;
		}


		int Read(lua_State* L)
		{
			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			SDL_RWops* file = (SDL_RWops*)strtoull(sId.c_str(), &end, 16);


			int length = 1;
			
			if (LuaEmbedder::IsInt(L, 2))
				length = LuaEmbedder::PullInt(L, 2);

			char* data = FileSystem::File::Read(file, length);

			std::string text = data;

			delete data;

			LuaEmbedder::PushString(L, text);

			return 1;
		}

		int ReadLine(lua_State* L)
		{
			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			SDL_RWops* file = (SDL_RWops*)strtoull(sId.c_str(), &end, 16);


			std::string text = FileSystem::File::ReadLine(file);

			LuaEmbedder::PushString(L, text);

			return 1;
		}

		int Write(lua_State* L)
		{
			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			SDL_RWops* file = (SDL_RWops*)strtoull(sId.c_str(), &end, 16);

			std::string text = LuaEmbedder::PullString(L, 2);

			FileSystem::File::Write(file, text);
			
			return 0;
		}

		int WriteLine(lua_State* L)
		{
			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			SDL_RWops* file = (SDL_RWops*)strtoull(sId.c_str(), &end, 16);

			std::string text = LuaEmbedder::PullString(L, 2);

			FileSystem::File::WriteLine(file, text);
			
			return 0;
		}

		

	}
}
