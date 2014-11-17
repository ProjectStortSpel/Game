#ifndef PROJEKTSERVER_H
#define PROJEKTSERVER_H

#include <functional>
#include <queue>
#include <RakNet/RakPeerInterface.h>
#include <mutex>
#include <thread>
#include <SDL/SDL.h>


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

	const char* GetIp(void) { return m_ipAddress.c_str(); }
	const char* GetPassword(void) { return m_password.c_str(); }
	const int GetPort(void) { return m_port; }

	void SetNetPort(const int _port) { m_port = _port; }
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
	int m_port;

	std::queue<RakNet::Packet*> m_packets;
	std::mutex m_packetLock;

	RakNet::RakPeerInterface *m_server;

	std::thread m_thread;

};

#endif