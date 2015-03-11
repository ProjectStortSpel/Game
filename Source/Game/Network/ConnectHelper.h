#ifndef CONNECTHELPER_H
#define CONNECTHELPER_H

#include "Network/PacketHandler.h"

#include <string>
#include <functional>

namespace ConnectHelper
{
	typedef std::function<void(std::string)> LoadGameModeHook;

	void Initialize();
	void Connect(std::string _gamemode);
	
	void SetName(const char* _name);
	void SetLoadGameModeHook(LoadGameModeHook _hook);
	//add callback to tell gamecreator to load the gamemode
}

#endif