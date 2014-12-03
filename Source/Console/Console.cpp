#include "Console/Console.h"

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
	std::vector<Argument> _vec;
	ParseArgs(_command, &_vec);


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

