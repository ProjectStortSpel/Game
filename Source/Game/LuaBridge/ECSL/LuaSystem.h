#ifndef LUASYSTEM_H
#define LUASYSTEM_H

#include "ECSL/Interfaces/System.h"
#include "Network/Packet.h"
#include <LuaEmbedder/LuaEmbedder.h>

#include <string>
#include <map>

namespace LuaBridge
{
  class LuaSystem : public ECSL::System
  {
  public:
    LuaSystem(lua_State* L);
	~LuaSystem();
    
    static void Embed(lua_State* L);
    
    void Initialize();
	void Update(const ECSL::RuntimeInfo& _runtime);
	void EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);
	void EntitiesRemoved(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities);
	void PostInitialize();
	
	void SetLuaState(lua_State* L) { m_L = L; }
    
  private:
	int SetName(lua_State* L);

    int GetComponent(lua_State* L);
    
    int AddComponentTypeToFilter(lua_State* L);
    int GetEntities(lua_State* L);
    
    int EntityHasComponent(lua_State* L);

	int InitializeNetworkEvents(lua_State* L);

	int UsingUpdate(lua_State* L);
	int UsingEntitiesAdded(lua_State* L);
	int UsingEntitiesRemoved(lua_State* L);

	void OnBannedFromServer(Network::NetConnection _nc, const char* _message);
	void OnConnectedToServer(Network::NetConnection _nc, const char* _message);
	void OnDisconnectedFromServer(Network::NetConnection _nc, const char* _message);
	void OnFailedToConnect(Network::NetConnection _nc, const char* _message);
	void OnKickedFromServer(Network::NetConnection _nc, const char* _message);
	void OnPasswordInvalid(Network::NetConnection _nc, const char* _message);
	void OnRemotePlayerBanned(Network::NetConnection _nc, const char* _message);
	void OnRemotePlayerConnected(Network::NetConnection _nc, const char* _message);
	void OnRemotePlayerDisconnected(Network::NetConnection _nc, const char* _message);
	void OnRemotePlayerKicked(Network::NetConnection _nc, const char* _message);
	void OnServerFull(Network::NetConnection _nc, const char* _message);
	void OnTimedOutFromServer(Network::NetConnection _nc, const char* _message);

	void OnPlayerConnected(Network::NetConnection _nc, const char* _message);
	void OnPlayerDisconnected(Network::NetConnection _nc, const char* _message);
	void OnPlayerTimedOut(Network::NetConnection _nc, const char* _message);
	
  private:
    lua_State* m_L;
  };
}

#endif