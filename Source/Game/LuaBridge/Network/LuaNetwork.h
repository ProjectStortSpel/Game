#ifndef LUANETWORK_H
#define LUANETWORK_H

#include <LuaEmbedder/LuaEmbedder.h>
#include "Network/BaseNetwork.h"

namespace LuaBridge
{
	namespace LuaNetwork
	{
		void Embed(lua_State* L);
        void SetClientLuaState(lua_State* L);
        void SetServerLuaState(lua_State* L);
	}

	namespace LuaNetworkEvents
	{
		extern std::vector<Network::NetEvent> g_onConnectedToServer;
		extern std::vector<Network::NetEvent> g_onDisconnectedFromServer;
		extern std::vector<Network::NetEvent> g_onTimedOutFromServer;
		extern std::vector<Network::NetEvent> g_onFailedToConnect;
		extern std::vector<Network::NetEvent> g_onPasswordInvalid;
		extern std::vector<Network::NetEvent> g_onKickedFromServer;
		extern std::vector<Network::NetEvent> g_onBannedFromServer;
		extern std::vector<Network::NetEvent> g_onServerFull;

		extern std::vector<Network::NetEvent> g_onRemotePlayerConnected;
		extern std::vector<Network::NetEvent> g_onRemotePlayerDisconnected;
		extern std::vector<Network::NetEvent> g_onRemotePlayerTimedOut;
		extern std::vector<Network::NetEvent> g_onRemotePlayerKicked;
		extern std::vector<Network::NetEvent> g_onRemotePlayerBanned;


		extern std::vector<Network::NetEvent> g_onPlayerConnected;
		extern std::vector<Network::NetEvent> g_onPlayerDisconnected;
		extern std::vector<Network::NetEvent> g_onPlayerTimedOut;

		void Clear();
	}

	namespace LuaClientNetwork
	{
		void Embed(lua_State* L);
	}

	namespace LuaServerNetwork
	{
		void Embed(lua_State* L);
	}
}

#endif