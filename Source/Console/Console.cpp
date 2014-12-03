#include "Console/Console.h"

using namespace Console;

ConsoleManager::ConsoleManager()
{
}

ConsoleManager::~ConsoleManager()
{
}

bool ConsoleManager::ParseArgs(char* _args, std::vector<Argument>* _vector)
{
	_vector->clear();

	char* args = new char[strlen(_args) + 1];
	memcpy(args, _args, strlen(_args) + 1);


	size_t len = strlen(args);


	char* space = strchr(args, ' ');
	char* fnutt = strchr(args, '\"');

	char* prev = args;
	char* current = args;

	bool inFnutt = false;
	bool lastrun = true;
	while (space || fnutt || lastrun)
	{
		bool nextInFnutt = false;
		if (!space && !fnutt)
			lastrun = false;
		else
		{
			
			if (inFnutt)
			{
				if (fnutt)
				{
					current = fnutt;
					nextInFnutt = false;
				}
				else
				{
					break;
				}
			}
			else
			{
				if (!space)
				{
					current = fnutt;
					nextInFnutt = true;
				}
				else if (!fnutt || space < fnutt)
				{
					current = space;
				}
				else
				{
					current = fnutt;
					nextInFnutt = true;
				}
			}
			*current = '\0';
			++current;
		}

		if (strlen(prev) > 0)
		{
			char* resultChar;
			float resultFloat;

			resultFloat = strtof(prev, &resultChar);
			if (*resultChar == '\0' && !inFnutt)
				_vector->push_back(Argument(resultFloat));
			else
				_vector->push_back(Argument(prev));

			//_vector->push_back(Argument(prev));
		}

		prev = current;

		space = strchr(current, ' ');
		fnutt = strchr(current, '\"');
		inFnutt = nextInFnutt;

	}
	delete args;
	if (inFnutt)
	{
		_vector->clear();

		if (CONSOLE_DEBUG)
			printf("Invalid arguments!\n");
		return false;
	}
	return true;
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
		std::vector<Argument> vec;

		ParseArgs(args, &vec);

		m_consoleHooks[command](&vec);
	}

	else if (CONSOLE_DEBUG)
	{
		printf("Command \"%s\" not bound.\n", "hej");
	}

	delete command;	
}
void ConsoleManager::AddCommand(char* _name, ConsoleHook _hook)
{
	char* spacePointer = strchr(_name, ' ');

	if (spacePointer)
	{
		if (CONSOLE_DEBUG)
		{
			printf("Command %s can not be added.\n", _name);
		}
	}
	else
	{
		m_consoleHooks[_name] = _hook;
	}

}

