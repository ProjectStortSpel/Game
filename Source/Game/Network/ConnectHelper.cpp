#include "ConnectHelper.h"

#include "NetworkInstance.h"
#include "FileSystem/File.h"
#include "FileSystem/MD5.h"
#include "Game/ResourceManager.h"
#include "Game/HomePath.h"

#include "Console/Console.h"

#include <map>
#include <sstream>

namespace ConnectHelper
{
	enum State
	{
		//Password, 
		//GameMode, 
		GameModeFileList,
		GameModeFiles,
		ContentFileList,
		ContentFiles,
		Done
	};

	State state;
	std::string gamemode;
	std::map<std::string, ResourceManager::Resource> missingFiles;

	LoadGameModeHook loadGameModeHook;

	unsigned int bytesToDownload;
	unsigned int bytesDownloaded;
	char percent;

	void BindNetworkEvents();
	void Reset();

	//void NetworkGameMode(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
	void NetworkGameModeFileList(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
	void NetworkGameModeFile(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
	void NetworkContentFileList(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
	void NetworkContentFile(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);

	void Initialize()
	{
		//Network::NetMessageHook hook = std::bind(&NetworkGameMode, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		//NetworkInstance::GetClient()->AddNetworkHook("GameMode", hook);

		Network::NetMessageHook hook = std::bind(&NetworkGameModeFileList, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
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
		gamemode = _gamemode;
		missingFiles.clear();

		

		//Request Gamemode File List
		Network::PacketHandler* ph = NetworkInstance::GetClient()->GetPacketHandler();
		uint64_t id = ph->StartPack("RequestGameModeFileList");
		NetworkInstance::GetClient()->Send(ph->EndPack(id));

		state = GameModeFileList;
	}

	void SetLoadGameModeHook(LoadGameModeHook _hook)
	{
		loadGameModeHook = _hook;
	}

	/*void NetworkGameMode(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
	{

	}*/

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
				ResourceManager::CreateResource(gamemode, filename, r, HomePath::Type::Client);

				if (md5 != r.MD5)
				{
					//SDL_Log("I don't have: %s", filename.c_str());

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
				Console::ConsoleManager::GetInstance().AddToCommandQueue("SetLoadingText \"Downloading Gamemode: 0%\"");

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
				//md5 check and delete if failed
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
						ss << "SetLoadingText \"Downloading Gamemode: " << p << "%\"";
						Console::ConsoleManager::GetInstance().AddToCommandQueue(ss.str().c_str());
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
				ss << "SetLoadingText \"Downloading Gamemode: " << p << "%\"";
				Console::ConsoleManager::GetInstance().AddToCommandQueue(ss.str().c_str());
			}
		}
	}

	void NetworkContentFileList(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
	{
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
				ResourceManager::CreateResource(gamemode, filename, r, HomePath::Type::Client);

				if (md5 != r.MD5)
				{
					SDL_Log("I don't have: %s", filename.c_str());

					FileSystem::MD5::MD5_Print(md5);
					FileSystem::MD5::MD5_Print(r.MD5);

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
				
				if (!NetworkInstance::GetServer()->IsRunning())
					loadGameModeHook(gamemode);

				Network::PacketHandler* ph = NetworkInstance::GetClient()->GetPacketHandler();
				uint64_t id = ph->StartPack("GameModeLoaded");
				NetworkInstance::GetClient()->Send(ph->EndPack(id));
				state = Done;
			}
			else
			{
				bytesDownloaded = 0;
				percent = 0;
				Console::ConsoleManager::GetInstance().AddToCommandQueue("SetLoadingText \"Downloading Content: 0%\"");

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
				//md5 check and delete if failed
				missingFiles.erase(filename);

				if (missingFiles.empty())
				{
					//Load GameMode
					if (!NetworkInstance::GetServer()->IsRunning())
						loadGameModeHook(gamemode);

					Network::PacketHandler* ph = NetworkInstance::GetClient()->GetPacketHandler();
					uint64_t id = ph->StartPack("GameModeLoaded");
					NetworkInstance::GetClient()->Send(ph->EndPack(id));
					state = Done;
				}
				else
				{
					if (p != percent)
					{
						percent = p;
						std::stringstream ss;
						ss << "SetLoadingText \"Downloading Content: " << p << "%\"";
						Console::ConsoleManager::GetInstance().AddToCommandQueue(ss.str().c_str());
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
				ss << "SetLoadingText \"Downloading Content: " << p << "%\"";
				Console::ConsoleManager::GetInstance().AddToCommandQueue(ss.str().c_str());
			}
		}
	}


}