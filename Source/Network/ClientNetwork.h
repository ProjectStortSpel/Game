#ifndef CLIENTNETWORK_H
#define CLIENTNETWORK_H

#include <thread>
#include <time.h>
#include <stdio.h>

#include "BaseNetwork.h"

namespace Network
{
	class DECLSPEC ClientNetwork : public BaseNetwork
	{
	public:

		ClientNetwork();
		~ClientNetwork();

		// Connect to server using already predefined settings
		bool Connect();

		// Connect to server with Ip Address, Password, outgoing port & incoming port
		// Note that all previously information will be overridden
		bool Connect(const char* _ipAddress, const char* _password, const int& _outgoing, const int& _incomingPort);
		// Disconnect from the server 
		// This is called in the deconstructor and is not necessary to close the program graceful
		void Disconnect();
		// Send a packet to the server
		void Send(Packet* _packet);

		// Returns the remote Ip Address the client connects to
		const char* GetRemoteAddress(void) { return m_remoteAddress->c_str(); }
		// Returns the outgoing port the client connects to
		const int GetOutgoingPort(void) { return *m_outgoingPort; }
		const float GetPing(void) { return *m_ping; }

		bool IsConnected() { return *m_connected; }

		// Set the remote Ip Address the client will connect to
		void SetRemoteAddress(const char* _ipAddress) { *m_remoteAddress = _ipAddress; }
		// Set the outgoing port which the client will connect to
		void SetOutgoingPort(const int _port) { *m_outgoingPort = _port; }

		// Bind function which will trigger when the client connect to the server
		void SetOnConnectedToServer(std::function<void(NetConnection, const char*)> _function);
		// Bind function which will trigger when the client disconnect from the server
		void SetOnDisconnectedFromServer(std::function<void(NetConnection, const char*)> _function);
		// Bind function which will trigger when the client disconnect from the server
		void SetOnTimedOutFromServer(std::function<void(NetConnection, const char*)> _function);
		// Bind function which will trigger when the client fails to connect to the server
		void SetOnFailedToConnect(std::function<void(NetConnection, const char*)> _function);
		// Bind function which will trigger when the client tried to connect with an invalid password
		void SetOnPasswordInvalid(std::function<void(NetConnection, const char*)> _function);
		// Bind function which will trigger when the client is kicked from the server.
		void SetOnKickedFromServer(std::function<void(NetConnection, const char*)> _function);
		// Bind function which will trigger when the client is banned from the server.
		void SetOnBannedFromServer(std::function<void(NetConnection, const char*)> _function);
		// Bind function which will trigger when the client tried to connect to a full server.
		void SetOnServerFull(std::function<void(NetConnection, const char*)> _function);



		// Bind function which will trigger when a client connect to the server
		void SetOnRemotePlayerConnected(std::function<void(NetConnection, const char*)> _function);
		// Bind function which will trigger when a client disconnect from the server
		void SetOnRemotePlayerDisconnected(std::function<void(NetConnection, const char*)> _function);
		// Bind function which will trigger when a client fails to connect to the server
		void SetOnRemotePlayerTimedOut(std::function<void(NetConnection, const char*)> _function);
		// Bind function which will trigger when a client is kicked from the server
		void SetOnRemotePlayerKicked(std::function<void(NetConnection, const char*)> _function);
		// Bind function which will trigger when a client is banned from the server
		void SetOnRemotePlayerBanned(std::function<void(NetConnection, const char*)> _function);


	private:

		void ReceivePackets(void);

		void UpdateNetUsage(float& _dt);
		void UpdateTimeOut(float& _dt);

		void NetPasswordInvalid(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection);
		void NetConnectionAccepted(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection);
		void NetConnectionServerFull(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection);

		void NetConnectionLost(NetConnection& _connection);
		void NetConnectionDisconnected(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection);

		void NetConnectionKicked(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection);
		void NetConnectionBanned(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection);

		void NetPing(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection);
		void NetPong(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection);

		//Remote
		void NetRemoteConnectionAccepted(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection);

		void NetRemoteConnectionLost(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection);
		void NetRemoteConnectionDisconnected(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection);

		void NetRemoteConnectionKicked(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection);
		void NetRemoteConnectionBanned(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection);


	private:
		ISocket* m_socket;

		std::string* m_remoteAddress;
		int* m_outgoingPort;
		bool* m_socketBound;

		std::thread* m_receivePacketsThread;
		bool* m_receivePacketsThreadAlive;
		bool* m_connected;

		float* m_ping;
		float* m_sendTime;
		float* m_receiveTime;

		float* m_currentTimeOutIntervall;
		int* m_currentIntervallCounter;

		std::vector<std::function<void(NetConnection, const char*)>>* m_onConnectedToServer;
		std::vector<std::function<void(NetConnection, const char*)>>* m_onDisconnectedFromServer;
		std::vector<std::function<void(NetConnection, const char*)>>* m_onTimedOutFromServer;
		std::vector<std::function<void(NetConnection, const char*)>>* m_onFailedToConnect;
		std::vector<std::function<void(NetConnection, const char*)>>* m_onPasswordInvalid;
		std::vector<std::function<void(NetConnection, const char*)>>* m_onKickedFromServer;
		std::vector<std::function<void(NetConnection, const char*)>>* m_onBannedFromServer;
		std::vector<std::function<void(NetConnection, const char*)>>* m_onServerFull;

		std::vector<std::function<void(NetConnection, const char*)>>* m_onRemotePlayerConnected;
		std::vector<std::function<void(NetConnection, const char*)>>* m_onRemotePlayerDisconnected;
		std::vector<std::function<void(NetConnection, const char*)>>* m_onRemotePlayerTimedOut;
		std::vector<std::function<void(NetConnection, const char*)>>* m_onRemotePlayerKicked;
		std::vector<std::function<void(NetConnection, const char*)>>* m_onRemotePlayerBanned;


	};

}


#endif