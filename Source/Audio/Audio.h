#ifndef AUDIO_H
#define AUDIO_H

#if defined(WIN32) && defined(_DEBUG)
#include <VLD/vld.h>
#endif
#include <SDL/SDL.h>
#include <string>
#include <glm/glm.hpp>

namespace Audio
{
	void DECLSPEC Init();
	void DECLSPEC Update();
	void DECLSPEC Quit();
	
	void DECLSPEC SetVolume(int volume);
	
	void DECLSPEC SetDistance(float near, float far);
	void DECLSPEC SetCameraPosition(glm::vec3 position);
	
	void DECLSPEC LoadMusic(const std::string& filepath);
	void DECLSPEC PlayMusic();
	void DECLSPEC PauseMusic();
	void DECLSPEC ResumeMusic();
	void DECLSPEC StopMusic();
	void DECLSPEC FadeInMusic(int ms);
	void DECLSPEC FadeOutMusic(int ms);
	
	void DECLSPEC LoadSound(const std::string& name, const std::string& filepath);
	void DECLSPEC PlaySound(const std::string& name, bool loop = false);
	void DECLSPEC PlaySound(const std::string& name, const std::string channelName, bool loop = false);
	void DECLSPEC PauseSound(const std::string& channelName);
	void DECLSPEC ResumeSound(const std::string& channelName);
	void DECLSPEC StopSound(const std::string& channelName);
	void DECLSPEC SetSoundPosition(const std::string& channelName, glm::vec3 position);
	void DECLSPEC FadeInSound(const std::string &name, int ms, bool loop = false);
	void DECLSPEC FadeInSound(const std::string &name, const std::string& channelName, int ms, bool loop = false);
	void DECLSPEC FadeOutSound(const std::string& channelName, int ms);
	void DECLSPEC SetVolume(const std::string& channelName, int volume);
	
	bool DECLSPEC ChannelExists(const std::string& channelName);
}

#endif