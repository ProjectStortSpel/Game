#include "LuaNetwork.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "../../NetworkInstance.h"
#include <sstream>

namespace LuaBridge
{
	namespace LuaClientNetwork
	{
		int StartPack();
		int Send();

		int Connect();
		int Disconnect();
		int IsConnected();
		int Update();
		int HandlePacket();

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
			LuaEmbedder::AddFunction("HandlePacket", &HandlePacket, "Client");

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
			uint64_t id = NetworkInstance::GetClient()->GetPacketHandler()->StartPack(identifier.c_str());

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
		int HandlePacket()
		{
			int remaningPackets = NetworkInstance::GetClient()->PopAndExecutePacket();
			LuaEmbedder::PushInt(remaningPackets);

			return 1;
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
			return 1;
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
		int HandlePacket();

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
			LuaEmbedder::AddFunction("HandlePacket", &HandlePacket, "Server");

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
			uint64_t id = NetworkInstance::GetServer()->GetPacketHandler()->StartPack(identifier.c_str());

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
		int HandlePacket()
		{
			int remainingPackets = NetworkInstance::GetServer()->PopAndExecutePacket();
			LuaEmbedder::PushInt(remainingPackets);

			return 1;
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
		}

		int BroadcastEntity()
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (!server->IsRunning())
				return 0;

			unsigned int id = LuaEmbedder::PullInt(1);

			Network::Packet* p = NetworkInstance::GetNetworkHelper()->WriteEntityAll(server->GetPacketHandler(), id);
			server->Broadcast(p);
		}
		int BroadcastEntityKill()
		{
			Network::ServerNetwork* server = NetworkInstance::GetServer();
			if (!server->IsRunning())
				return 0;

			unsigned int id = LuaEmbedder::PullInt(1);

			Network::Packet* p = NetworkInstance::GetNetworkHelper()->WriteEntityKill(server->GetPacketHandler(), id);
			server->Broadcast(p);
		}

	}
}