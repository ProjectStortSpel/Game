#include "Console/Console.h"

#include <cstring>

using namespace Console;

ConsoleManager::ConsoleManager()
{
}

ConsoleManager::~ConsoleManager()
{
}

void ConsoleManager::ParseArgs(char* _args, std::vector<Argument>* _vector)
{
	_vector->clear();

	std::string temp = _args;
	std::string substring;

	char* resultChar;
	float resultInt;

	size_t pos = temp.find_first_of(' ');
	while (pos != std::string::npos)
	{
		substring = temp.substr(0, pos);
		temp = temp.substr(pos + 1);

		resultInt = strtof(substring.c_str(), &resultChar);
		
		if (*resultChar == '\0')
			_vector->push_back(Argument(resultInt));
		else
			_vector->push_back(Argument(resultChar));

		pos = temp.find_first_of(' ');
	}

}

void ConsoleManager::ExecuteCommand(char* _command)
{
	char* command = new char[strlen(_command)+1];
	memcpy(command, _command, strlen(_command) + 1);

	char* args = strchr(command, ' ');

	if (args != 0)
	{
		*args = '\0';
		++args;
	}
	else
	{
		args = command + strlen(command);
	}

	if (m_consoleHooks.find(command) != m_consoleHooks.end())
	{
		std::vector<Argument> _vec;

		ParseArgs(args, &_vec);

		m_consoleHooks[_command](&_vec);
	}

	else if (CONSOLE_DEBUG)
	{
		printf("Command \"%s\" not bound.\n", "hej");
	}

	delete command;	
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
				printf("Command %s can not be added.\n", _name);
			}
			return;
		}
	}

	m_consoleHooks[_name] = _hook;

}

