#ifndef PROJEKTCLIENT_H
#define PROJEKTCLIENT_H


#include <functional>
#include <queue>
#include <RakNet/RakPeerInterface.h>
#include <mutex>
#include <thread>
#include <SDL/SDL.h>
#include "PacketHandler.h"

class DECLSPEC Client
{
public:
	Client();
	~Client();

	void Connect();
	void Disconect();

	void Send(PacketHandler::Packet _packet);

	const char* GetIp(void) { return m_ipAddress.c_str(); }
	const char* GetPassword(void) { return m_password.c_str(); }
	const int GetPort(void) { return m_port; }
	const int GetClientPort(void) { return m_clientPort; }

	void SetIp(const char *_ip) { m_ipAddress = _ip; }
	void SetNetPort(const int _port) { m_port = _port; }
	void SetClientPort(const int _port) { m_clientPort = _port; }
	void SetPassword(const char* _password) { m_password = _password; }

	RakNet::Packet* GetPacket();

	// std::bind(&Class:Function, pointer to object, number of arguments (0)
	void SetOnUserConnect(std::function<void()> _function);
	// std::bind(&Class:Function, pointer to object, number of arguments (0)
	void SetOnUserDisconnect(std::function<void()> _function);
	// std::bind(&Class:Function, pointer to object, number of arguments (0)
	void SetOnUserTimeOut(std::function<void()> _function);


private:
	void Run(void);
	void RecivePackets(void);
	unsigned char GetPacketIdentifier(RakNet::Packet *p);

private:
	std::function<void()> m_onUserConnect;
	std::function<void()> m_onUserDisconnect;
	std::function<void()> m_onUserTimeOut;

	std::string m_ipAddress;
	std::string m_password;
	int m_port, m_clientPort;

	std::queue<RakNet::Packet*> m_packets;
	std::mutex m_packetLock;

	RakNet::RakPeerInterface *m_client;

	std::thread m_thread;

};

#endif
