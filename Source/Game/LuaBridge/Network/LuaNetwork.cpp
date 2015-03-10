#include "LuaNetwork.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "../../Network/NetworkInstance.h"
#include "Game/Network/ClientManager.h"
#include <sstream>

namespace LuaBridge
{

	namespace LuaNetwork
	{

		lua_State* g_clientLuaState;
		lua_State* g_serverLuaState;

		//Shared
		int Receive(lua_State* L);

		int StartPack(lua_State* L);

		int Update(lua_State* L);

		int ResetNetworkMaps(lua_State* L);

		//int WriteByte();
		int WriteFloat(lua_State* L);
		//int WriteShort();
		int WriteString(lua_State* L);
		int WriteBool(lua_State* L);
		int WriteInt(lua_State* L);

		//int ReadByte();
		int ReadFloat(lua_State* L);
		//int ReadShort();
		int ReadString(lua_State* L);
		int ReadBool(lua_State* L);
		int ReadInt(lua_State* L);

		//Client
		int SendToServer(lua_State* L);
		int Connect(lua_State* L);
		int Disconnect(lua_State* L);
		int IsConnected(lua_State* L);
		int Name(lua_State* L);
		int ToClientID(lua_State* L);
		int ToServerID(lua_State* L);

		//Server
		int Send(lua_State* L);
		int Broadcast(lua_State* L);

		int Host(lua_State* L);
		int Stop(lua_State* L);
		int IsRunning(lua_State* L);
		int Kick(lua_State* L);
		int MaxConnections(lua_State* L);
		int ConnectedClients(lua_State* L);
		int GetPlayerName(lua_State* L);

		int SendEntity(lua_State* L);
		int SendEntityKill(lua_State* L);
		int BroadcastEntity(lua_State* L);
		int BroadcastEntityKill(lua_State* L);


		void Embed(lua_State* L)
		{
			//init global bools
			LuaEmbedder::AddBool(L, "Client", false);
			LuaEmbedder::AddBool(L, "Server", false);

			//Shared
			LuaEmbedder::AddFunction(L, "Receive", &Receive, "Net");

			LuaEmbedder::AddFunction(L, "StartPack", &StartPack, "Net");

			LuaEmbedder::AddFunction(L, "Update", &Update, "Net");

			LuaEmbedder::AddFunction(L, "ResetNetworkMaps", &ResetNetworkMaps, "Net");



			//LuaEmbedder::AddFunction("WriteByte", &WriteByte, "Net");
			LuaEmbedder::AddFunction(L, "WriteFloat", &WriteFloat, "Net");
			//LuaEmbedder::AddFunction("WriteShort", &WriteShort, "Net");
			LuaEmbedder::AddFunction(L, "WriteString", &WriteString, "Net");
			LuaEmbedder::AddFunction(L, "WriteBool", &WriteBool, "Net");
			LuaEmbedder::AddFunction(L, "WriteInt", &WriteInt, "Net");

			//LuaEmbedder::AddFunction("ReadByte", &ReadByte, "Net");
			LuaEmbedder::AddFunction(L, "ReadFloat", &ReadFloat, "Net");
			//LuaEmbedder::AddFunction("ReadShort", &ReadShort, "Net");
			LuaEmbedder::AddFunction(L, "ReadString", &ReadString, "Net");
			LuaEmbedder::AddFunction(L, "ReadBool", &ReadBool, "Net");
			LuaEmbedder::AddFunction(L, "ReadInt", &ReadInt, "Net");


			//Client
			LuaEmbedder::AddFunction(L, "SendToServer", &SendToServer, "Net");
			LuaEmbedder::AddFunction(L, "Connect", &Connect, "Net");
			LuaEmbedder::AddFunction(L, "Disconnect", &Disconnect, "Net");
			LuaEmbedder::AddFunction(L, "IsConnected", &IsConnected, "Net");
			LuaEmbedder::AddFunction(L, "Name", &Name, "Net");

			LuaEmbedder::AddFunction(L, "ToServerID", &ToServerID, "Net");
			LuaEmbedder::AddFunction(L, "ToClientID", &ToClientID, "Net");

			//Server
			LuaEmbedder::AddFunction(L, "Send", &Send, "Net");
			LuaEmbedder::AddFunction(L, "Broadcast", &Broadcast, "Net");

			LuaEmbedder::AddFunction(L, "Host", &Host, "Net");
			LuaEmbedder::AddFunction(L, "Stop", &Stop, "Net");
			LuaEmbedder::AddFunction(L, "IsRunning", &IsRunning, "Net");
			LuaEmbedder::AddFunction(L, "Kick", &Kick, "Net");
			LuaEmbedder::AddFunction(L, "MaxConnections", &MaxConnections, "Net");
			LuaEmbedder::AddFunction(L, "ConnectedClients", &ConnectedClients, "Net"); 
			LuaEmbedder::AddFunction(L, "GetPlayerName", &GetPlayerName, "Net");
			LuaEmbedder::AddFunction(L, "GetPlayerName", &GetPlayerName, "Net");

			LuaEmbedder::AddFunction(L, "SendEntity", &SendEntity, "Net");
			LuaEmbedder::AddFunction(L, "SendEntityKill", &SendEntityKill, "Net");
			LuaEmbedder::AddFunction(L, "BroadcastEntity", &BroadcastEntity, "Net");
			LuaEmbedder::AddFunction(L, "BroadcastEntityKill", &BroadcastEntityKill, "Net");
		}

		void SetClientLuaState(lua_State* L)
		{
			g_clientLuaState = L;
		}

		void SetServerLuaState(lua_State* L)
		{
			g_serverLuaState = L;
		}

		//Shared

		Network::PacketHandler* GetPacketHandler(lua_State* L)
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			Network::ClientNetwork* client = NetworkInstance::GetClient();

			Network::PacketHandler* ph = NULL;

			lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;

			if (server->IsRunning() && parent == g_serverLuaState)
			{
				ph = server->GetPacketHandler();
			}
			else if (client->IsConnected() && parent == g_clientLuaState)
			{
				ph = client->GetPacketHandler();
			}
			return ph;
		}

		int Receive(lua_State* L)
		{
			std::string name = LuaEmbedder::PullString(L, 1);
			LuaEmbedder::SaveFunction(L, 2, name);
			return 0;
		}

		int StartPack(lua_State* L)
		{
			Network::PacketHandler *ph = GetPacketHandler(L);

			if (!ph)
				return 0;

			std::string identifier = LuaEmbedder::PullString(L, 1);
			uint64_t id = ph->StartPack("LuaPacket");

			ph->WriteString(id, identifier.c_str());

			std::ostringstream ss;
			ss << id;
			LuaEmbedder::PushString(L, ss.str().c_str());

			return 1;
		}

		int Update(lua_State* L)
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			Network::ClientNetwork* client = NetworkInstance::GetClient();

			server->Update(LuaEmbedder::PullFloat(L, 1));
			client->Update(LuaEmbedder::PullFloat(L, 1));

			return 0;
		}

		int ResetNetworkMaps(lua_State* L)
		{
			lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;

			if (parent == g_clientLuaState)
				NetworkInstance::GetClientNetworkHelper()->ResetNetworkMaps();
			else if (parent == g_serverLuaState)
				NetworkInstance::GetServerNetworkHelper()->ResetNetworkMaps();
			return 0;
		}

		int WriteFloat(lua_State* L)
		{
			Network::PacketHandler *ph = GetPacketHandler(L);

			if (!ph)
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			ph->WriteFloat(id, LuaEmbedder::PullFloat(L, 2));

			return 0;
		}
		int WriteString(lua_State* L)
		{
			Network::PacketHandler *ph = GetPacketHandler(L);

			if (!ph)
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			ph->WriteString(id, LuaEmbedder::PullString(L, 2).c_str());

			return 0;
		}
		int WriteBool(lua_State* L)
		{
			Network::PacketHandler *ph = GetPacketHandler(L);

			if (!ph)
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			ph->WriteByte(id, LuaEmbedder::PullBool(L, 2));

			return 0;
		}
		int WriteInt(lua_State* L)
		{
			Network::PacketHandler *ph = GetPacketHandler(L);

			if (!ph)
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			ph->WriteInt(id, LuaEmbedder::PullInt(L, 2));

			return 0;
		}

		int ReadFloat(lua_State* L)
		{
			Network::PacketHandler *ph = GetPacketHandler(L);

			if (!ph)
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			float value = ph->ReadFloat(id);
			LuaEmbedder::PushFloat(L, value);

			return 1;
		}
		int ReadString(lua_State* L)
		{
			Network::PacketHandler *ph = GetPacketHandler(L);

			if (!ph)
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			std::string value = ph->ReadString(id);
			LuaEmbedder::PushString(L, value.c_str());

			return 1;
		}
		int ReadBool(lua_State* L)
		{
			Network::PacketHandler *ph = GetPacketHandler(L);

			if (!ph)
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			bool value = (bool)ph->ReadByte(id);
			LuaEmbedder::PushBool(L, value);

			return 1;
		}
		int ReadInt(lua_State* L)
		{
			Network::PacketHandler *ph = GetPacketHandler(L);

			if (!ph)
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			int value = ph->ReadInt(id);
			LuaEmbedder::PushInt(L, value);

			return 1;
		}

		//Client
		int SendToServer(lua_State* L)
		{
			Network::ClientNetwork* client = NetworkInstance::GetClient();
			if (client->IsConnected())
			{
				std::string sId = LuaEmbedder::PullString(L, 1);
				char* end;
				uint64_t id = strtoull(sId.c_str(), &end, 10);

				Network::Packet* p = client->GetPacketHandler()->EndPack(id);
				client->Send(p);
			}
			return 0;
		}

		int Connect(lua_State* L)
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			Network::ClientNetwork* client = NetworkInstance::GetClient();
			if (server->IsRunning())
				server->Stop();

			if (client->IsConnected())
				client->Disconnect();

			std::string ip = client->GetRemoteAddress();
			unsigned int port = client->GetOutgoingPort();
			std::string pw = client->GetServerPassword();

			if (LuaEmbedder::IsString(L, 1) && LuaEmbedder::IsInt(L, 2) && LuaEmbedder::IsString(L, 3))
			{
				ip = LuaEmbedder::PullString(L, 1);
				port = LuaEmbedder::PullInt(L, 2);
				pw = LuaEmbedder::PullString(L, 3);
			}
			else if (LuaEmbedder::IsString(L, 1) && LuaEmbedder::IsInt(L, 2))
			{
				ip = LuaEmbedder::PullString(L, 1);
				port = LuaEmbedder::PullInt(L, 2);
			}
			else if (LuaEmbedder::IsString(L, 1))
			{
				ip = LuaEmbedder::PullString(L, 1);
			}

			bool connected = client->Connect(ip.c_str(), pw.c_str(), port, 0);
			LuaEmbedder::AddBool(L, "Client", connected);
			LuaEmbedder::PushBool(L, connected);

			return 1;
		}
		int Disconnect(lua_State* L)
		{
			Network::ClientNetwork* client = NetworkInstance::GetClient();
			if (client->IsConnected())
			{
				NetworkInstance::GetClient()->Disconnect();
				LuaEmbedder::AddBool(L, "Client", false);
			}
			return 0;
		}
		int IsConnected(lua_State* L)
		{
			LuaEmbedder::PushBool(L, NetworkInstance::GetClient()->IsConnected());
			return 1;
		}
		int Name(lua_State* L)
		{
			std::string name = LuaEmbedder::PullString(L, 1);

			if (name.length() > 0)
			{
				uint64_t id = NetworkInstance::GetClient()->GetPacketHandler()->StartPack("LuaPacket");
				NetworkInstance::GetClient()->GetPacketHandler()->WriteString(id, name.c_str());
				Network::Packet* packet = NetworkInstance::GetClient()->GetPacketHandler()->EndPack(id);
				NetworkInstance::GetClient()->Send(packet);
			}
			LuaEmbedder::PushString(L, name);
			return 1;
		}
		int ToServerID(lua_State* L)
		{
			unsigned int idH = LuaEmbedder::PullInt(L, 1);
			unsigned int idN = NetworkInstance::GetClientNetworkHelper()->HostToNet(idH);
			LuaEmbedder::PushInt(L, idN);
			return 1;
		}
		int ToClientID(lua_State* L)
		{
			unsigned int idN = LuaEmbedder::PullInt(L, 1);
			unsigned int idH = NetworkInstance::GetClientNetworkHelper()->NetToHost(idN);
			LuaEmbedder::PushInt(L, idH);
			return 1;
		}

		//Server
		int Send(lua_State* L)
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (server->IsRunning())
			{
				std::string sId = LuaEmbedder::PullString(L, 1);

				char* end;
				uint64_t id = strtoull(sId.c_str(), &end, 10);

				Network::Packet* p = server->GetPacketHandler()->EndPack(id);

				Network::NetConnection nc;
				if (LuaEmbedder::IsString(L, 2) && LuaEmbedder::IsInt(L, 3))
				{
					nc = Network::NetConnection(LuaEmbedder::PullString(L, 2).c_str(), LuaEmbedder::PullInt(L, 3));
				}
				else if (LuaEmbedder::IsString(L, 2))
				{
					// Get Netconnection based on username
				}

				server->Send(p, nc);
			}

			return 0;
		}
		int Broadcast(lua_State* L)
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (server->IsRunning())
			{
				std::string sId = LuaEmbedder::PullString(L, 1);

				char* end;
				uint64_t id = strtoull(sId.c_str(), &end, 10);

				Network::Packet* p = server->GetPacketHandler()->EndPack(id);

				std::vector<Network::NetConnection> NCs = ClientManager::GetConnectedClients();

				bool exclude = false;
				Network::NetConnection nc;
				if (LuaEmbedder::IsString(L, 2) && LuaEmbedder::IsInt(L, 3))
				{
					nc = Network::NetConnection(LuaEmbedder::PullString(L, 2).c_str(), LuaEmbedder::PullInt(L, 3));
				}
				else if (LuaEmbedder::IsString(L, 2))
				{
					// Get Netconnection based on username
				}

				if (exclude)
				{
					for (int i = 0; i < NCs.size(); ++i)
					{
						NCs.erase(std::remove(NCs.begin(), NCs.end(), nc), NCs.end());
					}
				}

				server->Send(p, NCs);
			}
			return 0;
		}
		int Kick(lua_State* L)
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (server->IsRunning())
			{
				std::string ip = LuaEmbedder::PullString(L, 1);
				unsigned int port = LuaEmbedder::PullInt(L, 2);
				std::string reason = "";

				if (LuaEmbedder::IsString(L, 3))
					reason = LuaEmbedder::PullString(L, 3);

				Network::NetConnection nc(ip.c_str(), port);
				//NetworkInstance::GetServer()->Kick(nc, reason.c_str());
			}
			return 0;
		}

		int ConnectedClients(lua_State* L)
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (server->IsRunning())
			{
				std::vector<Network::NetConnection> nc = ClientManager::GetConnectedClients();// server->GetConnectedClients();

				for (int i = 0; i < nc.size(); ++i)
				{
					LuaEmbedder::PushString(L, nc[i].GetIpAddress());
					LuaEmbedder::PushInt(L, nc[i].GetPort());
				}
				return nc.size() * 2;
			}
			return 0;
		}

		int GetPlayerName(lua_State* L)
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (server->IsRunning())
			{
				Network::NetConnection nc(LuaEmbedder::PullString(L, 1).c_str(), LuaEmbedder::PullInt(L, 2));
				LuaEmbedder::PushString(L, ClientManager::GetPlayerName(nc));

			}
			else
				LuaEmbedder::PushString(L, "");
			return 1;
		}

		int MaxConnections(lua_State* L)
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			LuaEmbedder::PushInt(L, server->GetMaxConnections());
			return 1;
		}

		int Host(lua_State* L)
		{
			if (NetworkInstance::GetClient()->IsConnected())
				NetworkInstance::GetClient()->Disconnect();

			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (server->IsRunning())
				server->Stop();


			std::string pw = server->GetServerPassword();
			unsigned int port = server->GetIncomingPort();
			unsigned int connections = server->GetMaxConnections();

			if (LuaEmbedder::IsString(L, 1) && LuaEmbedder::IsInt(L, 2) && LuaEmbedder::IsString(L, 3))
			{
				port = LuaEmbedder::PullInt(L, 1);
				pw = LuaEmbedder::PullString(L, 2);
				connections = LuaEmbedder::PullInt(L, 3);
			}
			else if (LuaEmbedder::IsString(L, 1) && LuaEmbedder::IsInt(L, 2))
			{
				port = LuaEmbedder::PullInt(L, 1);
				pw = LuaEmbedder::PullString(L, 2);
			}
			else if (LuaEmbedder::IsString(L, 1))
			{
				port = LuaEmbedder::PullInt(L, 1);
			}

			bool hosting = server->Start(port, pw.c_str(), connections);

			LuaEmbedder::AddBool(L, "Server", hosting);
			LuaEmbedder::PushBool(L, hosting);

			return 1;
		}
		int Stop(lua_State* L)
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (server->IsRunning())
			{
				server->Stop();
				LuaEmbedder::AddBool(L, "Server", false);
			}

			return 0;
		}
		int IsRunning(lua_State* L)
		{
			LuaEmbedder::PushBool(L, NetworkInstance::GetServer()->IsRunning());
			return 1;
		}

		int SendEntity(lua_State* L)
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (!server->IsRunning())
				return 0;

			unsigned int id = LuaEmbedder::PullInt(L, 1);
			std::string ip = LuaEmbedder::PullString(L, 2);
			unsigned int port = LuaEmbedder::PullInt(L, 3);

			Network::Packet* p = NetworkInstance::GetServerNetworkHelper()->WriteEntityAll(server->GetPacketHandler(), id);
			Network::NetConnection nc(ip.c_str(), port);
			server->Send(p, nc);

			return 0;
		}
		int SendEntityKill(lua_State* L)
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (!server->IsRunning())
				return 0;

			unsigned int id = LuaEmbedder::PullInt(L, 1);
			std::string ip = LuaEmbedder::PullString(L, 2);
			unsigned int port = LuaEmbedder::PullInt(L, 3);

			Network::Packet* p = NetworkInstance::GetServerNetworkHelper()->WriteEntityKill(server->GetPacketHandler(), id);
			Network::NetConnection nc(ip.c_str(), port);
			server->Send(p, nc);

			return 0;
		}

		int BroadcastEntity(lua_State* L)
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (!server->IsRunning())
				return 0;

			unsigned int id = LuaEmbedder::PullInt(L, 1);

			Network::Packet* p = NetworkInstance::GetServerNetworkHelper()->WriteEntityAll(server->GetPacketHandler(), id);
			std::vector<Network::NetConnection> NCs = ClientManager::GetConnectedClients();

			bool exclude = false;
			Network::NetConnection nc;
			if (LuaEmbedder::IsString(L, 2) && LuaEmbedder::IsInt(L, 3))
			{
				nc = Network::NetConnection(LuaEmbedder::PullString(L, 2).c_str(), LuaEmbedder::PullInt(L, 3));
			}
			else if (LuaEmbedder::IsString(L, 2))
			{
				// Get Netconnection based on username
			}

			if (exclude)
			{
				for (int i = 0; i < NCs.size(); ++i)
				{
					NCs.erase(std::remove(NCs.begin(), NCs.end(), nc), NCs.end());
				}
			}

			server->Send(p, NCs);

			return 0;
		}
		int BroadcastEntityKill(lua_State* L)
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (!server->IsRunning())
				return 0;

			unsigned int id = LuaEmbedder::PullInt(L, 1);

			Network::Packet* p = NetworkInstance::GetServerNetworkHelper()->WriteEntityKill(server->GetPacketHandler(), id);
			std::vector<Network::NetConnection> NCs = ClientManager::GetConnectedClients();


			bool exclude = false;
			Network::NetConnection nc;
			if (LuaEmbedder::IsString(L, 2) && LuaEmbedder::IsInt(L, 3))
			{
				nc = Network::NetConnection(LuaEmbedder::PullString(L, 2).c_str(), LuaEmbedder::PullInt(L, 3));
			}
			else if (LuaEmbedder::IsString(L, 2))
			{
				// Get Netconnection based on username
			}

			if (exclude)
			{
				for (int i = 0; i < NCs.size(); ++i)
				{
					NCs.erase(std::remove(NCs.begin(), NCs.end(), nc), NCs.end());
				}
			}

			server->Send(p, NCs);

			return 0;
		}
	}

	namespace LuaNetworkEvents
	{
		std::vector<Network::NetEvent> g_onConnectedToServer;
		std::vector<Network::NetEvent> g_onDisconnectedFromServer;
		std::vector<Network::NetEvent> g_onTimedOutFromServer;
		std::vector<Network::NetEvent> g_onFailedToConnect;
		std::vector<Network::NetEvent> g_onPasswordInvalid;
		std::vector<Network::NetEvent> g_onKickedFromServer;
		std::vector<Network::NetEvent> g_onBannedFromServer;
		std::vector<Network::NetEvent> g_onServerFull;

		std::vector<Network::NetEvent> g_onRemotePlayerConnected;
		std::vector<Network::NetEvent> g_onRemotePlayerDisconnected;
		std::vector<Network::NetEvent> g_onRemotePlayerTimedOut;
		std::vector<Network::NetEvent> g_onRemotePlayerKicked;
		std::vector<Network::NetEvent> g_onRemotePlayerBanned;


		std::vector<Network::NetEvent> g_onPlayerConnected;
		std::vector<Network::NetEvent> g_onPlayerDisconnected;
		std::vector<Network::NetEvent> g_onPlayerTimedOut;

		void Clear()
		{
			g_onConnectedToServer.clear();
			g_onDisconnectedFromServer.clear();
			g_onTimedOutFromServer.clear();
			g_onFailedToConnect.clear();
			g_onPasswordInvalid.clear();
			g_onKickedFromServer.clear();
			g_onBannedFromServer.clear();
			g_onServerFull.clear();

			g_onRemotePlayerConnected.clear();
			g_onRemotePlayerDisconnected.clear();
			g_onRemotePlayerTimedOut.clear();
			g_onRemotePlayerKicked.clear();
			g_onRemotePlayerBanned.clear();


			g_onPlayerConnected.clear();
			g_onPlayerDisconnected.clear();
			g_onPlayerTimedOut.clear();
		}
	}

	namespace LuaClientNetwork
	{
		int StartPack(lua_State* L);
		int Send(lua_State* L);

		int Connect(lua_State* L);
		int Disconnect(lua_State* L);
		int IsConnected(lua_State* L);
		int Update(lua_State* L);

		//int WriteByte();
		int WriteFloat(lua_State* L);
		//int WriteShort();
		int WriteString(lua_State* L);
		int WriteBool(lua_State* L);
		int WriteInt(lua_State* L);

		//int ReadByte();
		int ReadFloat(lua_State* L);
		//int ReadShort();
		int ReadString(lua_State* L);
		int ReadBool(lua_State* L);
		int ReadInt(lua_State* L);

		int ResetNetworkMaps(lua_State* L);

		void Embed(lua_State* L)
		{
			LuaEmbedder::AddFunction(L, "StartPack", &StartPack, "Client");
			LuaEmbedder::AddFunction(L, "Send", &Send, "Client");

			LuaEmbedder::AddFunction(L, "Connect", &Connect, "Client");
			LuaEmbedder::AddFunction(L, "Disconnect", &Disconnect, "Client");
			LuaEmbedder::AddFunction(L, "IsConnected", &IsConnected, "Client");
			LuaEmbedder::AddFunction(L, "Update", &Update, "Client");

			//LuaEmbedder::AddFunction("WriteByte", &WriteByte, "Client");
			LuaEmbedder::AddFunction(L, "WriteFloat", &WriteFloat, "Client");
			//LuaEmbedder::AddFunction("WriteShort", &WriteShort, "Client");
			LuaEmbedder::AddFunction(L, "WriteString", &WriteString, "Client");
			LuaEmbedder::AddFunction(L, "WriteBool", &WriteBool, "Client");
			LuaEmbedder::AddFunction(L, "WriteInt", &WriteInt, "Client");

			//LuaEmbedder::AddFunction("ReadByte", &ReadByte, "Client");
			LuaEmbedder::AddFunction(L, "ReadFloat", &ReadFloat, "Client");
			//LuaEmbedder::AddFunction("ReadShort", &ReadShort, "Client");
			LuaEmbedder::AddFunction(L, "ReadString", &ReadString, "Client");
			LuaEmbedder::AddFunction(L, "ReadBool", &ReadBool, "Client");
			LuaEmbedder::AddFunction(L, "ReadInt", &ReadInt, "Client");

			LuaEmbedder::AddFunction(L, "ResetNetworkMaps", &ResetNetworkMaps, "Client");
		}

		int StartPack(lua_State* L)
		{
			if (!NetworkInstance::GetClient()->IsConnected())
				return 0;

			std::string identifier = LuaEmbedder::PullString(L, 1);
			uint64_t id = NetworkInstance::GetClient()->GetPacketHandler()->StartPack("LuaPacket");

			NetworkInstance::GetClient()->GetPacketHandler()->WriteString(id, identifier.c_str());

			std::ostringstream ss;
			ss << id;
			LuaEmbedder::PushString(L, ss.str().c_str());

			return 1;
		}

		int Send(lua_State* L)
		{
			if (!NetworkInstance::GetClient()->IsConnected())
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			Network::Packet* p = NetworkInstance::GetClient()->GetPacketHandler()->EndPack(id);
			NetworkInstance::GetClient()->Send(p);

			return 0;
		}

		int Connect(lua_State* L)
		{
			if (NetworkInstance::GetClient()->IsConnected())
				NetworkInstance::GetClient()->Disconnect();

			if (strcmp(NetworkInstance::GetClient()->GetRemoteAddress(), "127.0.0.1") != 0)
			{
				if (NetworkInstance::GetServer()->IsRunning())
					NetworkInstance::GetServer()->Stop();
			}

			std::string ip = NetworkInstance::GetClient()->GetRemoteAddress();
			unsigned int port = NetworkInstance::GetClient()->GetOutgoingPort();
			std::string pw = NetworkInstance::GetClient()->GetServerPassword();

			if (LuaEmbedder::IsString(L, 1) && LuaEmbedder::IsInt(L, 2) && LuaEmbedder::IsString(L, 3))
			{
				ip = LuaEmbedder::PullString(L, 1);
				port = LuaEmbedder::PullInt(L, 2);
				pw = LuaEmbedder::PullString(L, 3);
			}
			else if (LuaEmbedder::IsString(L, 1) && LuaEmbedder::IsInt(L, 2))
			{
				ip = LuaEmbedder::PullString(L, 1);
				port = LuaEmbedder::PullInt(L, 2);
			}
			else if (LuaEmbedder::IsString(L, 1))
			{
				ip = LuaEmbedder::PullString(L, 1);
			}

			NetworkInstance::GetClient()->Connect(ip.c_str(), pw.c_str(), port, 0);

			return 0;
		}
		int Disconnect(lua_State* L)
		{
			NetworkInstance::GetClient()->Disconnect();

			return 0;
		}
		int IsConnected(lua_State* L)
		{
			LuaEmbedder::PushBool(L, NetworkInstance::GetClient()->IsConnected());
			return 1;
		}
		int Update(lua_State* L)
		{
			NetworkInstance::GetClient()->Update(LuaEmbedder::PullFloat(L, 1));

			return 0;
		}

		int WriteFloat(lua_State* L)
		{
			if (!NetworkInstance::GetClient()->IsConnected())
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			NetworkInstance::GetClient()->GetPacketHandler()->WriteFloat(id, LuaEmbedder::PullFloat(L, 2));

			return 0;
		}
		int WriteString(lua_State* L)
		{
			if (!NetworkInstance::GetClient()->IsConnected())
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			NetworkInstance::GetClient()->GetPacketHandler()->WriteString(id, LuaEmbedder::PullString(L, 2).c_str());

			return 0;
		}
		int WriteBool(lua_State* L)
		{
			if (!NetworkInstance::GetClient()->IsConnected())
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			NetworkInstance::GetClient()->GetPacketHandler()->WriteByte(id, LuaEmbedder::PullBool(L, 2));

			return 0;
		}
		int WriteInt(lua_State* L)
		{
			if (!NetworkInstance::GetClient()->IsConnected())
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			NetworkInstance::GetClient()->GetPacketHandler()->WriteInt(id, LuaEmbedder::PullInt(L, 2));

			return 0;
		}

		int ReadFloat(lua_State* L)
		{
			if (!NetworkInstance::GetClient()->IsConnected())
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			float value = NetworkInstance::GetClient()->GetPacketHandler()->ReadFloat(id);
			LuaEmbedder::PushFloat(L, value);

			return 1;
		}
		int ReadString(lua_State* L)
		{
			if (!NetworkInstance::GetClient()->IsConnected())
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			std::string value = NetworkInstance::GetClient()->GetPacketHandler()->ReadString(id);
			LuaEmbedder::PushString(L, value.c_str());

			return 1;
		}
		int ReadBool(lua_State* L)
		{
			if (!NetworkInstance::GetClient()->IsConnected())
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			bool value = (bool)NetworkInstance::GetClient()->GetPacketHandler()->ReadByte(id);
			LuaEmbedder::PushBool(L, value);

			return 1;
		}
		int ReadInt(lua_State* L)
		{
			if (!NetworkInstance::GetClient()->IsConnected())
				return 0;


			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			int value = NetworkInstance::GetClient()->GetPacketHandler()->ReadInt(id);
			LuaEmbedder::PushInt(L, value);

			return 1;
		}

		int ResetNetworkMaps(lua_State* L)
		{
			NetworkInstance::GetClientNetworkHelper()->ResetNetworkMaps();
			return 0;
		}
	}

	namespace LuaServerNetwork
	{
		int StartPack(lua_State* L);
		int Send(lua_State* L);
		int Broadcast(lua_State* L);

		int Start(lua_State* L);
		int Stop(lua_State* L);
		int IsRunning(lua_State* L);
		int Update(lua_State* L);
		int Kick(lua_State* L);

		//int WriteByte();
		int WriteFloat(lua_State* L);
		//int WriteShort();
		int WriteString(lua_State* L);
		int WriteBool(lua_State* L);
		int WriteInt(lua_State* L);

		//int ReadByte();
		int ReadFloat(lua_State* L);
		//int ReadShort();
		int ReadString(lua_State* L);
		int ReadBool(lua_State* L);
		int ReadInt(lua_State* L);

		int SendEntity(lua_State* L);
		int SendEntityKill(lua_State* L);
		int BroadcastEntity(lua_State* L);
		int BroadcastEntityKill(lua_State* L);

		void Embed(lua_State* L)
		{
			LuaEmbedder::AddFunction(L, "StartPack", &StartPack, "Server");
			LuaEmbedder::AddFunction(L, "Send", &Send, "Server");
			LuaEmbedder::AddFunction(L, "Broadcast", &Broadcast, "Server");

			LuaEmbedder::AddFunction(L, "Start", &Start, "Server");
			LuaEmbedder::AddFunction(L, "Stop", &Stop, "Server");
			LuaEmbedder::AddFunction(L, "IsRunning", &IsRunning, "Server");
			LuaEmbedder::AddFunction(L, "Update", &Update, "Server");
			LuaEmbedder::AddFunction(L, "Kick", &Kick, "Server");

			//LuaEmbedder::AddFunction("WriteByte", &WriteByte, "Server");
			LuaEmbedder::AddFunction(L, "WriteFloat", &WriteFloat, "Server");
			//LuaEmbedder::AddFunction("WriteShort", &WriteShort, "Server");
			LuaEmbedder::AddFunction(L, "WriteString", &WriteString, "Server");
			LuaEmbedder::AddFunction(L, "WriteBool", &WriteBool, "Server");
			LuaEmbedder::AddFunction(L, "WriteInt", &WriteInt, "Server");


			//LuaEmbedder::AddFunction("ReadByte", &ReadByte, "Server");
			LuaEmbedder::AddFunction(L, "ReadFloat", &ReadFloat, "Server");
			//LuaEmbedder::AddFunction("ReadShort", &ReadShort, "Server");
			LuaEmbedder::AddFunction(L, "ReadString", &ReadString, "Server");
			LuaEmbedder::AddFunction(L, "ReadBool", &ReadBool, "Server");
			LuaEmbedder::AddFunction(L, "ReadInt", &ReadInt, "Server");


			LuaEmbedder::AddFunction(L, "SendEntity", &SendEntity, "Server");
			LuaEmbedder::AddFunction(L, "SendEntityKill", &SendEntityKill, "Server");
			LuaEmbedder::AddFunction(L, "BroadcastEntity", &BroadcastEntity, "Server");
			LuaEmbedder::AddFunction(L, "BroadcastEntityKill", &BroadcastEntityKill, "Server");
		}

		int StartPack(lua_State* L)
		{
			if (!NetworkInstance::GetServer()->IsRunning())
				return 0;

			std::string identifier = LuaEmbedder::PullString(L, 1);
			uint64_t id = NetworkInstance::GetServer()->GetPacketHandler()->StartPack("LuaPacket");

			NetworkInstance::GetServer()->GetPacketHandler()->WriteString(id, identifier.c_str());

			std::ostringstream ss;
			ss << id;
			LuaEmbedder::PushString(L, ss.str().c_str());

			return 1;
		}
		int Send(lua_State* L)
		{
			if (!NetworkInstance::GetServer()->IsRunning())
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);

			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			Network::Packet* p = NetworkInstance::GetServer()->GetPacketHandler()->EndPack(id);

			Network::NetConnection nc;
			if (LuaEmbedder::IsString(L, 2) && LuaEmbedder::IsInt(L, 3))
			{
				nc = Network::NetConnection(LuaEmbedder::PullString(L, 2).c_str(), LuaEmbedder::PullInt(L, 3));
			}
			else if (LuaEmbedder::IsString(L, 2))
			{
				// Get Netconnection based on username
			}

			NetworkInstance::GetServer()->Send(p, nc);

			return 0;
		}
		int Broadcast(lua_State* L)
		{
			if (!NetworkInstance::GetServer()->IsRunning())
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);

			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			Network::Packet* p = NetworkInstance::GetServer()->GetPacketHandler()->EndPack(id);

			Network::NetConnection nc;
			if (LuaEmbedder::IsString(L, 2) && LuaEmbedder::IsInt(L, 3))
			{
				nc = Network::NetConnection(LuaEmbedder::PullString(L, 2).c_str(), LuaEmbedder::PullInt(L, 3));
			}
			else if (LuaEmbedder::IsString(L, 2))
			{
				// Get Netconnection based on username
			}


			NetworkInstance::GetServer()->Broadcast(p, nc);

			return 0;
		}

		int Start(lua_State* L)
		{
			if (NetworkInstance::GetClient()->IsConnected())
				NetworkInstance::GetClient()->Disconnect();

			if (NetworkInstance::GetServer()->IsRunning())
				NetworkInstance::GetServer()->Stop();


			std::string pw = NetworkInstance::GetServer()->GetServerPassword();
			unsigned int port = NetworkInstance::GetServer()->GetIncomingPort();
			unsigned int connections = NetworkInstance::GetServer()->GetMaxConnections();

			if (LuaEmbedder::IsString(L, 1) && LuaEmbedder::IsInt(L, 2) && LuaEmbedder::IsString(L, 3))
			{
				port = LuaEmbedder::PullInt(L, 1);
				pw = LuaEmbedder::PullString(L, 2);
				connections = LuaEmbedder::PullInt(L, 3);
			}
			else if (LuaEmbedder::IsString(L, 1) && LuaEmbedder::IsInt(L, 2))
			{
				port = LuaEmbedder::PullInt(L, 1);
				pw = LuaEmbedder::PullString(L, 2);
			}
			else if (LuaEmbedder::IsString(L, 1))
			{
				port = LuaEmbedder::PullInt(L, 1);
			}

			NetworkInstance::GetServer()->Start(port, pw.c_str(), connections);


			return 0;
		}
		int Stop(lua_State* L)
		{
			if (NetworkInstance::GetClient()->IsConnected() && strcmp(NetworkInstance::GetClient()->GetRemoteAddress(), "127.0.0.1") == 0)
				NetworkInstance::GetClient()->Disconnect();

			if (NetworkInstance::GetServer()->IsRunning())
				NetworkInstance::GetServer()->Stop();

			return 0;
		}
		int IsRunning(lua_State* L)
		{
			LuaEmbedder::PushBool(L, NetworkInstance::GetServer()->IsRunning());

			return 1;
		}
		int Update(lua_State* L)
		{
			NetworkInstance::GetServer()->Update(LuaEmbedder::PullFloat(L, 1));

			return 0;
		}

		int Kick(lua_State* L)
		{
			std::string ip = LuaEmbedder::PullString(L, 1);
			unsigned int port = LuaEmbedder::PullInt(L, 2);
			std::string reason = "";

			if (LuaEmbedder::IsString(L, 3))
				reason = LuaEmbedder::PullString(L, 3);

			Network::NetConnection nc(ip.c_str(), port);
			//NetworkInstance::GetServer()->Kick(nc, reason.c_str());

			return 0;
		}

		int WriteFloat(lua_State* L)
		{
			if (!NetworkInstance::GetServer()->IsRunning())
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			NetworkInstance::GetServer()->GetPacketHandler()->WriteFloat(id, LuaEmbedder::PullFloat(L, 2));

			return 0;
		}
		int WriteString(lua_State* L)
		{
			if (!NetworkInstance::GetServer()->IsRunning())
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			NetworkInstance::GetServer()->GetPacketHandler()->WriteString(id, LuaEmbedder::PullString(L, 2).c_str());

			return 0;
		}
		int WriteBool(lua_State* L)
		{
			if (!NetworkInstance::GetServer()->IsRunning())
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			NetworkInstance::GetServer()->GetPacketHandler()->WriteByte(id, LuaEmbedder::PullBool(L, 2));

			return 0;
		}
		int WriteInt(lua_State* L)
		{
			if (!NetworkInstance::GetServer()->IsRunning())
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			NetworkInstance::GetServer()->GetPacketHandler()->WriteInt(id, LuaEmbedder::PullInt(L, 2));

			return 0;
		}

		int ReadFloat(lua_State* L)
		{
			if (!NetworkInstance::GetServer()->IsRunning())
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			float value = NetworkInstance::GetServer()->GetPacketHandler()->ReadFloat(id);
			LuaEmbedder::PushFloat(L, value);

			return 1;
		}
		int ReadString(lua_State* L)
		{
			if (!NetworkInstance::GetServer()->IsRunning())
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			std::string value = NetworkInstance::GetServer()->GetPacketHandler()->ReadString(id);
			LuaEmbedder::PushString(L, value.c_str());

			return 1;
		}
		int ReadBool(lua_State* L)
		{
			if (!NetworkInstance::GetServer()->IsRunning())
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			bool value = (bool)NetworkInstance::GetServer()->GetPacketHandler()->ReadByte(id);
			LuaEmbedder::PushBool(L, value);

			return 1;
		}
		int ReadInt(lua_State* L)
		{
			if (!NetworkInstance::GetServer()->IsRunning())
				return 0;

			std::string sId = LuaEmbedder::PullString(L, 1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			int value = NetworkInstance::GetServer()->GetPacketHandler()->ReadInt(id);
			LuaEmbedder::PushInt(L, value);

			return 1;
		}

		int SendEntity(lua_State* L)
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (!server->IsRunning())
				return 0;

			unsigned int id = LuaEmbedder::PullInt(L, 1);
			std::string ip = LuaEmbedder::PullString(L, 2);
			unsigned int port = LuaEmbedder::PullInt(L, 3);

			Network::Packet* p = NetworkInstance::GetServerNetworkHelper()->WriteEntityAll(server->GetPacketHandler(), id);
			Network::NetConnection nc(ip.c_str(), port);
			server->Send(p, nc);

			return 0;
		}
		int SendEntityKill(lua_State* L)
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (!server->IsRunning())
				return 0;

			unsigned int id = LuaEmbedder::PullInt(L, 1);
			std::string ip = LuaEmbedder::PullString(L, 2);
			unsigned int port = LuaEmbedder::PullInt(L, 3);

			Network::Packet* p = NetworkInstance::GetServerNetworkHelper()->WriteEntityKill(server->GetPacketHandler(), id);
			Network::NetConnection nc(ip.c_str(), port);
			server->Send(p, nc);

			return 0;
		}

		int BroadcastEntity(lua_State* L)
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (!server->IsRunning())
				return 0;

			unsigned int id = LuaEmbedder::PullInt(L, 1);

			Network::Packet* p = NetworkInstance::GetServerNetworkHelper()->WriteEntityAll(server->GetPacketHandler(), id);
			std::vector<Network::NetConnection> NCs = ClientManager::GetConnectedClients();
			server->Send(p, NCs);

			return 0;
		}
		int BroadcastEntityKill(lua_State* L)
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (!server->IsRunning())
				return 0;

			unsigned int id = LuaEmbedder::PullInt(L, 1);

			Network::Packet* p = NetworkInstance::GetServerNetworkHelper()->WriteEntityKill(server->GetPacketHandler(), id);
			std::vector<Network::NetConnection> NCs = ClientManager::GetConnectedClients();
			server->Send(p, NCs);

			return 0;
		}

	}
}