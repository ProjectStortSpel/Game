#ifndef BASENETWORK_H
#define BASENETWORK_H

#include <vector>
#include <map>
#include <queue>
#include <mutex>

#include "Stdafx.h"
#include "NetTypeMessageID.h"

#ifdef WIN32
#include "WinSocket.h"
#else
#include "LinSocket.h"
#endif

#include "PacketHandler.h"
#include "Packet.h"

namespace Network
{

	typedef std::function<void(PacketHandler*, uint64_t, NetConnection)> NetMessageHook;
	typedef std::function<void(NetConnection)> NetEvent;

	class DECLSPEC BaseNetwork
	{

		friend class PacketHandler;

	public:
		BaseNetwork();
		virtual ~BaseNetwork();

		// Returns get local Ip Address
		const char* GetLocalAddress(void) { return m_localAddress.c_str(); }
		// Returns the server password
		const char* GetServerPassword(void) { return m_password.c_str(); }
		// Returns the incoming port
		const int GetIncomingPort(void) { return m_incomingPort; }

		const float GetTotalBytesReceived(void) { return m_totalDataReceived; }
		const float GetTotalBytesSent(void) { return m_totalDataSent; }

		const float GetCurrentBytesReceived(void) { return m_currentDataReceived; }
		const float GetCurrentBytesSent(void) { return m_currentDataSent; }

		// Reads the oldest user specific packet and calls its specified function
		// Will return the number of packets remaining
		int TriggerPacket(void);

		void Update(float _dt);

		// Set the incoming port
		void SetIncomingPort(const int _port) { m_incomingPort = _port; }
		// Set the server password
		void SetServerPassword(const char* _password) { m_password = _password; }

		void AddNetworkHook(char* _name, NetMessageHook _hook);
		//NetMessageHook* GetNetworkFunction(NetTypeMessageId _function);

		PacketHandler* GetPacketHandler() { return &m_packetHandler; }

		void SetMaxTimeOutCounter(int _max) { m_maxIntervallCounter = _max; }
		void SetMaxTimeOutIntervall(float _max) { m_maxTimeOutIntervall = _max; }

	protected:
		void TriggerEvent(NetMessageHook _function, uint64_t _packetId, NetConnection _connection);
		void TriggerEvent(NetEvent _function, NetConnection _connection);

		void HandlePacket(Packet* _packet);

		virtual void UpdateTimeOut(float _dt) = 0;
		virtual void UpdateNetUsage(float _dt) = 0;

		unsigned int GetMillisecondsTime();

	protected:

#pragma warning( disable : 4251 )


		std::map < std::string, NetMessageHook > m_userFunctions;
		std::map < char, NetMessageHook > m_networkFunctions;

		std::queue<Packet*> m_systemPackets;
		std::queue<Packet*> m_customPackets;
		std::mutex m_systemPacketLock;
		std::mutex m_customPacketLock;

		std::string m_localAddress;
		std::string m_password;
		unsigned int m_incomingPort;

		PacketHandler m_packetHandler;

		float m_maxTimeOutIntervall;
		int m_maxIntervallCounter;

		char m_packetData[MAX_PACKET_SIZE];

		float m_totalDataReceived;
		float m_totalDataSent;

		float m_currentDataReceived;
		float m_currentDataSent;
		float m_usageDataTimer;

#pragma warning( default : 4251 )

	private:
		//NetMessageHook* GetUserFunction(std::string _functionName);
		//NetMessageHook* GetNetworkFunction(char _functionIdentifier);

	};

}

#endif