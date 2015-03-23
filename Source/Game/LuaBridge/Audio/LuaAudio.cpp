#include "LuaAudio.h"
#include "Audio/Audio.h"
#include "Game/HomePath.h"'
#include "FileSystem/File.h"

namespace LuaBridge
{
	namespace LuaAudio
	{
		int SetVolume(lua_State* L);
		int GetVolume(lua_State* L);

		int SetDistance(lua_State* L);
		int SetCameraPosition(lua_State* L);
		
		int LoadMusic(lua_State* L);
		int PlayMusic(lua_State* L);
		int ResumeMusic(lua_State* L);
		int PauseMusic(lua_State* L);
		int StopMusic(lua_State* L);
		int FadeInMusic(lua_State* L);
		int FadeOutMusic(lua_State* L);
		
		int LoadSound(lua_State* L);
		int PlaySound(lua_State* L);
		int PauseSound(lua_State* L);
		int ResumeSound(lua_State* L);
		int StopSound(lua_State* L);
		int SetSoundPosition(lua_State* L);
		int FadeInSound(lua_State* L);
		int FadeOutSound(lua_State* L);
		int SetSoundVolume(lua_State* L);
		
		int ChannelExists(lua_State* L);
		
		void Embed(lua_State* L)
		{
			LuaEmbedder::AddFunction(L, "SetVolume", SetVolume, "Audio");
			LuaEmbedder::AddFunction(L, "GetVolume", GetVolume, "Audio");

			LuaEmbedder::AddFunction(L, "SetDistance", SetDistance, "Audio");
			LuaEmbedder::AddFunction(L, "SetCameraPosition", SetCameraPosition, "Audio");
			
			LuaEmbedder::AddFunction(L, "LoadMusic", LoadMusic, "Audio");
			LuaEmbedder::AddFunction(L, "PlayMusic", PlayMusic, "Audio");
			LuaEmbedder::AddFunction(L, "ResumeMusic", ResumeMusic, "Audio");
			LuaEmbedder::AddFunction(L, "PauseMusic", PauseMusic, "Audio");
			LuaEmbedder::AddFunction(L, "StopMusic", StopMusic, "Audio");
			LuaEmbedder::AddFunction(L, "FadeInMusic", FadeInMusic, "Audio");
			LuaEmbedder::AddFunction(L, "FadeOutMusic", FadeOutMusic, "Audio");
			
			LuaEmbedder::AddFunction(L, "LoadSound", LoadSound, "Audio");
			LuaEmbedder::AddFunction(L, "PlaySound", PlaySound, "Audio");
			LuaEmbedder::AddFunction(L, "PauseSound", PauseSound, "Audio");
			LuaEmbedder::AddFunction(L, "ResumeSound", ResumeSound, "Audio");
			LuaEmbedder::AddFunction(L, "StopSound", StopSound, "Audio");
			LuaEmbedder::AddFunction(L, "SetSoundPosition", SetSoundPosition, "Audio");
			LuaEmbedder::AddFunction(L, "FadeInSound", FadeInSound, "Audio");
			LuaEmbedder::AddFunction(L, "FadeOutSound", FadeOutSound, "Audio");
			LuaEmbedder::AddFunction(L, "SetSoundVolume", SetSoundVolume, "Audio");
			
			LuaEmbedder::AddFunction(L, "ChannelExists", ChannelExists, "Audio");
		}
		
		int SetVolume(lua_State* L)
		{
			float volume = LuaEmbedder::PullFloat(L, 1);
			Audio::SetVolume(volume);
			return 0;
		}

		int GetVolume(lua_State* L)
		{
			LuaEmbedder::PushFloat(L, Audio::GetVolume());
			return 1;
		}

		int SetDistance(lua_State* L)
		{
			float near = LuaEmbedder::PullFloat(L, 1);
			float far = LuaEmbedder::PullFloat(L, 2);
			Audio::SetDistance(near, far);
			return 0;
		}
		
		int SetCameraPosition(lua_State* L)
		{
			float x = LuaEmbedder::PullFloat(L, 1);
			float y = LuaEmbedder::PullFloat(L, 2);
			float z = LuaEmbedder::PullFloat(L, 3);
			Audio::SetCameraPosition(glm::vec3(x, y, z));
			return 0;
		}
		
		int LoadMusic(lua_State* L)
		{
			std::string filepath = LuaEmbedder::PullString(L, 1);
			Audio::LoadMusic(filepath);
			return 0;
		}
		int PlayMusic(lua_State* L)
		{
			Audio::PlayMusic();
			return 0;
		}
		int ResumeMusic(lua_State* L)
		{
			Audio::ResumeMusic();
			return 0;
		}
		int PauseMusic(lua_State* L)
		{
			Audio::PauseMusic();
			return 0;
		}
		int StopMusic(lua_State* L)
		{
			Audio::StopMusic();
			return 0;
		}
		int FadeInMusic(lua_State* L)
		{
			int ms = LuaEmbedder::PullInt(L, 1);
			Audio::FadeInMusic(ms);
			return 0;
		}
		int FadeOutMusic(lua_State* L)
		{
			int ms = LuaEmbedder::PullInt(L, 1);
			Audio::FadeOutMusic(ms);
			return 0;
		}

		
		int LoadSound(lua_State* L)
		{
			std::string name = LuaEmbedder::PullString(L, 1);
			std::string filepath = LuaEmbedder::PullString(L, 2);

			HomePath::Type type = LuaEmbedder::PullBool(L, "Client") ? HomePath::Type::Client : HomePath::Type::Server;
			std::vector<std::string> paths = HomePath::GetPaths(type);

			for (int i = 0; i < paths.size(); ++i)
			{
				paths[i].append(filepath);

				if (FileSystem::File::Exist(paths[i]))
				{
					Audio::LoadSound(name, paths[i]);
					break;
				}
			}
			return 0;
		}
		int PlaySound(lua_State* L)
		{
			std::string name = LuaEmbedder::PullString(L, 1);
			bool loop = false;
			if (LuaEmbedder::IsBool(L, 2))
			{
				loop = LuaEmbedder::PullBool(L, 2);
			}
			else if (LuaEmbedder::IsString(L, 2))
			{
				std::string channelName = LuaEmbedder::PullString(L, 2);
				bool loop = false;
				if (LuaEmbedder::IsBool(L, 3))
				{
					loop = LuaEmbedder::PullBool(L, 3);
				}
				Audio::PlaySound(name, channelName, loop);
				return 0;
			}
			Audio::PlaySound(name, loop);
			return 0;
		}
		int PauseSound(lua_State* L)
		{
			std::string channelName = LuaEmbedder::PullString(L, 1);
			Audio::PauseSound(channelName);
			return 0;
		}
		int ResumeSound(lua_State* L)
		{
			std::string channelName = LuaEmbedder::PullString(L, 1);
			Audio::ResumeSound(channelName);
			return 0;
		}
		int StopSound(lua_State* L)
		{
			std::string channelName = LuaEmbedder::PullString(L, 1);
			Audio::StopSound(channelName);
			return 0;
		}
		int SetSoundPosition(lua_State* L)
		{
			std::string channelName = LuaEmbedder::PullString(L, 1);
			float x = LuaEmbedder::PullFloat(L, 2);
			float y = LuaEmbedder::PullFloat(L, 3);
			float z = LuaEmbedder::PullFloat(L, 4);
			Audio::SetSoundPosition(channelName, glm::vec3(x, y, z));
			return 0;
		}
		int FadeInSound(lua_State* L)
		{
			std::string name = LuaEmbedder::PullString(L, 1);
			if (LuaEmbedder::IsInt(L, 2))
			{
				int ms = LuaEmbedder::PullInt(L, 2);
				bool loop = false;
				if (LuaEmbedder::IsBool(L, 3))
				{
					loop = LuaEmbedder::PullBool(L, 3);
				}
				Audio::FadeInSound(name, ms, loop);
			}
			else if (LuaEmbedder::IsString(L, 2))
			{
				std::string channelName = LuaEmbedder::PullString(L, 2);
				int ms = LuaEmbedder::PullInt(L, 3);
				bool loop = false;
				if (LuaEmbedder::IsBool(L, 4))
				{
					loop = LuaEmbedder::PullBool(L, 4);
				}
				Audio::FadeInSound(name, channelName, ms, loop);
			}
			return 0;
		}
		int FadeOutSound(lua_State* L)
		{
			std::string channelName = LuaEmbedder::PullString(L, 1);
			int ms = LuaEmbedder::PullInt(L, 2);
			Audio::FadeOutSound(channelName, ms);
			return 0;
		}
		int SetSoundVolume(lua_State* L)
		{
			std::string channelName = LuaEmbedder::PullString(L, 1);
			int volume = LuaEmbedder::PullInt(L, 2);
			Audio::SetVolume(channelName, volume);
			return 0;
		}
		
		int ChannelExists(lua_State* L)
		{
			std::string channelName = LuaEmbedder::PullString(L, 1);
			LuaEmbedder::PushBool(L, Audio::ChannelExists(channelName));
			return 1;
		}
	}
}