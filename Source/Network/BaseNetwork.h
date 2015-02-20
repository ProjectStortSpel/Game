#ifndef BASENETWORK_H
#define BASENETWORK_H

#include <vector>
#include <queue>
#include <functional>

#ifdef WIN32
#include "WinSocket.h"
#else
#include "LinSocket.h"
#endif

#include "PacketHandler.h"
#include "Packet.h"

namespace Network
{

	typedef std::function<void(PacketHandler*, uint64_t&, NetConnection&)> NetMessageHook;
	typedef std::function<void(NetConnection&, const char*)> NetEvent;

	class DECLSPEC BaseNetwork
	{
	public:
		BaseNetwork();
		virtual ~BaseNetwork();

		int PopAndExecutePacket(void);
		void Update(float _dt);
		void AddNetworkHook(const char* _name, NetMessageHook& _hook);

		const char* GetLocalAddress(void) { return m_localAddress->c_str(); }
		const char* GetServerPassword(void) { return m_password->c_str(); }
		const unsigned int GetIncomingPort(void) { return *m_incomingPort; }

		const int GetTotalBytesReceived(void) { return *m_totalDataReceived; }
		const int GetTotalBytesSent(void) { return *m_totalDataSent; }

		PacketHandler* GetPacketHandler() { return m_packetHandler; }

		void SetServerPassword(const char* _password) { *m_password = _password; }
		void SetIncomingPort(const int _port) { *m_incomingPort = _port; }

		void SetMaxTimeOutCounter(int _max) { *m_maxIntervallCounter = _max; }
		void SetMaxTimeOutIntervall(float _max) { *m_maxTimeOutIntervall = _max; }

		void Clear();

	protected:

		void TriggerEvent(std::vector<NetEvent>* _event, NetConnection& _connection, const char* _message);
		void HandlePacket(Packet* _packet);

		virtual void UpdateTimeOut(float& _dt) = 0;
		virtual void UpdateNetUsage(float& _dt) = 0;

		unsigned int GetMillisecondsTime();

	protected:

		std::map<std::string, NetMessageHook>* m_customFunctions;
		std::map<char, NetMessageHook>* m_systemFunctions;

		PacketHandler* m_packetHandler;

		std::string* m_localAddress;
		std::string* m_password;

		unsigned int* m_incomingPort;

		int* m_totalDataReceived;
		int* m_totalDataSent;

		int* m_currentDataReceived;
		int* m_currentDataSent;

		float* m_usageDataTimer;

		float* m_maxTimeOutIntervall;
		int* m_maxIntervallCounter;

	private:
		SDL_mutex* m_customPacketLock;
		SDL_mutex* m_systemPacketLock;

		std::queue<Packet*>* m_customPackets;
		std::queue<Packet*>* m_systemPackets;

	};

}
#endif