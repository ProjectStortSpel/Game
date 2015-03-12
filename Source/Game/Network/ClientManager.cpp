#include "ClientManager.h"

#include "NetworkInstance.h"
#include "FileSystem/File.h"
#include "FileSystem/MD5.h"
#include "Game/ResourceManager.h"
#include "Game/HomePath.h"
#include "Game/LuaBridge/Network/LuaNetwork.h"
#include <SDL/SDL_mutex.h>
#include <SDL/SDL_thread.h>

#include <map>
#include <queue>
#include <sstream>

namespace ClientManager
{




#define FileChunkSize 10240
	enum State
	{
		Connecting = 1,
		Name = 2,
		Connected = 4
	};

	struct Job
	{
		ResourceManager::Resource resource;
		std::string packetType;
		Network::NetConnection nc;
	};

	inline State operator|(State a, State b)
	{
		return static_cast<State>(static_cast<int>(a) | static_cast<int>(b));
	}

	//struct Client
	//{
	//	State state;
	//	//Network::NetConnection nc;
	//};

	std::map<Network::NetConnection, State> clients;
	std::map<Network::NetConnection, std::string> names;
	std::queue<Job> jobs;
	SDL_Thread* jobThread;
	SDL_mutex* jobMutex;

	bool running = false;
	bool done = true;

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
			if (it->second & Connected)
				connectedClients.push_back(it->first);
		}
		return connectedClients;
	}

	std::string GetPlayerName(Network::NetConnection& _nc)
	{
		if (names.find(_nc) != names.end())
		{
			return names[_nc];
		}
		return "UntitledName";
	}

	std::string SetPlayerName(Network::NetConnection& _nc, const char* _name)
	{
		if (names.find(_nc) != names.end())
		{
			std::stringstream newName;
			newName << _name;

			int counter = 0;
			for (auto it = names.begin(); it != names.end(); ++it)
			{
				if (it->second == _name)
				{
					++counter;
					newName << counter;
					break;
				}
			}

			names[_nc] = newName.str();
			return newName.str();
		}
		else
			return "";
	}

	static int Upload(void* ptr)
	{
		SDL_LockMutex(jobMutex);
		bool empty = jobs.empty();
		SDL_UnlockMutex(jobMutex);

		Network::PacketHandler ph;

		while (!empty)
		{
			SDL_LockMutex(jobMutex);
			Job job = jobs.front();
			SDL_UnlockMutex(jobMutex);

			SDL_RWops* file;
			bool binary = FileSystem::File::IsBinary(job.resource.Location);

			FileSystem::File::Open(job.resource.Location, &file);
			unsigned char* fileData = (unsigned char*)FileSystem::File::Read(file, job.resource.Size);
			FileSystem::File::Close(file);

			int bytesLeft;

			unsigned char* data;
			std::string str;
			if (binary)
			{
				data = fileData;
				bytesLeft = job.resource.Size;
			}
			else
			{
				str = std::string((char*)fileData, job.resource.Size);

				//auto first = std::remove(str.begin(), str.end(), '\r');
				//int numRemoved = str.end() - first;
				//str.erase(first, str.end());

				std::string from = "\r\n";
				std::string to = "\n";

				size_t start_pos = 0;
				while ((start_pos = str.find(from, start_pos)) != std::string::npos)
				{
					str.replace(start_pos, from.length(), to);
					start_pos += to.length();
				}
				data = (unsigned char*)str.c_str();
				bytesLeft = str.size();
			}


			int currentPos = 0;
			bool firstPart = true;
			while (bytesLeft > 0)
			{
				uint64_t id = ph.StartPack(job.packetType.c_str());
				ph.WriteString(id, job.resource.File.c_str());

				if (firstPart)
				{
					ph.WriteByte(id, 1);
					firstPart = false;
				}
				else
					ph.WriteByte(id, 0);

				int size = bytesLeft > FileChunkSize ? FileChunkSize : bytesLeft;

				//last part?
				ph.WriteByte(id, size == bytesLeft);

				ph.WriteInt(id, size);
				ph.WriteBytes(id, data, size);

				data += size;

				NetworkInstance::GetServer()->Send(ph.EndPack(id), job.nc);

				bytesLeft -= size;
			}

			delete fileData;

			SDL_LockMutex(jobMutex);
			jobs.pop();
			empty = jobs.empty();
			SDL_UnlockMutex(jobMutex);

			for (int i = 0; i < 500; ++i)
			{
				if (!empty)
					break;

				SDL_Delay(10);

				SDL_LockMutex(jobMutex);
				empty = jobs.empty();
				SDL_UnlockMutex(jobMutex);
			}
		}
		done = true;
		return 0;
	}

	void Update()
	{
		if (!running && !jobs.empty())
		{
			running = true;
			done = false;
			jobMutex = SDL_CreateMutex();

			jobThread = SDL_CreateThread(Upload, "UploadThread", (void *)NULL);

			if (!jobThread)
			{
				SDL_DestroyMutex(jobMutex);
				running = false;
			}
		}
		else if (running && done)
		{
			int returnValue;
			SDL_WaitThread(jobThread, &returnValue);
			SDL_DestroyMutex(jobMutex);
			running = false;
		}
		//Upload();
	}

	void AcknowledgeName(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
	void RequestGameModeFileList(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
	void RequestGameModeFile(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
	void RequestContentFileList(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
	void RequestContentFile(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);
	void GameModeLoaded(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc);

	void Initialize()
	{
		//Network::NetMessageHook hook = std::bind(&NetworkGameMode, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		//NetworkInstance::GetClient()->AddNetworkHook("GameMode", hook);

		Network::NetMessageHook hook = std::bind(&AcknowledgeName, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NetworkInstance::GetServer()->AddNetworkHook("SEND_PLAYER_NAME", hook);

		hook = std::bind(&RequestGameModeFileList, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
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
	void AcknowledgeName(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
	{
		if (clients.find(_nc) != clients.end())
		{
			std::stringstream newName;
			clients[_nc] = clients[_nc] | Name;
			newName << _ph->ReadString(_id);
			int counter = 0;

			for (auto it = names.begin(); it != names.end(); ++it)
			{
				if (it->second == newName.str())
				{
					++counter;
					newName << counter;
					break;
				}
			}

			names[_nc] = newName.str();
		}
		else
		{
			names[_nc] = "NotConnectedPlayer";
			SDL_Log("Tried to set name on a player not connected!?");
		}

		//Search loadingscreen background
		std::vector<std::string> paths;
		paths.push_back(HomePath::GetSecondaryHomePath());
		std::vector<std::string> gmPaths = HomePath::GetGameModePaths(HomePath::Type::Server);

		for (int i = 0; i < gmPaths.size(); ++i)
			paths.push_back(gmPaths[i]);

		for (int i = 0; i < paths.size(); ++i)
		{
			std::string path = paths[i];
			path.append("loadingscreen.png");
			if (FileSystem::File::Exist(path))
			{
				//Send loadingscreen background
				SDL_RWops* file;
				FileSystem::File::Open(path, &file);
				Sint64 size = FileSystem::File::GetFileSize(file);
				unsigned char* fileData = (unsigned char*)FileSystem::File::Read(file, size);
				FileSystem::File::Close(file);

				int bytesLeft = size;

				unsigned char* data = fileData;


				int currentPos = 0;
				bool firstPart = true;
				while (bytesLeft > 0)
				{
					uint64_t id = _ph->StartPack("LoadingScreen");

					if (firstPart)
					{
						_ph->WriteByte(id, 1);
						firstPart = false;
					}
					else
						_ph->WriteByte(id, 0);

					int size = bytesLeft > FileChunkSize ? FileChunkSize : bytesLeft;

					//last part?
					_ph->WriteByte(id, size == bytesLeft);

					_ph->WriteInt(id, size);
					_ph->WriteBytes(id, data, size);

					data += size;

					NetworkInstance::GetServer()->Send(_ph->EndPack(id), _nc);

					bytesLeft -= size;
				}

				delete fileData;
				break;
			}
		}

		uint64_t id = _ph->StartPack("SERVER_ACKNOWLEDGE_NAME");
		NetworkInstance::GetServer()->Send(_ph->EndPack(id), _nc);

	}

	void RequestGameModeFileList(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
	{
		//Send filelist
		std::map<std::string, ResourceManager::Resource>* resources = ResourceManager::GetGamemodeResources();

		unsigned int numFiles = resources->size();
		bool firstPacket = true;
		bool newPacket = true;
		unsigned int filesLeftToPack = 0;
		uint64_t id;
		for (auto it = resources->begin(); it != resources->end(); ++it)
		{
			if (newPacket)
			{
				id = _ph->StartPack("GameModeFileList");

				//Is this the first packet?
				_ph->WriteByte(id, firstPacket);

				filesLeftToPack = numFiles > 50 ? 50 : numFiles;
				numFiles -= filesLeftToPack;

				//number of files in this packet
				_ph->WriteInt(id, filesLeftToPack);

				newPacket = false;
				firstPacket = false;
			}

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

			--filesLeftToPack;
			if (filesLeftToPack == 0)
			{
				//Is this the last packet?
				_ph->WriteByte(id, numFiles == 0);
				NetworkInstance::GetServer()->Send(_ph->EndPack(id), _nc);
				newPacket = true;
			}
		}
	}

	void RequestGameModeFile(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
	{
		std::map<std::string, ResourceManager::Resource>* resources = ResourceManager::GetGamemodeResources();

		std::string filename = _ph->ReadString(_id);
		ResourceManager::Resource resource;

		if (resources->find(filename) != resources->end())
		{

			resource = resources->at(filename);

			Job job;
			job.resource = resource;
			job.packetType = "GameModeFile";
			job.nc = _nc;

			if (running)
			{
				SDL_LockMutex(jobMutex);
				jobs.push(job);
				SDL_UnlockMutex(jobMutex);
			}
			else
				jobs.push(job);

			/*SDL_RWops* file;
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
			}*/
		}
	}

	void RequestContentFileList(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
	{
		//Send filelist
		std::map<std::string, ResourceManager::Resource>* resources = ResourceManager::GetContentResources();
		
		unsigned int numFiles = resources->size();
		bool firstPacket = true;
		bool newPacket = true;
		unsigned int filesLeftToPack = 0;
		uint64_t id;
		for (auto it = resources->begin(); it != resources->end(); ++it)
		{
			if (newPacket)
			{
				id = _ph->StartPack("ContentFileList");

				//Is this the first packet?
				_ph->WriteByte(id, firstPacket);

				filesLeftToPack = numFiles > 50 ? 50 : numFiles;
				numFiles -= filesLeftToPack;

				//number of files in this packet
				_ph->WriteInt(id, filesLeftToPack);

				newPacket = false;
				firstPacket = false;
			}

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

			--filesLeftToPack;
			if (filesLeftToPack == 0)
			{
				//Is this the last packet?
				_ph->WriteByte(id, numFiles == 0);
				NetworkInstance::GetServer()->Send(_ph->EndPack(id), _nc);
				newPacket = true;
			}
		}
	}

	void RequestContentFile(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
	{
		std::map<std::string, ResourceManager::Resource>* resources = ResourceManager::GetContentResources();

		std::string filename = _ph->ReadString(_id);
		ResourceManager::Resource resource;

		if (resources->find(filename) != resources->end())
		{

			resource = resources->at(filename);

			Job job;
			job.resource = resource;
			job.packetType = "ContentFile";
			job.nc = _nc;

			if (running)
			{
				SDL_LockMutex(jobMutex);
				jobs.push(job);
				SDL_UnlockMutex(jobMutex);
			}
			else
				jobs.push(job);

			/*SDL_RWops* file;
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
			}*/
		}
	}

	void GameModeLoaded(Network::PacketHandler* _ph, uint64_t& _id, Network::NetConnection& _nc)
	{
		if (clients.find(_nc) != clients.end())
		{
			int hest = clients[_nc];
			if (clients[_nc] & Name)
				clients[_nc] = Connected | Name;
			else
				SDL_Log("Connecting without setting name.");

			for (int i = 0; i < LuaBridge::LuaNetworkEvents::g_onPlayerConnected.size(); ++i)
			{
				LuaBridge::LuaNetworkEvents::g_onPlayerConnected[i](_nc, "");
			}
		}
	}
}