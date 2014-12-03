#include "Console/Console.h"

using namespace Console;

void ConsoleManager::ParseArgs(char* _args, std::vector<Argument>* _vector)
{

}

void ConsoleManager::ExecuteCommand(char* _command)
{
	if (m_consoleHooks.find("hej") != m_consoleHooks.end())
	{
		std::vector<Argument> _vec;

		ParseArgs(_command, &_vec);

		m_consoleHooks[_command](&_vec);
	}

	else if (CONSOLE_DEBUG)
	{
		printf("Command \"%s\" not bound.", "hej");
	}
	
}
void ConsoleManager::AddCommand(char* _name, ConsoleHook _hook)
{
	//kolla så att _name inte innehåller ett mellanslag

	for (int i = 0; i < strlen(_name); ++i)
	{
		if (_name[i] == ' ')
		{
			if (CONSOLE_DEBUG)
			{
				printf("Command %s can not be added.", _name);
			}
			return;
		}
	}

	m_consoleHooks[_name] = _hook;

}

