#include "ConnectHelper.h"

#include "NetworkInstance.h"
#include "FileSystem/File.h"
#include "FileSystem/MD5.h"
#include "Console/Console.h"
#include "Game/ResourceManager.h"
#include "Game/HomePath.h"

#include "Game/LoadingScreen.h"

#include <map>
#include <sstream>

namespace ConnectHelper
{
	enum State
	{
		//Password, 
		//GameMode,
		NameOrLoadingScreen,
		AcknowledgeName,
		GameModeFileList,
		GameModeFiles,
		ContentFileList,
		ContentFiles,
		Done
	};

	State state;
	std::string gamemode;
	std::string name = "DefaultName";
	std::map<std::string, ResourceManager::Resource> missingFiles;

	LoadGameModeHook loadGameModeHook;

	unsigned int bytesToDownload;
	unsigned int bytesDownloaded;
	char percent;

	void BindNetworkEvents();
	void Reset();

	void NetworkLoadingScreen(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
	void NetworkAcknowledgeName(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
	//void NetworkGameMode(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
	void NetworkGameModeFileList(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
	void NetworkGameModeFile(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
	void NetworkContentFileList(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
	void NetworkContentFile(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);

	void LoadGameMode()
	{
		if (!NetworkInstance::GetServer()->IsRunning())
			loadGameModeHook(gamemode);
		else
			LoadingScreen::GetInstance().SetInactive(0);

		Network::PacketHandler* ph = NetworkInstance::GetClient()->GetPacketHandler();
		uint64_t id = ph->StartPack("GameModeLoaded");
		NetworkInstance::GetClient()->Send(ph->EndPack(id));
		state = Done;
	}

	void Initialize()
	{
		//Network::NetMessageHook hook = std::bind(&NetworkGameMode, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		//NetworkInstance::GetClient()->AddNetworkHook("GameMode", hook);

		Network::NetMessageHook hook = std::bind(&NetworkLoadingScreen, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NetworkInstance::GetClient()->AddNetworkHook("LoadingScreen", hook); 
		
		hook = std::bind(&NetworkAcknowledgeName, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NetworkInstance::GetClient()->AddNetworkHook("SERVER_ACKNOWLEDGE_NAME", hook);

		hook = std::bind(&NetworkGameModeFileList, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NetworkInstance::GetClient()->AddNetworkHook("GameModeFileList", hook);

		hook = std::bind(&NetworkGameModeFile, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NetworkInstance::GetClient()->AddNetworkHook("GameModeFile", hook);

		hook = std::bind(&NetworkContentFileList, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NetworkInstance::GetClient()->AddNetworkHook("ContentFileList", hook);

		hook = std::bind(&NetworkContentFile, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NetworkInstance::GetClient()->AddNetworkHook("ContentFile", hook);
	}

	void Connect(std::string _gamemode)
	{
		LoadingScreen::GetInstance().SetActive();
		LoadingScreen::GetInstance().SetLoadingText("Syncing files with the server.");
		gamemode = _gamemode;
		missingFiles.clear();

		Network::PacketHandler* ph = NetworkInstance::GetClient()->GetPacketHandler();
		uint64_t id = ph->StartPack("SEND_PLAYER_NAME");
		ph->WriteString(id, name.c_str());
		NetworkInstance::GetClient()->Send(ph->EndPack(id));

		state = NameOrLoadingScreen;
	}

	void SetLoadGameModeHook(LoadGameModeHook _hook)
	{
		loadGameModeHook = _hook;
	}

	void SetName(const char* _name)
	{
		name = _name;
	}

	/*void NetworkGameMode(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
	{

	}*/

	void NetworkLoadingScreen(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
	{
		if (state == NameOrLoadingScreen)
		{
			std::string path = HomePath::GetDownloadHomePath();
			path.append("loadingscreen.png");

			char firstPart = _ph->ReadByte(_id);
			char lastPart = _ph->ReadByte(_id);

			int size = _ph->ReadInt(_id);

			unsigned char* data = _ph->ReadBytes(_id, size);

			if (firstPart == 1)
			{
				LoadingScreen::GetInstance().SetLoadingText("Downloading Loadingscreen background.");
				FileSystem::File::Create(path);
			}

			SDL_RWops* file;
			FileSystem::File::Append(path, &file);
			FileSystem::File::Write(file, data, size);
			Sint64 fileSize = FileSystem::File::GetFileSize(file);
			FileSystem::File::Close(file);

			if (lastPart == 1)
			{
				LoadingScreen::GetInstance().SetLoadingText(" ");
				LoadingScreen::GetInstance().SetBackground(path);
				SDL_Log("Downloaded loadingscreen.");

				state = AcknowledgeName;
			}
		}
	}

	void NetworkAcknowledgeName(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
	{
		if (state == AcknowledgeName || state == NameOrLoadingScreen)
		{
			//Request Gamemode File List
			Network::PacketHandler* ph = NetworkInstance::GetClient()->GetPacketHandler();
			uint64_t id = ph->StartPack("RequestGameModeFileList");
			NetworkInstance::GetClient()->Send(ph->EndPack(id));

			state = GameModeFileList;
		}
	}

	void NetworkGameModeFileList(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
	{
		if (state == GameModeFileList)
		{
			bytesToDownload = 0;
			int numFiles = _ph->ReadInt(_id);

			for (int i = 0; i < numFiles; ++i)
			{
				std::string filename = _ph->ReadString(_id);

				int size = _ph->ReadInt(_id);

				FileSystem::MD5::MD5Data md5;
				for (int j = 0; j < 16; ++j)
				{
					md5.data[j] = _ph->ReadByte(_id);
				}

				ResourceManager::Resource r;
				bool hasFile = ResourceManager::CreateResource(gamemode, filename, r, HomePath::Type::Client, false);

				if (!hasFile || md5 != r.MD5)
				{

					SDL_Log("Missing gamemode file: %s", filename.c_str());

#if defined(__IOS__) && !defined(__DevMode__)
					//Don't download luafiles on IOS.

					//Kolla om sista 4 teknerna i r.File == ".lua"

					//Console::ConsoleManager::GetInstance().AddToCommandQueue("disconnect;stop;gamemode lobby");
					//return;
#endif


					ResourceManager::Resource temp;
					temp.File = filename;
					temp.Location = HomePath::GetDownloadGameModePath(gamemode);
					temp.Location.append(filename);
					temp.Size = size;
					temp.MD5 = md5;

					bytesToDownload += size;

					missingFiles[filename] = temp;

					//IOS DISCONNECT
				}
				/*else
				{
				SDL_Log("I have: %s", filename.c_str());
				}*/
			}

			if (missingFiles.empty())
			{
				//Request Content File List
				Network::PacketHandler* ph = NetworkInstance::GetClient()->GetPacketHandler();
				uint64_t id = ph->StartPack("RequestContentFileList");
				NetworkInstance::GetClient()->Send(ph->EndPack(id));

				state = ContentFileList;
			}
			else
			{
				bytesDownloaded = 0;
				percent = 0;
				LoadingScreen::GetInstance().SetLoadingText("Downloading Gamemode: 0%");


				Network::PacketHandler* ph = NetworkInstance::GetClient()->GetPacketHandler();
				uint64_t id = ph->StartPack("RequestGameModeFile");

				ph->WriteString(id, missingFiles.begin()->second.File.c_str());


				//ph->WriteInt(id, missingFiles.size());

				//for (auto it = missingFiles.begin(); it != missingFiles.end(); ++it)
				//{
				//	//Filename
				//	ph->WriteString(id, it->second.File.c_str());
				//}
				NetworkInstance::GetClient()->Send(ph->EndPack(id));
				state = GameModeFiles;
			}
		}

	}

	void NetworkGameModeFile(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
	{
		if (state == GameModeFiles)
		{
			std::string filename = _ph->ReadString(_id);
			if (missingFiles.find(filename) == missingFiles.end())
				return;

			char firstPart = _ph->ReadByte(_id);
			char lastPart = _ph->ReadByte(_id);

			ResourceManager::Resource r = missingFiles[filename];

			int size = _ph->ReadInt(_id);

			bytesDownloaded += size;

			int p = (int)(((float)bytesDownloaded / (float)bytesToDownload) * 100);

			unsigned char* data = _ph->ReadBytes(_id, size);

			if (firstPart == 1)
				FileSystem::File::Create(r.Location);

			SDL_RWops* file;
			FileSystem::File::Append(r.Location, &file);
			FileSystem::File::Write(file, data, size);
			Sint64 fileSize = FileSystem::File::GetFileSize(file);
			FileSystem::File::Close(file);

			if (lastPart == 1)
			{
				FileSystem::MD5::MD5Data MD5 = FileSystem::MD5::MD5_File(r.Location);
				if (r.MD5 != MD5)
				{
					FileSystem::File::Delete(r.Location);
					SDL_Log("Failed to download gamemode file (md5 mismatch): %s", filename.c_str());
					FileSystem::MD5::MD5_Print(r.MD5);
					FileSystem::MD5::MD5_Print(MD5);
					Console::ConsoleManager::GetInstance().AddToCommandQueue("disconnect;stop;gamemode lobby", false);
					return;
				}

				SDL_Log("Downloaded gamemode file: %s", filename.c_str());

				missingFiles.erase(filename);

				if (missingFiles.empty())
				{
					//Request Content File List
					Network::PacketHandler* ph = NetworkInstance::GetClient()->GetPacketHandler();
					uint64_t id = ph->StartPack("RequestContentFileList");
					NetworkInstance::GetClient()->Send(ph->EndPack(id));

					state = ContentFileList;
				}
				else
				{
					if (p != percent)
					{
						percent = p;
						std::stringstream ss;
						ss << "Downloading Gamemode: " << p << "%";
						LoadingScreen::GetInstance().SetLoadingText(ss.str().c_str());
					}

					Network::PacketHandler* ph = NetworkInstance::GetClient()->GetPacketHandler();
					uint64_t id = ph->StartPack("RequestGameModeFile");
					ph->WriteString(id, missingFiles.begin()->second.File.c_str());
					NetworkInstance::GetClient()->Send(ph->EndPack(id));
				}
			}
			else if (p != percent)
			{
				percent = p;
				std::stringstream ss;
				ss << "Downloading Gamemode: " << p << "%";
				LoadingScreen::GetInstance().SetLoadingText(ss.str().c_str());
			}
		}
	}

	void NetworkContentFileList(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
	{
		SDL_Log("NetworkContentFileList");
		if (state == ContentFileList)
		{

			bytesToDownload = 0;
			int numFiles = _ph->ReadInt(_id);

			for (int i = 0; i < numFiles; ++i)
			{
				std::string filename = _ph->ReadString(_id);

				int size = _ph->ReadInt(_id);

				FileSystem::MD5::MD5Data md5;
				for (int j = 0; j < 16; ++j)
				{
					md5.data[j] = _ph->ReadByte(_id);
				}

				ResourceManager::Resource r;
				bool hasFile = ResourceManager::CreateResource(gamemode, filename, r, HomePath::Type::Client, true);


				if (!hasFile || md5 != r.MD5)
				{
					SDL_Log("Missing content file: %s", filename.c_str());


#if defined(__IOS__) && !defined(__DevMode__)
					//Don't download luafiles on IOS.

					//Kolla om sista 4 teknerna i r.File == ".lua"

					//Console::ConsoleManager::GetInstance().AddToCommandQueue("disconnect;stop;gamemode lobby");
					//return;
#endif

					ResourceManager::Resource temp;
					temp.File = filename;
					temp.Location = HomePath::GetDownloadHomePath();
					temp.Location.append(filename);
					temp.Size = size;
					temp.MD5 = md5;

					bytesToDownload += size;

					missingFiles[filename] = temp;

					//IOS DISCONNECT?
				}
				/*else
				{
				SDL_Log("I have: %s", filename.c_str());
				}*/
			}

			if (missingFiles.empty())
			{
				//Load GameMode
				LoadGameMode();
			}
			else
			{
				bytesDownloaded = 0;
				percent = 0;
				LoadingScreen::GetInstance().SetLoadingText("Downloading Content: 0%");

				Network::PacketHandler* ph = NetworkInstance::GetClient()->GetPacketHandler();
				uint64_t id = ph->StartPack("RequestContentFile");

				ph->WriteString(id, missingFiles.begin()->second.File.c_str());

				NetworkInstance::GetClient()->Send(ph->EndPack(id));
				state = ContentFiles;
			}
		}

	}

	void NetworkContentFile(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
	{
		if (state == ContentFiles)
		{

			std::string filename = _ph->ReadString(_id);
			if (missingFiles.find(filename) == missingFiles.end())
				return;

			char firstPart = _ph->ReadByte(_id);
			char lastPart = _ph->ReadByte(_id);

			ResourceManager::Resource r = missingFiles[filename];

			int size = _ph->ReadInt(_id);

			bytesDownloaded += size;

			int p = (int)(((float)bytesDownloaded / (float)bytesToDownload) * 100);

			unsigned char* data = _ph->ReadBytes(_id, size);

			if (firstPart == 1)
				FileSystem::File::Create(r.Location);

			SDL_RWops* file;
			FileSystem::File::Append(r.Location, &file);
			FileSystem::File::Write(file, data, size);
			FileSystem::File::Close(file);

			if (lastPart == 1)
			{
				FileSystem::MD5::MD5Data MD5 = FileSystem::MD5::MD5_File(r.Location);
				if (r.MD5 != MD5)
				{
					FileSystem::File::Delete(r.Location);
					SDL_Log("Failed to download content file (md5 mismatch): %s", filename.c_str());
					FileSystem::MD5::MD5_Print(r.MD5);
					FileSystem::MD5::MD5_Print(MD5);
					Console::ConsoleManager::GetInstance().AddToCommandQueue("disconnect;stop;gamemode lobby", false);
					return;
				}

				SDL_Log("Downloaded content file: %s", filename.c_str());


				missingFiles.erase(filename);

				if (missingFiles.empty())
				{
					//Load GameMode
					LoadGameMode();
				}
				else
				{
					if (p != percent)
					{
						percent = p;
						std::stringstream ss;
						ss << "Downloading Content: " << p << "%";
						LoadingScreen::GetInstance().SetLoadingText(ss.str().c_str());
					}

					Network::PacketHandler* ph = NetworkInstance::GetClient()->GetPacketHandler();
					uint64_t id = ph->StartPack("RequestContentFile");
					ph->WriteString(id, missingFiles.begin()->second.File.c_str());
					NetworkInstance::GetClient()->Send(ph->EndPack(id));
				}
			}
			else if (p != percent)
			{
				percent = p;
				std::stringstream ss;
				ss << "Downloading Content: " << p << "%";
				LoadingScreen::GetInstance().SetLoadingText(ss.str().c_str());
			}
		}
	}

}