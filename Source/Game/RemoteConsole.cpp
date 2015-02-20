#include "Game/RemoteConsole.h"
#include "Console/Console.h"
#include "Network/NetworkInstance.h"
#include <sstream>

RemoteConsole::RemoteConsole()
{
	m_passwordServer = "";
	m_passwordClient = "";

	UpdateCommands();
}

RemoteConsole::~RemoteConsole()
{

}

void RemoteConsole::UpdateCommands()
{
	//Console::ConsoleManager::GetInstance().RemoveCommand("password");
	//Console::ConsoleManager::GetInstance().RemoveCommand("rcon");
	//Console::ConsoleManager::GetInstance().RemoveCommand("rcon_password");

	//if (NetworkInstance::isServer())
	//{
	//	Console::ConsoleManager::GetInstance().AddCommand("password", std::bind(&RemoteConsole::SetServerPasswordConsole, this, std::placeholders::_1, std::placeholders::_2));

	//	Network::NetMessageHook hook = std::bind(&RemoteConsole::ExecuteCommand, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	//	NetworkInstance::GetServer()->AddNetworkHook("rcon", hook);
	//}
	//if (NetworkInstance::isClient())
	//{
	//	Console::ConsoleManager::GetInstance().AddCommand("rcon_password", std::bind(&RemoteConsole::SetClientPasswordConsole, this, std::placeholders::_1, std::placeholders::_2));
	//	Console::ConsoleManager::GetInstance().AddCommand("rcon", std::bind(&RemoteConsole::ExecuteCommandClient, this, std::placeholders::_1, std::placeholders::_2));
	//}

	Console::ConsoleManager::GetInstance().AddCommand("password", std::bind(&RemoteConsole::SetServerPasswordConsole, this, std::placeholders::_1, std::placeholders::_2));

	Network::NetMessageHook hook = std::bind(&RemoteConsole::ExecuteCommand, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	NetworkInstance::GetServer()->AddNetworkHook("rcon", hook);

	Console::ConsoleManager::GetInstance().AddCommand("rcon_password", std::bind(&RemoteConsole::SetClientPasswordConsole, this, std::placeholders::_1, std::placeholders::_2));
	Console::ConsoleManager::GetInstance().AddCommand("rcon", std::bind(&RemoteConsole::ExecuteCommandClient, this, std::placeholders::_1, std::placeholders::_2));
}

void RemoteConsole::ExecuteCommandClient(std::string _command, std::vector<Console::Argument>* _vec)
{
	if (_vec->size() > 0 && NetworkInstance::isClient())
	{
		std::stringstream ss;
		
		if (_vec->at(0).ArgType == Console::ArgumentType::Text)
			ss << _vec->at(0).Text;
		else if (_vec->at(0).ArgType == Console::ArgumentType::Number)
			ss << _vec->at(0).Number;

		for (unsigned int i = 1; i < _vec->size(); ++i)
		{
			ss << ' ';
			ss << _vec->at(i).GetString();			
		}

		Network::PacketHandler* _ph = NetworkInstance::GetClient()->GetPacketHandler();
		uint64_t id = _ph->StartPack("rcon");
		_ph->WriteString(id, m_passwordClient.c_str());
		_ph->WriteString(id, ss.str().c_str());
		Network::Packet* p = _ph->EndPack(id);
		NetworkInstance::GetClient()->Send(p);

	}
	else
	{

	}
}

void RemoteConsole::SetServerPasswordConsole(std::string _command, std::vector<Console::Argument>* _vec)
{
	if (_vec->size() == 1)
	{
		if (_vec->at(0).ArgType == Console::ArgumentType::Text)
		{
			m_passwordServer = _vec->at(0).Text;
		}
	}
	else
	{

	}
}

void RemoteConsole::SetClientPasswordConsole(std::string _command, std::vector<Console::Argument>* _vec)
{
	if (_vec->size() == 1)
	{
		if (_vec->at(0).ArgType == Console::ArgumentType::Text)
		{
			m_passwordClient = _vec->at(0).Text;
		}
	}
	else
	{

	}
}

void RemoteConsole::ExecuteCommand(Network::PacketHandler* _ph, uint64_t _id, Network::NetConnection _nc)
{
	if (NetworkInstance::isServer())
	{
		if (CheckServerPassword(_ph->ReadString(_id)))
		{
			std::string command = _ph->ReadString(_id);
			Console::ConsoleManager::GetInstance().AddToCommandQueue(command.c_str());
		}
		else
		{
			printf("Invalid rcon password\n");
		}
	}	
}


void RemoteConsole::SetServerPassword(std::string _password)
{ 
	m_passwordServer = _password; 
}

const char* RemoteConsole::GetServerPassword()
{ 
	return m_passwordServer.c_str();
}

bool RemoteConsole::CheckServerPassword(std::string _password)
{ 
	return m_passwordServer == _password || m_passwordServer == ""; 
}

void RemoteConsole::SetClientPassword(std::string _password)
{
	m_passwordClient = _password;
}

const char* RemoteConsole::GetClientPassword()
{ 
	return m_passwordClient.c_str();
}

bool RemoteConsole::CheckClientPassword(std::string _password)
{ 
	return m_passwordClient == _password || m_passwordClient == "";
}