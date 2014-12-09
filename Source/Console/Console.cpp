#include "Console/Console.h"
#include <sstream>

using namespace Console;

std::string Argument::GetString()
{
	std::stringstream ss;

	if (ArgType == ArgumentType::Text)
	{
		ss << "\"";
		ss << Text;
		ss << "\"";
	}

	else if (ArgType == ArgumentType::Number)
	{
		ss << Number;
	}
	return ss.str();
}

ConsoleManager::ConsoleManager()
{
}

ConsoleManager::~ConsoleManager()
{
}

ConsoleManager& ConsoleManager::GetInstance()
{
	static ConsoleManager* instance = new ConsoleManager();
	return *instance;
}

bool ConsoleManager::ParseArgs(char* _args, std::vector<Argument>* _vector)
{
	_vector->clear();

	//char* args = new char[strlen(_args) + 1];
	//memcpy(args, _args, strlen(_args) + 1);

	//char* args = _args;


	size_t len = strlen(_args);


	char* space = strchr(_args, ' ');
	char* fnutt = strchr(_args, '\"');

	char* prev = _args;
	char* current = _args;

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
	//delete args;
	if (inFnutt)
	{
		_vector->clear();

		if (CONSOLE_DEBUG)
			printf("Invalid arguments!\n");
		return false;
	}
	return true;
}

void ConsoleManager::ExecuteCommand(const char* _command)
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
		printf("Command \"%s\" not bound.\n", command);
	}

	delete command;	
}
void ConsoleManager::AddCommand(const char* _name, ConsoleHook _hook)
{
	char* spacePointer = strchr((char*)_name, ' ');

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

void ConsoleManager::RemoveCommand(const char* _name)
{
	m_consoleHooks.erase(_name);
}

void ConsoleManager::ClearCommands()
{
	m_consoleHooks.clear();
}
