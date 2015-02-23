#include "ClientManager.h"

#include "NetworkInstance.h"
#include "FileSystem/File.h"
#include "FileSystem/MD5.h"
#include "Game/ResourceManager.h"
#include "Game/HomePath.h"
#include "Game/LuaBridge/Network/LuaNetwork.h"

#include <map>

namespace ClientManager
{
	enum State
	{
		Connecting,
		Connected
	};

	//struct Client
	//{
	//	State state;
	//	//Network::NetConnection nc;
	//};

	std::map<Network::NetConnection, State> clients;
#define FileChunkSize 1024

	void NewClient(Network::NetConnection _client)
	{
		clients[_client] = Connecting;
	}

	void ClientDisconnected(Network::NetConnection _client)
	{
		clients.erase(_client);
	}

	void SetAllClientsToConnecting()
	{
		for (auto it = clients.begin(); it != clients.end(); ++it)
		{
			it->second = Connecting;
		}
	}

	std::vector<Network::NetConnection> GetConnectedClients()
	{
		std::vector<Network::NetConnection> connectedClients;
		for (auto it = clients.begin(); it != clients.end(); ++it)
		{
			if (it->second == Connected)
				connectedClients.push_back(it->first);
		}
		return connectedClients;
	}


	void RequestGameModeFileList(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
	void RequestGameModeFile(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
	void RequestContentFileList(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
	void RequestContentFile(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
	void GameModeLoaded(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);

	void Initialize()
	{
		//Network::NetMessageHook hook = std::bind(&NetworkGameMode, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		//NetworkInstance::GetClient()->AddNetworkHook("GameMode", hook);

		Network::NetMessageHook hook = std::bind(&RequestGameModeFileList, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NetworkInstance::GetServer()->AddNetworkHook("RequestGameModeFileList", hook);

		hook = std::bind(&RequestGameModeFile, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NetworkInstance::GetServer()->AddNetworkHook("RequestGameModeFile", hook);

		hook = std::bind(&RequestContentFileList, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NetworkInstance::GetServer()->AddNetworkHook("RequestContentFileList", hook);

		hook = std::bind(&RequestContentFile, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NetworkInstance::GetServer()->AddNetworkHook("RequestContentFile", hook);

		hook = std::bind(&GameModeLoaded, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NetworkInstance::GetServer()->AddNetworkHook("GameModeLoaded", hook);
	}

	void RequestGameModeFileList(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
	{
		//Send filelist
		std::map<std::string, ResourceManager::Resource>* resources = ResourceManager::GetGamemodeResources();
		uint64_t id = _ph->StartPack("GameModeFileList");

		//number of files
		_ph->WriteInt(id, resources->size());
		for (auto it = resources->begin(); it != resources->end(); ++it)
		{
			ResourceManager::Resource* r = &it->second;

			//Filename
			_ph->WriteString(id, r->File.c_str());
			//Filesize
			_ph->WriteInt(id, r->Size);

			//MD5
			for (int j = 0; j < 16; ++j)
			{
				_ph->WriteByte(id, r->MD5.data[j]);
			}
		}
		NetworkInstance::GetServer()->Send(_ph->EndPack(id), _nc);

	}

	void RequestGameModeFile(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
	{
		std::map<std::string, ResourceManager::Resource>* resources = ResourceManager::GetGamemodeResources();

		std::string filename = _ph->ReadString(_id);
		ResourceManager::Resource resource;

		if (resources->find(filename) != resources->end())
		{
			
			resource = resources->at(filename);

			SDL_RWops* file;
			FileSystem::File::Open(resource.Location, &file);

			int bytesLeft = resource.Size;
			bool firstPart = true;
			while (bytesLeft > 0)
			{
				uint64_t id = _ph->StartPack("GameModeFile");
				_ph->WriteString(id, resource.File.c_str());

				if (firstPart)
				{
					_ph->WriteByte(id, 1);
					firstPart = false;
				}
				else
					_ph->WriteByte(id, 0);

				int size = bytesLeft > FileChunkSize ? FileChunkSize : bytesLeft;

				_ph->WriteInt(id, size);

				unsigned char* data = (unsigned char*)FileSystem::File::Read(file, size);

				_ph->WriteBytes(id, data, size);

				delete data;

				NetworkInstance::GetServer()->Send(_ph->EndPack(id), _nc);

				bytesLeft -= size;
			}
		}
	}

	void RequestContentFileList(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
	{
		//Send filelist
		std::map<std::string, ResourceManager::Resource>* resources = ResourceManager::GetContentResources();
		uint64_t id = _ph->StartPack("ContentFileList");

		//number of files
		_ph->WriteInt(id, resources->size());
		for (auto it = resources->begin(); it != resources->end(); ++it)
		{
			ResourceManager::Resource* r = &it->second;

			//Filename
			_ph->WriteString(id, r->File.c_str());
			//Filesize
			_ph->WriteInt(id, r->Size);

			//MD5
			for (int j = 0; j < 16; ++j)
			{
				_ph->WriteByte(id, r->MD5.data[j]);
			}
		}
		NetworkInstance::GetServer()->Send(_ph->EndPack(id), _nc);
	}

	void RequestContentFile(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
	{
		std::map<std::string, ResourceManager::Resource>* resources = ResourceManager::GetContentResources();

		std::string filename = _ph->ReadString(_id);
		ResourceManager::Resource resource;

		if (resources->find(filename) != resources->end())
		{

			resource = resources->at(filename);

			SDL_RWops* file;
			FileSystem::File::Open(resource.Location, &file);

			int bytesLeft = resource.Size;
			bool firstPart = true;
			while (bytesLeft > 0)
			{
				uint64_t id = _ph->StartPack("ContentFile");
				_ph->WriteString(id, resource.File.c_str());

				if (firstPart)
				{
					_ph->WriteByte(id, 1);
					firstPart = false;
				}
				else
					_ph->WriteByte(id, 0);

				int size = bytesLeft > FileChunkSize ? FileChunkSize : bytesLeft;

				_ph->WriteInt(id, size);

				unsigned char* data = (unsigned char*)FileSystem::File::Read(file, size);

				_ph->WriteBytes(id, data, size);

				delete data;

				NetworkInstance::GetServer()->Send(_ph->EndPack(id), _nc);

				bytesLeft -= size;
			}
		}
	}

	void GameModeLoaded(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
	{
		if (clients.find(_nc) != clients.end())
		{

			clients[_nc] = Connected;

			for (int i = 0; i < LuaBridge::LuaNetworkEvents::g_onPlayerConnected.size(); ++i)
			{
				LuaBridge::LuaNetworkEvents::g_onPlayerConnected[i](_nc, "");
			}
		}
	}
}