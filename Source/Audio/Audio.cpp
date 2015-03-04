#include "Audio.h"

#include <SDL/SDL_mixer.h>
#include <map>
#include <vector>
#include <algorithm>

namespace Audio
{
	const int FREQUENCY = MIX_DEFAULT_FREQUENCY;
	const Uint16 FORMAT = MIX_DEFAULT_FORMAT;
	const int CHANNELS = 2;
	const int CHUNK_SIZE = 1024;
	
	enum AudioState
	{
		Play,
		Pause,
		Resume,
		Stop
	};
	
	struct PlaySoundParams
	{
		std::string name;
		bool loop;
		
		bool useChannelName;
		std::string channelName;
	};
	
	struct FadeInParams
	{
		std::string name;
		bool loop;
		int ms;
		
		bool useChannelName;
		std::string channelName;
		
		bool operator==(const FadeInParams& other)
		{
			return name == other.name &&
			       loop == other.loop &&
			       ms == other.ms &&
			       useChannelName == other.useChannelName &&
			       channelName == other.channelName;
		}
	};
	
	Mix_Music* g_music = NULL;
	std::map<std::string, Mix_Chunk*> g_sounds;
	
	
	
	float g_near, g_far;
	glm::vec3 g_cameraPosition;
	
	int g_volume = MIX_MAX_VOLUME;
	
	std::map<std::string, int> g_channels;
	
	std::vector<std::string> g_loadMusicQueue;
	std::vector<AudioState> g_musicStateQueue;
	std::vector<int> g_musicFadeInQueue;
	std::vector<int> g_musicFadeOutQueue;
	
	std::vector<std::pair<std::string, std::string>> g_loadSoundQueue;
	std::vector<PlaySoundParams> g_playSoundQueue;
	std::vector<std::pair<std::string, AudioState>> g_soundStateQueue;
	std::vector<std::pair<std::string, glm::vec3>> g_soundPositionQueue;
	std::vector<FadeInParams> g_soundFadeInQueue;
	std::vector<std::pair<std::string, int>> g_soundFadeOutQueue;
	
	SDL_mutex* g_channelMutex;
	
	void ChannelFinished(int channel)
	{
		SDL_LockMutex(g_channelMutex);
		
		for (std::pair<std::string, int> channelPair : g_channels)
		{
			if (channel == channelPair.second)
			{
				g_channels.erase(channelPair.first);
				break;
			}
		}
		
		SDL_UnlockMutex(g_channelMutex);
	}
	void Init()
	{
		SDL_Init(SDL_INIT_AUDIO);
		Mix_OpenAudio(FREQUENCY, FORMAT, CHANNELS, CHUNK_SIZE);
		Mix_AllocateChannels(128);
		Mix_ChannelFinished(ChannelFinished);
		
		g_channelMutex = SDL_CreateMutex();
	}
	
	void Update()
	{
		SDL_LockMutex(g_channelMutex);
		
		for (std::string filepath : g_loadMusicQueue)
		{
			Mix_Music* music = Mix_LoadMUS(filepath.c_str());
			if (music == NULL)
				continue;
			if (g_music != NULL)
				Mix_FreeMusic(g_music);
			g_music = music;
		}
		for (std::vector<int>::iterator musicFadeInQueueIt = g_musicFadeInQueue.begin(); musicFadeInQueueIt != g_musicFadeInQueue.end();)
		{
			if (Mix_PlayingMusic() == 0)
			{
				musicFadeInQueueIt = g_musicFadeInQueue.erase(musicFadeInQueueIt);
				continue;
			}
			if (Mix_FadeInMusic(g_music, -1, *musicFadeInQueueIt) == -1)
			{
				musicFadeInQueueIt++;
				continue;
			}
			musicFadeInQueueIt = g_musicFadeInQueue.erase(musicFadeInQueueIt);
		}
		for (std::vector<int>::iterator musicFadeOutQueueIt = g_musicFadeOutQueue.begin(); musicFadeOutQueueIt != g_musicFadeOutQueue.end();)
		{
			if (Mix_FadeOutMusic(*musicFadeOutQueueIt) == 1)
				musicFadeOutQueueIt = g_musicFadeOutQueue.erase(musicFadeOutQueueIt);
			else
				musicFadeOutQueueIt++;
		}
		for (AudioState musicState : g_musicStateQueue)
		{
			switch (musicState)
			{
				case Play:
					if (Mix_PlayingMusic() == 0)
						Mix_PlayMusic(g_music, -1);
					break;
				case Pause:
					if (Mix_PlayingMusic() == 1)
						if (Mix_PausedMusic() == 0)
							Mix_PauseMusic();
					break;
				case Resume:
					if (Mix_PlayingMusic() == 1)
						if (Mix_PausedMusic() == 1)
							Mix_ResumeMusic();
					break;
				case Stop:
					Mix_HaltMusic();
					break;
			}
		}
		g_loadMusicQueue.clear();
		g_musicStateQueue.clear();
		g_musicFadeInQueue.clear();
		
		for (std::pair<std::string, std::string> loadSound : g_loadSoundQueue)
		{
			Mix_Chunk* chunk = Mix_LoadWAV(loadSound.second.c_str());
			if (chunk == NULL)
				continue;
			if (g_sounds.find(loadSound.first) != g_sounds.end())
				Mix_FreeChunk(g_sounds[loadSound.first]);
			g_sounds[loadSound.first] = chunk;
		}
		for (PlaySoundParams playSound : g_playSoundQueue)
		{
			if (g_sounds.find(playSound.name) == g_sounds.end())
				continue;
			int channel = Mix_PlayChannel(-1, g_sounds[playSound.name], playSound.loop ? -1 : 0);
			if (channel == -1)
				continue;
			if (playSound.useChannelName)
			{
				g_channels[playSound.channelName] = channel;
			}
		}
		for (std::vector<FadeInParams>::iterator soundFadeInQueueIt = g_soundFadeInQueue.begin(); soundFadeInQueueIt != g_soundFadeInQueue.end();)
		{
			if (g_sounds.find(soundFadeInQueueIt->name) == g_sounds.end())
			{
				soundFadeInQueueIt = g_soundFadeInQueue.erase(soundFadeInQueueIt);
				continue;
			}
			if (g_channels.find(soundFadeInQueueIt->channelName) != g_channels.end())
			{
				soundFadeInQueueIt = g_soundFadeInQueue.erase(soundFadeInQueueIt);
				continue;
			}
			int channel = Mix_FadeInChannel(-1, g_sounds[soundFadeInQueueIt->name], soundFadeInQueueIt->loop ? -1 : 0, soundFadeInQueueIt->ms);
			if (channel == -1)
			{
				soundFadeInQueueIt++;
				continue;
			}
			if (soundFadeInQueueIt->useChannelName)
				g_channels[soundFadeInQueueIt->channelName] = channel;
			soundFadeInQueueIt = g_soundFadeInQueue.erase(soundFadeInQueueIt);
		}
		for (std::vector<std::pair<std::string, int>>::iterator soundFadeOutQueueIt = g_soundFadeOutQueue.begin(); soundFadeOutQueueIt != g_soundFadeOutQueue.end();)
		{
			if (g_channels.find(soundFadeOutQueueIt->first) == g_channels.end())
			{
				soundFadeOutQueueIt = g_soundFadeOutQueue.erase(soundFadeOutQueueIt);
				continue;
			}
			
			if (Mix_FadeOutChannel(g_channels[soundFadeOutQueueIt->first], soundFadeOutQueueIt->second) == 1)
				soundFadeOutQueueIt = g_soundFadeOutQueue.erase(soundFadeOutQueueIt);
			else
				soundFadeOutQueueIt++;
		}
		for (std::pair<std::string, AudioState> soundState : g_soundStateQueue)
		{
			if (g_channels.find(soundState.first) == g_channels.end())
				continue;
			switch (soundState.second)
			{
				case Pause:
					if (Mix_Playing(g_channels[soundState.first]) == 1)
						if (Mix_Paused(g_channels[soundState.first]) == 0)
							Mix_Pause(g_channels[soundState.first]);
					break;
				case Resume:
					if (Mix_Playing(g_channels[soundState.first]) == 1)
						if (Mix_Paused(g_channels[soundState.first]) == 1)
							Mix_Resume(g_channels[soundState.first]);
					break;
				case Stop:
					Mix_HaltChannel(g_channels[soundState.first]);
					break;
			}
		}
		for (std::pair<std::string, glm::vec3> position : g_soundPositionQueue)
		{
			if (g_channels.find(position.first) == g_channels.end())
				continue;
			float l = glm::length(g_cameraPosition - position.second);
			Uint8 distance;
			if (l <= g_near)
				distance = 0;
			else if (l >= g_far)
				distance = 255;
			else
				distance = (Uint8)(255.0f * (l - g_near) / (g_far - g_near));
			Mix_SetPosition(g_channels[position.first], 0, distance);
		}
		g_loadSoundQueue.clear();
		g_playSoundQueue.clear();
		g_soundStateQueue.clear();
		g_soundPositionQueue.clear();
		
		Mix_Volume(-1, g_volume);
		
		SDL_UnlockMutex(g_channelMutex);
	}
	
	void Quit()
	{
		for (std::pair<std::string, Mix_Chunk*> sound : g_sounds)
			Mix_FreeChunk(sound.second);
		
		if (g_music != NULL)
			Mix_FreeMusic(g_music);
		
		Mix_CloseAudio();
		
		SDL_DestroyMutex(g_channelMutex);
	}
	
	void SetVolume(int volume)
	{
		g_volume = volume;
	}
	
	void SetDistance(float near, float far)
	{
		g_near = near;
		g_far = far;
	}
	
	void SetCameraPosition(glm::vec3 position)
	{
		g_cameraPosition = position;
	}
	
	void LoadMusic(const std::string& filepath)
	{
		g_loadMusicQueue.push_back(filepath);
	}
	
	void PlayMusic()
	{
		g_musicStateQueue.push_back(Play);
	}

	void PauseMusic()
	{
		g_musicStateQueue.push_back(Pause);
	}
	
	void ResumeMusic()
	{
		g_musicStateQueue.push_back(Resume);
	}

	void StopMusic()
	{
		g_musicStateQueue.push_back(Stop);
	}
	
	void FadeInMusic(int ms)
	{
		g_musicFadeInQueue.push_back(ms);
	}
	
	void FadeOutMusic(int ms)
	{
		g_musicFadeOutQueue.push_back(ms);
	}
	
	void LoadSound(const std::string& name, const std::string& filepath)
	{
		g_loadSoundQueue.push_back(std::pair<std::string, std::string>(name, filepath));
	}

	void PlaySound(const std::string& name, bool loop)
	{
		g_playSoundQueue.push_back({name, loop, false, std::string()});
	}
	
	void PlaySound(const std::string& name, glm::vec3 position, bool loop)
	{
		g_playSoundQueue.push_back({name, loop, false, std::string()});
	}
	
	void PlaySound(const std::string& name, const std::string channelName, bool loop)
	{
		g_playSoundQueue.push_back({name, loop, true, channelName});
	}
	
	void PlaySound(const std::string& name, const std::string channelName, glm::vec3 position, bool loop)
	{
		g_playSoundQueue.push_back({name, loop, true, channelName});
	}
	
	void PauseSound(const std::string& channelName)
	{
		g_soundStateQueue.push_back(std::pair<std::string, AudioState>(channelName, Pause));
	}
	
	void ResumeSound(const std::string& channelName)
	{
		g_soundStateQueue.push_back(std::pair<std::string, AudioState>(channelName, Resume));
	}
	
	void StopSound(const std::string& channelName)
	{
		g_soundStateQueue.push_back(std::pair<std::string, AudioState>(channelName, Stop));
	}
	
	void SetSoundPosition(const std::string& channelName, glm::vec3 position)
	{
		g_soundPositionQueue.push_back(std::pair<std::string, glm::vec3>(channelName, position));
	}
	
	void FadeInSound(const std::string& name, int ms, bool loop)
	{
		g_soundFadeInQueue.push_back({name, loop, ms, false, std::string()});
	}
	
	void FadeInSound(const std::string& name, const std::string& channelName, int ms, bool loop)
	{
		g_soundFadeInQueue.push_back({name, loop, ms, true, channelName});
	}
	
	void FadeOutSound(const std::string& channelName, int ms)
	{
		g_soundFadeOutQueue.push_back(std::pair<std::string, int>(channelName, ms));
	}
	
	bool ChannelExists(const std::string& channelName)
	{
		return g_channels.find(channelName) != g_channels.end();
	}
}