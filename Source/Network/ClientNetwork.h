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
		void SetOnConnectedToServer(NetEvent& _function);
		// Bind function which will trigger when the client disconnect from the server
		void SetOnDisconnectedFromServer(NetEvent& _function);
		// Bind function which will trigger when the client disconnect from the server
		void SetOnTimedOutFromServer(NetEvent& _function);
		// Bind function which will trigger when the client fails to connect to the server
		void SetOnFailedToConnect(NetEvent& _function);
		// Bind function which will trigger when the client tried to connect with an invalid password
		void SetOnPasswordInvalid(NetEvent& _function);
		// Bind function which will trigger when the client is kicked from the server.
		void SetOnKickedFromServer(NetEvent& _function);
		// Bind function which will trigger when the client is banned from the server.
		void SetOnBannedFromServer(NetEvent& _function);
		// Bind function which will trigger when the client tried to connect to a full server.
		void SetOnServerFull(NetEvent& _function);



		// Bind function which will trigger when a client connect to the server
		void SetOnRemotePlayerConnected(NetEvent& _function);
		// Bind function which will trigger when a client disconnect from the server
		void SetOnRemotePlayerDisconnected(NetEvent& _function);
		// Bind function which will trigger when a client fails to connect to the server
		void SetOnRemotePlayerTimedOut(NetEvent& _function);
		// Bind function which will trigger when a client is kicked from the server
		void SetOnRemotePlayerKicked(NetEvent& _function);
		// Bind function which will trigger when a client is banned from the server
		void SetOnRemotePlayerBanned(NetEvent& _function);

		void ResetNetworkEvents();

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

		std::vector<NetEvent>* m_onConnectedToServer;
		std::vector<NetEvent>* m_onDisconnectedFromServer;
		std::vector<NetEvent>* m_onTimedOutFromServer;
		std::vector<NetEvent>* m_onFailedToConnect;
		std::vector<NetEvent>* m_onPasswordInvalid;
		std::vector<NetEvent>* m_onKickedFromServer;
		std::vector<NetEvent>* m_onBannedFromServer;
		std::vector<NetEvent>* m_onServerFull;

		std::vector<NetEvent>* m_onRemotePlayerConnected;
		std::vector<NetEvent>* m_onRemotePlayerDisconnected;
		std::vector<NetEvent>* m_onRemotePlayerTimedOut;
		std::vector<NetEvent>* m_onRemotePlayerKicked;
		std::vector<NetEvent>* m_onRemotePlayerBanned;


		char m_packetData[MAX_PACKET_SIZE];
	};

}


#endif