#include "Console/Console.h"

using namespace Console;

void ConsoleManager::ParseArgs(char* _args, std::vector<Argument>* _vector)
{

}

void ConsoleManager::ExecuteCommand(char* _command)
{

	std::vector<Argument> _vec;

	ParseArgs(_command, &_vec);

	m_consoleHooks[_command](&_vec);


}
void ConsoleManager::AddCommand(char* _name, ConsoleHook _hook)
{
	//kolla så att _name inte innehåller ett mellanslag

	m_consoleHooks[_name] = _hook;

}

