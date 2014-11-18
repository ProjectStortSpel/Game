#ifndef PROJEKTSERVER_H
#define PROJEKTSERVER_H

#include <functional>
#include <queue>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/BitStream.h>
#include <mutex>
#include <thread>
#include <SDL/SDL.h>
#include "PacketHandler.h"


class DECLSPEC EventHandler
{
public:
	void AddHandler()
	{
		printf("HEJ\n");
	}
};

class DECLSPEC Server
{


public:
	Server();
	~Server();

	//void Test() { return; }

	void Connect();
	void Disconect();

	void Broadcast(PacketHandler::Packet _packet);

	const char* GetIp(void) { return m_ipAddress.c_str(); }
	const char* GetPassword(void) { return m_password.c_str(); }
	const int GetPort(void) { return m_port; }

	void SetNetPort(const int _port) { m_port = _port; }
	void SetPassword(const char* _password) { m_password = _password; }

	PacketHandler::Packet* GetPacket();

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

#pragma warning( disable : 4251 )

	std::function<void()> m_onUserConnect;
	std::function<void()> m_onUserDisconnect;
	std::function<void()> m_onUserTimeOut;

	std::string m_ipAddress;
	std::string m_password;
	int m_port;

	std::queue<PacketHandler::Packet> m_packets;
	std::mutex m_packetLock;

	RakNet::RakPeerInterface *m_server;

	RakNet::BitStream m_stream;

	std::thread m_thread;

#pragma warning( default : 4251 )

};

#endif