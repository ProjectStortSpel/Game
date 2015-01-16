#ifndef REMOTE_CONSOLE
#define REMOTE_CONSOLE

#include <string>
#include <vector>
#include "Network/PacketHandler.h"
#include "Console/Console.h"

class RemoteConsole
{

private:
	std::string m_passwordServer;
	std::string m_passwordClient;

	void ExecuteCommandClient(std::string _command, std::vector<Console::Argument>* _vec);

	void SetServerPasswordConsole(std::string _command, std::vector<Console::Argument>* _vec);
	void SetClientPasswordConsole(std::string _command, std::vector<Console::Argument>* _vec);

	void UpdateCommands();

public:

	RemoteConsole();
	~RemoteConsole();

	void SetServerPassword(std::string _password);
	const char* GetServerPassword();
	bool CheckServerPassword(std::string _password);

	void SetClientPassword(std::string _password);
	const char* GetClientPassword();
	bool CheckClientPassword(std::string _password);

	void ExecuteCommand(Network::PacketHandler* _ph, uint64_t _id, Network::NetConnection _nc);
};

#endif