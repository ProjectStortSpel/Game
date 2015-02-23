#include "Console/Console.h"
#include <string>
#include <regex>
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
	: m_match(), m_historyCounter(-1), m_start(0), m_end(0), m_open(false)
{
}

ConsoleManager::~ConsoleManager()
{
}

ConsoleManager& ConsoleManager::GetInstance()
{
	static ConsoleManager instance;
	return instance;
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

void ConsoleManager::ExecuteCommandQueue()
{
	while (!m_commandQueue.empty())
	{
		// split commands if it contains ;
		std::string s = m_commandQueue.front();
		std::string delimiter = ";";

		size_t pos = 0;
		std::string token;
		while ((pos = s.find(delimiter)) != std::string::npos) {
			token = s.substr(0, pos);
			ExecuteCommand(token.c_str());
			s.erase(0, pos + delimiter.length());
		}
		ExecuteCommand(s.c_str());

		m_commandQueue.pop();
	}
}

void ConsoleManager::AddToCommandQueue(const char* _command)
{
	m_commandQueue.push(_command);
}

void ConsoleManager::AddToHistory(const char* _command)
{
	AddMessage(_command);
	//m_history.push_back(_command);
}

void ConsoleManager::ExecuteCommand(const char* _command)
{
	m_historyCounter = -1;

	char* command = new char[strlen(_command) + 1];
	memcpy(command, _command, strlen(_command) + 1);

	/*for (int n = 0; n < strlen(_command); ++n)
		command[n] = tolower(command[n]);*/

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

	for (int n = 0; n < strlen(command); ++n)
		command[n] = tolower(command[n]);

	AddMessage(_command);
	

	//if (m_history.size() > 8)
	//	m_history.erase(m_history.begin());

	if (m_commandHistory.size() > 8)
		m_commandHistory.erase(m_commandHistory.begin());

	//m_history.push_back(_command);
	m_commandHistory.push_back(_command);

	if (m_consoleHooks.find(command) != m_consoleHooks.end())
	{
		std::vector<Argument> vec;

		ParseArgs(args, &vec);

		m_consoleHooks[command](command, &vec);
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
		char* name = new char[strlen(_name) + 1];
		memcpy(name, _name, strlen(_name) + 1);

		for (int n = 0; n < strlen(name); ++n)
			name[n] = tolower(name[n]);

		m_consoleHooks[name] = _hook;

		delete name;
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

void ConsoleManager::ClearHistory()
{
	m_history.clear();
	m_commandHistory.clear();

	m_match.clear();
	m_historyCounter = -1;
	m_start = 0;
	m_end = 0;
}

std::vector<std::string> ConsoleManager::GetHistory()
{
	std::vector<std::string>::const_iterator first = m_history.begin() + m_start;
	std::vector<std::string>::const_iterator last = m_history.begin() + m_end;

	return std::vector<std::string>(first, last);
}

void ConsoleManager::AddMessage(const char* _message)
{
	//if (m_history.size() > 8)
	//	m_history.erase(m_history.begin());
	m_history.push_back(_message);
	++m_end;
	m_start = m_history.size() > 29 ? m_start + 1 : m_start;
}

const char* ConsoleManager::GetFunctionMatch(const char* _command)
{
	m_match = "";

	if (*_command == '\0')
		return m_match.c_str();

	std::string cmd(_command);
	cmd = cmd.substr(0, cmd.find(' '));

	for (int n = 0; n < strlen(cmd.c_str()); ++n)
		cmd[n] = tolower(cmd[n]);



	char buffer[256];
#ifdef WIN32
	sprintf_s(buffer, "^(%s)(.*)?$", cmd.c_str());
#else
	sprintf(buffer, "^(%s)(.*)?", cmd.c_str());
#endif

	std::regex reg(buffer);

	for (auto it = m_consoleHooks.begin(); it != m_consoleHooks.end(); ++it)
		if (std::regex_match(it->first, reg))
		{
			m_match = it->first;
			break;
		}

	return m_match.c_str();
}

const char* ConsoleManager::GetPreviousHistory()
{
	int size = m_commandHistory.size() - 1;
	if (size < 0)
		return 0;

	m_historyCounter = m_historyCounter >= size ? 0 : m_historyCounter + 1;
	return m_commandHistory[size - m_historyCounter].c_str();

	return m_match.c_str();
}

const char* ConsoleManager::GetNextHistory()
{
	int size = m_commandHistory.size() - 1;
	if (size < 0)
		return 0;

	m_historyCounter = m_historyCounter <= 0 ? size : m_historyCounter - 1;

	return m_commandHistory[size - m_historyCounter].c_str();

}

void ConsoleManager::ScrollUp()
{
	m_start = m_start - 1 < 0 ? 0 : m_start - 1;
	m_end = m_start + 29 > m_history.size() ? m_history.size() : m_start + 29;

	//m_end = m_history.size() < 10 ? 
}

void ConsoleManager::ScrollDown()
{
	m_end = m_end + 1 > m_history.size() ? m_history.size() : m_end + 1;
	m_start = m_end - 29 < 0 ? 0 : m_end - 29;
}