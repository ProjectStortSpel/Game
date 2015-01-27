#include "LuaNetwork.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "../../NetworkInstance.h"
#include "Game/Network/ClientDatabase.h"
#include <sstream>

namespace LuaBridge
{

	namespace LuaNetwork
	{

		//Shared
		int Receive();

		int StartPack();

		int Update();

		int ResetNetworkMaps();

		//int WriteByte();
		int WriteFloat();
		//int WriteShort();
		int WriteString();
		int WriteBool();
		int WriteInt();

		//int ReadByte();
		int ReadFloat();
		//int ReadShort();
		int ReadString();
		int ReadBool();
		int ReadInt();

		//Client
		int SendToServer();
		int Connect();
		int Disconnect();
		int IsConnected();	
		int ToClientID();
		int ToServerID();

		//Server
		int Send();
		int Broadcast();

		int Host();
		int Stop();
		int IsRunning();
		int Kick();
		int MaxConnections();
		int ConnectedClients();

		int SendEntity();
		int SendEntityKill();
		int BroadcastEntity();
		int BroadcastEntityKill();



		int GetServerList();

		void Embed()
		{
			//init global bools
			LuaEmbedder::AddBool("Client", false);
			LuaEmbedder::AddBool("Server", false);

			//Shared
			LuaEmbedder::AddFunction("Receive", &Receive, "Net");

			LuaEmbedder::AddFunction("StartPack", &StartPack, "Net");

			LuaEmbedder::AddFunction("Update", &Update, "Net");

			LuaEmbedder::AddFunction("ResetNetworkMaps", &ResetNetworkMaps, "Net");

			

			//LuaEmbedder::AddFunction("WriteByte", &WriteByte, "Net");
			LuaEmbedder::AddFunction("WriteFloat", &WriteFloat, "Net");
			//LuaEmbedder::AddFunction("WriteShort", &WriteShort, "Net");
			LuaEmbedder::AddFunction("WriteString", &WriteString, "Net");
			LuaEmbedder::AddFunction("WriteBool", &WriteBool, "Net");
			LuaEmbedder::AddFunction("WriteInt", &WriteInt, "Net");

			//LuaEmbedder::AddFunction("ReadByte", &ReadByte, "Net");
			LuaEmbedder::AddFunction("ReadFloat", &ReadFloat, "Net");
			//LuaEmbedder::AddFunction("ReadShort", &ReadShort, "Net");
			LuaEmbedder::AddFunction("ReadString", &ReadString, "Net");
			LuaEmbedder::AddFunction("ReadBool", &ReadBool, "Net");
			LuaEmbedder::AddFunction("ReadInt", &ReadInt, "Net");


			//Client
			LuaEmbedder::AddFunction("SendToServer", &SendToServer, "Net");
			LuaEmbedder::AddFunction("Connect", &Connect, "Net");
			LuaEmbedder::AddFunction("Disconnect", &Disconnect, "Net");
			LuaEmbedder::AddFunction("IsConnected", &IsConnected, "Net");

			LuaEmbedder::AddFunction("ToServerID", &ToServerID, "Net");
			LuaEmbedder::AddFunction("ToClientID", &ToClientID, "Net");

			//Server
			LuaEmbedder::AddFunction("Send", &Send, "Net");
			LuaEmbedder::AddFunction("Broadcast", &Broadcast, "Net");

			LuaEmbedder::AddFunction("Host", &Host, "Net");
			LuaEmbedder::AddFunction("Stop", &Stop, "Net");
			LuaEmbedder::AddFunction("IsRunning", &IsRunning, "Net");
			LuaEmbedder::AddFunction("Kick", &Kick, "Net");
			LuaEmbedder::AddFunction("MaxConnections", &MaxConnections, "Net");
			LuaEmbedder::AddFunction("ConnectedClients", &ConnectedClients, "Net");

			LuaEmbedder::AddFunction("SendEntity", &SendEntity, "Net");
			LuaEmbedder::AddFunction("SendEntityKill", &SendEntityKill, "Net");
			LuaEmbedder::AddFunction("BroadcastEntity", &BroadcastEntity, "Net");
			LuaEmbedder::AddFunction("BroadcastEntityKill", &BroadcastEntityKill, "Net");


			LuaEmbedder::AddFunction("GetServerList", &GetServerList, "Net");
		}

		//Shared

		Network::PacketHandler* GetPacketHandler()
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			Network::ClientNetwork* client = NetworkInstance::GetClient();

			Network::PacketHandler* ph = NULL;

			if (server->IsRunning())
			{
				ph = server->GetPacketHandler();
			}
			else if (client->IsConnected())
			{
				ph = client->GetPacketHandler();
			}
			return ph;
		}

		int Receive()
		{
			std::string name = LuaEmbedder::PullString(1);
			LuaEmbedder::SaveFunction(2, name);
			return 0;
		}

		int StartPack()
		{
			Network::PacketHandler *ph = GetPacketHandler();

			if (!ph)
				return 0;
			
			std::string identifier = LuaEmbedder::PullString(1);
			uint64_t id = ph->StartPack("LuaPacket");

			ph->WriteString(id, identifier.c_str());

			std::ostringstream ss;
			ss << id;
			LuaEmbedder::PushString(ss.str().c_str());

			return 1;
		}

		int Update()
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			Network::ClientNetwork* client = NetworkInstance::GetClient();

			server->Update(LuaEmbedder::PullFloat(1));
			client->Update(LuaEmbedder::PullFloat(1));

			return 0;
		}

		int ResetNetworkMaps()
		{
			NetworkInstance::GetNetworkHelper()->ResetNetworkMaps();
			return 0;
		}

		int WriteFloat()
		{
			Network::PacketHandler *ph = GetPacketHandler();

			if (!ph)
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			ph->WriteFloat(id, LuaEmbedder::PullFloat(2));

			return 0;
		}
		int WriteString()
		{
			Network::PacketHandler *ph = GetPacketHandler();

			if (!ph)
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			ph->WriteString(id, LuaEmbedder::PullString(2).c_str());

			return 0;
		}
		int WriteBool()
		{
			Network::PacketHandler *ph = GetPacketHandler();

			if (!ph)
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			ph->WriteByte(id, LuaEmbedder::PullBool(2));

			return 0;
		}
		int WriteInt()
		{
			Network::PacketHandler *ph = GetPacketHandler();

			if (!ph)
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			ph->WriteInt(id, LuaEmbedder::PullInt(2));

			return 0;
		}

		int ReadFloat()
		{
			Network::PacketHandler *ph = GetPacketHandler();

			if (!ph)
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			float value = ph->ReadFloat(id);
			LuaEmbedder::PushFloat(value);

			return 1;
		}
		int ReadString()
		{
			Network::PacketHandler *ph = GetPacketHandler();

			if (!ph)
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			std::string value = ph->ReadString(id);
			LuaEmbedder::PushString(value.c_str());

			return 1;
		}
		int ReadBool()
		{
			Network::PacketHandler *ph = GetPacketHandler();

			if (!ph)
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			bool value = (bool)ph->ReadByte(id);
			LuaEmbedder::PushBool(value);

			return 1;
		}
		int ReadInt()
		{
			Network::PacketHandler *ph = GetPacketHandler();

			if (!ph)
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			int value = ph->ReadInt(id);
			LuaEmbedder::PushInt(value);

			return 1;
		}

		//Client
		int SendToServer()
		{
			Network::ClientNetwork* client = NetworkInstance::GetClient();
			if (client->IsConnected())
			{
				std::string sId = LuaEmbedder::PullString(1);
				char* end;
				uint64_t id = strtoull(sId.c_str(), &end, 10);

				Network::Packet* p = client->GetPacketHandler()->EndPack(id);
				client->Send(p);
			}
			return 0;
		}

		int Connect()
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

			if (LuaEmbedder::IsString(1) && LuaEmbedder::IsInt(2) && LuaEmbedder::IsString(3))
			{
				ip = LuaEmbedder::PullString(1);
				port = LuaEmbedder::PullInt(2);
				pw = LuaEmbedder::PullString(3);
			}
			else if (LuaEmbedder::IsString(1) && LuaEmbedder::IsInt(2))
			{
				ip = LuaEmbedder::PullString(1);
				port = LuaEmbedder::PullInt(2);
			}
			else if (LuaEmbedder::IsString(1))
			{
				ip = LuaEmbedder::PullString(1);
			}

			bool connected = client->Connect(ip.c_str(), pw.c_str(), port, 0);
			LuaEmbedder::AddBool("Client", connected);
			LuaEmbedder::PushBool(connected);

			return 1;
		}
		int Disconnect()
		{
			Network::ClientNetwork* client = NetworkInstance::GetClient();
			if (client->IsConnected())
			{
				NetworkInstance::GetClient()->Disconnect();
				LuaEmbedder::AddBool("Client", false);
			}
			return 0;
		}
		int IsConnected()
		{
			LuaEmbedder::PushBool(NetworkInstance::GetClient()->IsConnected());
			return 1;
		}
		int ToServerID()
		{
			unsigned int idH = LuaEmbedder::PullInt(1);
			unsigned int idN = NetworkInstance::GetNetworkHelper()->HostToNet(idH);
			LuaEmbedder::PushInt(idN);
			return 1;
		}
		int ToClientID()
		{
			unsigned int idN = LuaEmbedder::PullInt(1);
			unsigned int idH = NetworkInstance::GetNetworkHelper()->NetToHost(idN);
			LuaEmbedder::PushInt(idH);
			return 1;
		}

		//Server
		int Send()
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (server->IsRunning())
			{
				std::string sId = LuaEmbedder::PullString(1);

				char* end;
				uint64_t id = strtoull(sId.c_str(), &end, 10);

				Network::Packet* p = server->GetPacketHandler()->EndPack(id);

				Network::NetConnection nc;
				if (LuaEmbedder::IsString(2) && LuaEmbedder::IsInt(3))
				{
					nc = Network::NetConnection(LuaEmbedder::PullString(2).c_str(), LuaEmbedder::PullInt(3));
				}
				else if (LuaEmbedder::IsString(2))
				{
					// Get Netconnection based on username
				}

				server->Send(p, nc);
			}

			return 0;
		}
		int Broadcast()
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (server->IsRunning())
			{
				std::string sId = LuaEmbedder::PullString(1);

				char* end;
				uint64_t id = strtoull(sId.c_str(), &end, 10);

				Network::Packet* p = server->GetPacketHandler()->EndPack(id);

				Network::NetConnection nc;
				if (LuaEmbedder::IsString(2) && LuaEmbedder::IsInt(3))
				{
					nc = Network::NetConnection(LuaEmbedder::PullString(2).c_str(), LuaEmbedder::PullInt(3));
				}
				else if (LuaEmbedder::IsString(2))
				{
					// Get Netconnection based on username
				}

				server->Broadcast(p, nc);
			}
			return 0;
		}
		int Kick()
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (server->IsRunning())
			{
				std::string ip = LuaEmbedder::PullString(1);
				unsigned int port = LuaEmbedder::PullInt(2);
				std::string reason = "";

				if (LuaEmbedder::IsString(3))
					reason = LuaEmbedder::PullString(3);

				Network::NetConnection nc(ip.c_str(), port);
				NetworkInstance::GetServer()->Kick(nc, reason.c_str());
			}
			return 0;
		}

		int ConnectedClients()
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (server->IsRunning())
			{
				std::vector<Network::NetConnection> nc = server->GetConnectedClients();
				
				for (int i = 0; i < nc.size(); ++i)
				{
					LuaEmbedder::PushString(nc[i].GetIpAddress());
					LuaEmbedder::PushInt(nc[i].GetPort());
				}
				return nc.size() * 2;
			}
			return 0;
		}		

		int MaxConnections()
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			LuaEmbedder::PushInt(server->GetMaxConnections());
			return 1;
		}

		int Host()
		{
			if (NetworkInstance::GetClient()->IsConnected())
				NetworkInstance::GetClient()->Disconnect();

			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (server->IsRunning())
				server->Stop();


			std::string pw = server->GetServerPassword();
			unsigned int port = server->GetIncomingPort();
			unsigned int connections = server->GetMaxConnections();

			if (LuaEmbedder::IsString(1) && LuaEmbedder::IsInt(2) && LuaEmbedder::IsString(3))
			{
				port = LuaEmbedder::PullInt(1);
				pw = LuaEmbedder::PullString(2);
				connections = LuaEmbedder::PullInt(3);
			}
			else if (LuaEmbedder::IsString(1) && LuaEmbedder::IsInt(2))
			{
				port = LuaEmbedder::PullInt(1);
				pw = LuaEmbedder::PullString(2);
			}
			else if (LuaEmbedder::IsString(1))
			{
				port = LuaEmbedder::PullInt(1);
			}

			bool hosting = server->Start(port, pw.c_str(), connections);

			LuaEmbedder::AddBool("Server", hosting);
			LuaEmbedder::PushBool(hosting);

			return 1;
		}
		int Stop()
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (server->IsRunning())
			{
				server->Stop();
				LuaEmbedder::AddBool("Server", false);
			}

			return 0;
		}
		int IsRunning()
		{
			LuaEmbedder::PushBool(NetworkInstance::GetServer()->IsRunning());
			return 1;
		}

		int SendEntity()
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (!server->IsRunning())
				return 0;

			unsigned int id = LuaEmbedder::PullInt(1);
			std::string ip = LuaEmbedder::PullString(2);
			unsigned int port = LuaEmbedder::PullInt(3);

			Network::Packet* p = NetworkInstance::GetNetworkHelper()->WriteEntityAll(server->GetPacketHandler(), id);
			Network::NetConnection nc(ip.c_str(), port);
			server->Send(p, nc);

			return 0;
		}
		int SendEntityKill()
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (!server->IsRunning())
				return 0;

			unsigned int id = LuaEmbedder::PullInt(1);
			std::string ip = LuaEmbedder::PullString(2);
			unsigned int port = LuaEmbedder::PullInt(3);

			Network::Packet* p = NetworkInstance::GetNetworkHelper()->WriteEntityKill(server->GetPacketHandler(), id);
			Network::NetConnection nc(ip.c_str(), port);
			server->Send(p, nc);

			return 0;
		}

		int BroadcastEntity()
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (!server->IsRunning())
				return 0;

			unsigned int id = LuaEmbedder::PullInt(1);

			Network::Packet* p = NetworkInstance::GetNetworkHelper()->WriteEntityAll(server->GetPacketHandler(), id);
			server->Broadcast(p);

			return 0;
		}
		int BroadcastEntityKill()
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (!server->IsRunning())
				return 0;

			unsigned int id = LuaEmbedder::PullInt(1);

			Network::Packet* p = NetworkInstance::GetNetworkHelper()->WriteEntityKill(server->GetPacketHandler(), id);
			server->Broadcast(p);

			return 0;
		}

		int GetServerList()
		{
			//ClientDatabase::ServerInfo si = ClientDatabase::GetInstance()->GetFirstServerAndPop();
			//if (si.TimeId == -1)
			//{
				LuaEmbedder::PushNull();
				LuaEmbedder::PushNull();
				LuaEmbedder::PushNull();
				LuaEmbedder::PushNull();
				LuaEmbedder::PushNull();
				LuaEmbedder::PushNull();
				LuaEmbedder::PushNull();
				LuaEmbedder::PushNull();

				return 8;
		//	}


			//LuaEmbedder::PushString(si.Name);
			//LuaEmbedder::PushString(si.IpAddress);
			//LuaEmbedder::PushInt(si.Port);
			//LuaEmbedder::PushInt(si.NoUsers);
			//LuaEmbedder::PushInt(si.MaxUsers);
			//LuaEmbedder::PushInt(si.NoSpectators);
			//LuaEmbedder::PushBool(si.GameStarted);
			//LuaEmbedder::PushBool(si.PasswordProtected);

			//return 8;

		}
	}

	namespace LuaClientNetwork
	{
		int StartPack();
		int Send();

		int Connect();
		int Disconnect();
		int IsConnected();
		int Update();

		//int WriteByte();
		int WriteFloat();
		//int WriteShort();
		int WriteString();
		int WriteBool();
		int WriteInt();

		//int ReadByte();
		int ReadFloat();
		//int ReadShort();
		int ReadString();
		int ReadBool();
		int ReadInt();

		int ResetNetworkMaps();

		void Embed()
		{
			LuaEmbedder::AddFunction("StartPack", &StartPack, "Client");
			LuaEmbedder::AddFunction("Send", &Send, "Client");

			LuaEmbedder::AddFunction("Connect", &Connect, "Client");
			LuaEmbedder::AddFunction("Disconnect", &Disconnect, "Client");
			LuaEmbedder::AddFunction("IsConnected", &IsConnected, "Client");
			LuaEmbedder::AddFunction("Update", &Update, "Client");

			//LuaEmbedder::AddFunction("WriteByte", &WriteByte, "Client");
			LuaEmbedder::AddFunction("WriteFloat", &WriteFloat, "Client");
			//LuaEmbedder::AddFunction("WriteShort", &WriteShort, "Client");
			LuaEmbedder::AddFunction("WriteString", &WriteString, "Client");
			LuaEmbedder::AddFunction("WriteBool", &WriteBool, "Client");
			LuaEmbedder::AddFunction("WriteInt", &WriteInt, "Client");

			//LuaEmbedder::AddFunction("ReadByte", &ReadByte, "Client");
			LuaEmbedder::AddFunction("ReadFloat", &ReadFloat, "Client");
			//LuaEmbedder::AddFunction("ReadShort", &ReadShort, "Client");
			LuaEmbedder::AddFunction("ReadString", &ReadString, "Client");
			LuaEmbedder::AddFunction("ReadBool", &ReadBool, "Client");
			LuaEmbedder::AddFunction("ReadInt", &ReadInt, "Client");

			LuaEmbedder::AddFunction("ResetNetworkMaps", &ResetNetworkMaps, "Client");
		}

		int StartPack()
		{
			if (!NetworkInstance::GetClient()->IsConnected())
				return 0;

			std::string identifier = LuaEmbedder::PullString(1);
			uint64_t id = NetworkInstance::GetClient()->GetPacketHandler()->StartPack("LuaPacket");

			NetworkInstance::GetClient()->GetPacketHandler()->WriteString(id, identifier.c_str());

			std::ostringstream ss;
			ss << id;
			LuaEmbedder::PushString(ss.str().c_str());

			return 1;
		}

		int Send()
		{
			if (!NetworkInstance::GetClient()->IsConnected())
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			Network::Packet* p = NetworkInstance::GetClient()->GetPacketHandler()->EndPack(id);
			NetworkInstance::GetClient()->Send(p);

			return 0;
		}

		int Connect()
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

			if (LuaEmbedder::IsString(1) && LuaEmbedder::IsInt(2) && LuaEmbedder::IsString(3))
			{
				ip		= LuaEmbedder::PullString(1);
				port	= LuaEmbedder::PullInt(2);
				pw		= LuaEmbedder::PullString(3);
			}
			else if (LuaEmbedder::IsString(1) && LuaEmbedder::IsInt(2))
			{
				ip = LuaEmbedder::PullString(1);
				port = LuaEmbedder::PullInt(2);
			}
			else if (LuaEmbedder::IsString(1))
			{
				ip = LuaEmbedder::PullString(1);
			}

			NetworkInstance::GetClient()->Connect(ip.c_str(), pw.c_str(), port, 0);

			return 0;
		}
		int Disconnect()
		{
			NetworkInstance::GetClient()->Disconnect();

			return 0;
		}
		int IsConnected()
		{
			LuaEmbedder::PushBool(NetworkInstance::GetClient()->IsConnected());
			return 1;
		}
		int Update()
		{
			NetworkInstance::GetClient()->Update(LuaEmbedder::PullFloat(1));

			return 0;
		}

		int WriteFloat()
		{
			if (!NetworkInstance::GetClient()->IsConnected())
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			NetworkInstance::GetClient()->GetPacketHandler()->WriteFloat(id, LuaEmbedder::PullFloat(2));

			return 0;
		}
		int WriteString()
		{
			if (!NetworkInstance::GetClient()->IsConnected())
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			NetworkInstance::GetClient()->GetPacketHandler()->WriteString(id, LuaEmbedder::PullString(2).c_str());

			return 0;
		}
		int WriteBool()
		{
			if (!NetworkInstance::GetClient()->IsConnected())
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			NetworkInstance::GetClient()->GetPacketHandler()->WriteByte(id, LuaEmbedder::PullBool(2));

			return 0;
		}
		int WriteInt()
		{
			if (!NetworkInstance::GetClient()->IsConnected())
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			NetworkInstance::GetClient()->GetPacketHandler()->WriteInt(id, LuaEmbedder::PullInt(2));

			return 0;
		}

		int ReadFloat()
		{
			if (!NetworkInstance::GetClient()->IsConnected())
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			float value = NetworkInstance::GetClient()->GetPacketHandler()->ReadFloat(id);
			LuaEmbedder::PushFloat(value);

			return 1;
		}
		int ReadString()
		{
			if (!NetworkInstance::GetClient()->IsConnected())
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			std::string value = NetworkInstance::GetClient()->GetPacketHandler()->ReadString(id);
			LuaEmbedder::PushString(value.c_str());

			return 1;
		}
		int ReadBool()
		{
			if (!NetworkInstance::GetClient()->IsConnected())
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			bool value = (bool)NetworkInstance::GetClient()->GetPacketHandler()->ReadByte(id);
			LuaEmbedder::PushBool(value);

			return 1;
		}
		int ReadInt()
		{
			if (!NetworkInstance::GetClient()->IsConnected())
				return 0;


			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			int value = NetworkInstance::GetClient()->GetPacketHandler()->ReadInt(id);
			LuaEmbedder::PushInt(value);

			return 1;
		}

		int ResetNetworkMaps()
		{
			NetworkInstance::GetNetworkHelper()->ResetNetworkMaps();
			return 0;
		}
	}

	namespace LuaServerNetwork
	{
		int StartPack();
		int Send();
		int Broadcast();

		int Start();
		int Stop();
		int IsRunning();
		int Update();
		int Kick();

		//int WriteByte();
		int WriteFloat();
		//int WriteShort();
		int WriteString();
		int WriteBool();
		int WriteInt();

		//int ReadByte();
		int ReadFloat();
		//int ReadShort();
		int ReadString();
		int ReadBool();
		int ReadInt();

		int SendEntity();
		int SendEntityKill();
		int BroadcastEntity();
		int BroadcastEntityKill();

		void Embed()
		{
			LuaEmbedder::AddFunction("StartPack", &StartPack, "Server");
			LuaEmbedder::AddFunction("Send", &Send, "Server");
			LuaEmbedder::AddFunction("Broadcast", &Broadcast, "Server");

			LuaEmbedder::AddFunction("Start", &Start, "Server");
			LuaEmbedder::AddFunction("Stop", &Stop, "Server");
			LuaEmbedder::AddFunction("IsRunning", &IsRunning, "Server");
			LuaEmbedder::AddFunction("Update", &Update, "Server");
			LuaEmbedder::AddFunction("Kick", &Kick, "Server");

			//LuaEmbedder::AddFunction("WriteByte", &WriteByte, "Server");
			LuaEmbedder::AddFunction("WriteFloat", &WriteFloat, "Server");
			//LuaEmbedder::AddFunction("WriteShort", &WriteShort, "Server");
			LuaEmbedder::AddFunction("WriteString", &WriteString, "Server");
			LuaEmbedder::AddFunction("WriteBool", &WriteBool, "Server");
			LuaEmbedder::AddFunction("WriteInt", &WriteInt, "Server");
			

			//LuaEmbedder::AddFunction("ReadByte", &ReadByte, "Server");
			LuaEmbedder::AddFunction("ReadFloat", &ReadFloat, "Server");
			//LuaEmbedder::AddFunction("ReadShort", &ReadShort, "Server");
			LuaEmbedder::AddFunction("ReadString", &ReadString, "Server");
			LuaEmbedder::AddFunction("ReadBool", &ReadBool, "Server");
			LuaEmbedder::AddFunction("ReadInt", &ReadInt, "Server");


			LuaEmbedder::AddFunction("SendEntity", &SendEntity, "Server");
			LuaEmbedder::AddFunction("SendEntityKill", &SendEntityKill, "Server");
			LuaEmbedder::AddFunction("BroadcastEntity", &BroadcastEntity, "Server");
			LuaEmbedder::AddFunction("BroadcastEntityKill", &BroadcastEntityKill, "Server");
		}

		int StartPack()
		{
			if (!NetworkInstance::GetServer()->IsRunning())
				return 0;

			std::string identifier = LuaEmbedder::PullString(1);
			uint64_t id = NetworkInstance::GetServer()->GetPacketHandler()->StartPack("LuaPacket");

			NetworkInstance::GetServer()->GetPacketHandler()->WriteString(id, identifier.c_str());

			std::ostringstream ss;
			ss << id;
			LuaEmbedder::PushString(ss.str().c_str());

			return 1;
		}
		int Send()
		{
			if (!NetworkInstance::GetServer()->IsRunning())
				return 0;

			std::string sId = LuaEmbedder::PullString(1);

			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			Network::Packet* p = NetworkInstance::GetServer()->GetPacketHandler()->EndPack(id);

			Network::NetConnection nc;
			if (LuaEmbedder::IsString(2) && LuaEmbedder::IsInt(3))
			{
				nc = Network::NetConnection(LuaEmbedder::PullString(2).c_str(), LuaEmbedder::PullInt(3));
			}
			else if (LuaEmbedder::IsString(2))
			{
				// Get Netconnection based on username
			}

			NetworkInstance::GetServer()->Send(p, nc);

			return 0;
		}
		int Broadcast()
		{
			if (!NetworkInstance::GetServer()->IsRunning())
				return 0;

			std::string sId = LuaEmbedder::PullString(1);

			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			Network::Packet* p = NetworkInstance::GetServer()->GetPacketHandler()->EndPack(id);

			Network::NetConnection nc;
			if (LuaEmbedder::IsString(2) && LuaEmbedder::IsInt(3))
			{
				nc = Network::NetConnection(LuaEmbedder::PullString(2).c_str(), LuaEmbedder::PullInt(3));
			}
			else if (LuaEmbedder::IsString(2))
			{
				// Get Netconnection based on username
			}


			NetworkInstance::GetServer()->Broadcast(p, nc);

			return 0;
		}

		int Start()
		{
			if (NetworkInstance::GetClient()->IsConnected())
				NetworkInstance::GetClient()->Disconnect();

			if (NetworkInstance::GetServer()->IsRunning())
				NetworkInstance::GetServer()->Stop();


			std::string pw = NetworkInstance::GetServer()->GetServerPassword();
			unsigned int port = NetworkInstance::GetServer()->GetIncomingPort();
			unsigned int connections = NetworkInstance::GetServer()->GetMaxConnections();

			if (LuaEmbedder::IsString(1) && LuaEmbedder::IsInt(2) && LuaEmbedder::IsString(3))
			{
				port = LuaEmbedder::PullInt(1);
				pw = LuaEmbedder::PullString(2);
				connections = LuaEmbedder::PullInt(3);
			}
			else if (LuaEmbedder::IsString(1) && LuaEmbedder::IsInt(2))
			{
				port = LuaEmbedder::PullInt(1);
				pw = LuaEmbedder::PullString(2);
			}
			else if (LuaEmbedder::IsString(1))
			{
				port = LuaEmbedder::PullInt(1);
			}

			NetworkInstance::GetServer()->Start(port, pw.c_str(), connections);


			return 0;
		}
		int Stop()
		{
			if (NetworkInstance::GetClient()->IsConnected() && strcmp(NetworkInstance::GetClient()->GetRemoteAddress(), "127.0.0.1") == 0)
				NetworkInstance::GetClient()->Disconnect();

			if (NetworkInstance::GetServer()->IsRunning())
				NetworkInstance::GetServer()->Stop();

			return 0;
		}
		int IsRunning()
		{
			LuaEmbedder::PushBool(NetworkInstance::GetServer()->IsRunning());

			return 1;
		}
		int Update()
		{
			NetworkInstance::GetServer()->Update(LuaEmbedder::PullFloat(1));

			return 0;
		}

		int Kick()
		{
			std::string ip = LuaEmbedder::PullString(1);
			unsigned int port = LuaEmbedder::PullInt(2);
			std::string reason = "";

			if(LuaEmbedder::IsString(3))
				reason = LuaEmbedder::PullString(3);

			Network::NetConnection nc(ip.c_str(), port);
			NetworkInstance::GetServer()->Kick(nc, reason.c_str());

			return 0;
		}

		int WriteFloat()
		{
			if (!NetworkInstance::GetServer()->IsRunning())
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			NetworkInstance::GetServer()->GetPacketHandler()->WriteFloat(id, LuaEmbedder::PullFloat(2));

			return 0;
		}
		int WriteString()
		{
			if (!NetworkInstance::GetServer()->IsRunning())
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			NetworkInstance::GetServer()->GetPacketHandler()->WriteString(id, LuaEmbedder::PullString(2).c_str());

			return 0;
		}
		int WriteBool()
		{
			if (!NetworkInstance::GetServer()->IsRunning())
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			NetworkInstance::GetServer()->GetPacketHandler()->WriteByte(id, LuaEmbedder::PullBool(2));

			return 0;
		}
		int WriteInt()
		{
			if (!NetworkInstance::GetServer()->IsRunning())
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			NetworkInstance::GetServer()->GetPacketHandler()->WriteInt(id, LuaEmbedder::PullInt(2));

			return 0;
		}

		int ReadFloat()
		{
			if (!NetworkInstance::GetServer()->IsRunning())
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			float value = NetworkInstance::GetServer()->GetPacketHandler()->ReadFloat(id);
			LuaEmbedder::PushFloat(value);

			return 1;
		}
		int ReadString()
		{
			if (!NetworkInstance::GetServer()->IsRunning())
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			std::string value = NetworkInstance::GetServer()->GetPacketHandler()->ReadString(id);
			LuaEmbedder::PushString(value.c_str());

			return 1;
		}
		int ReadBool()
		{
			if (!NetworkInstance::GetServer()->IsRunning())
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			bool value = (bool)NetworkInstance::GetServer()->GetPacketHandler()->ReadByte(id);
			LuaEmbedder::PushBool(value);

			return 1;
		}
		int ReadInt()
		{
			if (!NetworkInstance::GetServer()->IsRunning())
				return 0;

			std::string sId = LuaEmbedder::PullString(1);
			char* end;
			uint64_t id = strtoull(sId.c_str(), &end, 10);

			int value = NetworkInstance::GetServer()->GetPacketHandler()->ReadInt(id);
			LuaEmbedder::PushInt(value);

			return 1;
		}

		int SendEntity()
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (!server->IsRunning())
				return 0;

			unsigned int id = LuaEmbedder::PullInt(1);
			std::string ip = LuaEmbedder::PullString(2);
			unsigned int port = LuaEmbedder::PullInt(3);

			Network::Packet* p = NetworkInstance::GetNetworkHelper()->WriteEntityAll(server->GetPacketHandler(), id);
			Network::NetConnection nc(ip.c_str(), port);
			server->Send(p, nc);

			return 0;
		}
		int SendEntityKill()
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (!server->IsRunning())
				return 0;

			unsigned int id		= LuaEmbedder::PullInt(1);
			std::string ip		= LuaEmbedder::PullString(2);
			unsigned int port	= LuaEmbedder::PullInt(3);

			Network::Packet* p = NetworkInstance::GetNetworkHelper()->WriteEntityKill(server->GetPacketHandler(), id);
			Network::NetConnection nc(ip.c_str(), port);
			server->Send(p, nc);

			return 0;
		}

		int BroadcastEntity()
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (!server->IsRunning())
				return 0;

			unsigned int id = LuaEmbedder::PullInt(1);

			Network::Packet* p = NetworkInstance::GetNetworkHelper()->WriteEntityAll(server->GetPacketHandler(), id);
			server->Broadcast(p);

			return 0;
		}
		int BroadcastEntityKill()
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (!server->IsRunning())
				return 0;

			unsigned int id = LuaEmbedder::PullInt(1);

			Network::Packet* p = NetworkInstance::GetNetworkHelper()->WriteEntityKill(server->GetPacketHandler(), id);
			server->Broadcast(p);

			return 0;
		}

	}
}