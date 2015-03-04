#ifndef SERVERNETWORK_H
#define SERVERNETWORK_H

#include <thread>
#include <vector>

#include "BaseNetwork.h"

namespace Network
{

	class DECLSPEC ServerNetwork : public BaseNetwork
	{

	public:
		ServerNetwork(void);
		~ServerNetwork(void);

		bool Start(void);
		bool Start(unsigned int& _incomingPort, const char* _password, unsigned int& _maxConnections);
		bool Stop(void);

		void Broadcast(Packet* _packet, const NetConnection& _exclude = NetConnection());
		void Send(Packet* _packet, NetConnection& _receiver);
		// Send a message to a list of clients
		void Send(Packet* _packet, std::vector<NetConnection>& _receivers);

		void SetOnPlayerConnected(NetEvent& _function);
		void SetOnPlayerDisconnected(NetEvent& _function);
		void SetOnPlayerTimedOut(NetEvent& _function);
		void SetOnServerShutdown(NetEvent& _function);

		bool IsRunning() { return *m_running; }
		unsigned int GetMaxConnections() { return *m_maxConnections; }
		unsigned int GetIncomingPort() { return *m_incomingPort; }
		const std::vector<NetConnection>& GetConnectedClients() { return *m_connectedClientsNC; }

		void ResetNetworkEvents();

	private:

		void Send(Packet* _packet, NetConnection& _connection, bool _deletePacket);

		void ReceivePackets(ISocket* _socket, const std::string _name);
		void ListenForConnections(void);
		void Send(Packet* _packet, ISocket* _socket);

		void UpdateNetUsage(float& _dt);
		void UpdateTimeOut(float& _dt);

		void NetPasswordAttempt(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection);
		void NetConnectionLost(NetConnection& _connection);
		void NetConnectionDisconnected(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection);
		void NetPing(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection);
		void NetPong(PacketHandler* _packetHandler, uint64_t& _id, NetConnection& _connection);


	private:

		bool* m_running;
		unsigned int* m_maxConnections;

		ISocket* m_listenSocket;
		std::thread* m_listenThread;
		std::map<NetConnection, std::thread>* m_receivePacketThreads;
		std::map<NetConnection, ISocket*>* m_connectedClients;
		std::map<NetConnection, float>* m_currentTimeOutIntervall;
		std::map<NetConnection, int>* m_currentIntervallCounter;

		std::vector<NetConnection>*	m_connectedClientsNC;

		std::vector<NetEvent>* m_onPlayerConnected;
		std::vector<NetEvent>* m_onPlayerDisconnected;
		std::vector<NetEvent>* m_onPlayerTimedOut;
		std::vector<NetEvent>* m_onServerShutdown;


		SDL_mutex* m_connectedClientsLock;
		SDL_mutex* m_dataSentLock;
		SDL_mutex* m_dataReceiveLock;
		SDL_mutex* m_timeOutLock;
	};
}


#endif