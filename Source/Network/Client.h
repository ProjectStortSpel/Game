#ifndef PROJEKTCLIENT_H
#define PROJEKTCLIENT_H


#include <functional>
#include <queue>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/BitStream.h>
#include <mutex>
#include <thread>
#include <SDL/SDL.h>
#include "PacketHandler.h"
#include "Stdafx.h"

class DECLSPEC Client
{
public:

	Client();
	~Client();

	void Connect();
	void Connect(const char* _ipAddress, const char* m_password, const int m_port, const int m_clientPort);
	void Disconect();

	void StartListen();
	void StopListen();
	void Send(PacketHandler::Packet _packet);

	const char* GetIp(void) { return m_ipAddress.c_str(); }
	const char* GetPassword(void) { return m_password.c_str(); }
	const int GetPort(void) { return m_port; }
	const int GetClientPort(void) { return m_clientPort; }

	void SetIp(const char *_ip) { m_ipAddress = _ip; }
	void SetNetPort(const int _port) { m_port = _port; }
	void SetClientPort(const int _port) { m_clientPort = _port; }
	void SetPassword(const char* _password) { m_password = _password; }

	PacketHandler::Packet* GetPacket();

	// std::bind(&Class:Function, pointer to object, number of arguments (0)

	// Bind function which will trigger when the client connect to the server
	void SetOnConnectedToServer(NetEvent _function);
	// Bind function which will trigger when the client disconnect from the server
	void SetOnDisconnectedFromServer(NetEvent _function);
	// Bind function which will trigger when the client fails to connect to the server
	void SetOnFailedToConnect(NetEvent _function);
	// Bind function which will trigger when another player connects to the server
	void SetOnPlayerConnected(NetEvent _function);
	// Bind function which will trigger when another player disconnects from the server
	void SetOnPlayerDisconnected(NetEvent _function);


private:
	void Run(void);
	void RecivePackets(void);
	unsigned char GetPacketIdentifier(RakNet::Packet *p);
	void TriggerEvent(NetEvent _function, unsigned char _identifier);

private:
#pragma warning( disable : 4251 )
	NetEvent m_onConnectedToServer;
	NetEvent m_onDisconnectedFromServer;
	NetEvent m_onFailedToConnect;
	NetEvent m_onPlayerConnected;
	NetEvent m_onPlayerDisconnected;

	std::string m_ipAddress;
	std::string m_password;
	unsigned int m_port, m_clientPort;
	bool m_threadAlive;

	std::queue<PacketHandler::Packet> m_packets;
	std::mutex m_packetLock;


	RakNet::RakPeerInterface *m_client;

	std::thread m_thread;

#pragma warning( default : 4251 )

};

#endif
