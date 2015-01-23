#ifndef LOGGER_H
#define LOGGER_H

#include <SDL/SDL.h>
#include <sstream> 
#include <string>
#include <vector>
#include <map>

enum LogSeverity
{
	Info,
	Warning,
	Error
};

struct LogEntry
{
	LogSeverity Severity;
	std::string	Message;
};

struct LogGroup
{
	unsigned int GroupId;
	std::string GroupName;
	std::vector<LogEntry>* Messages;
};



class Logger
{
public:
	static Logger& GetInstance();
	~Logger();

	unsigned int AddGroup(const std::string& _groupName, bool _addToOutput = true);
	void Log(unsigned int _groupIndex, LogSeverity _severity, const std::string& _message);
	void ChangeFilterFor(unsigned int _groupIndex, bool _printInfo);

	void DumpLog();

private:
	Logger();

	std::string SeverityToString(LogSeverity _severity);

	void Trim(std::string& _str);
	void TrimStart(std::string& _str);
	void TrimEnd(std::string& _str);

	unsigned int m_filterMask;
	unsigned int m_groupIdCounter;
	unsigned int m_messageIndex;
	unsigned int m_loggerGroupId;

	unsigned short m_longestGroupName;

	SDL_mutex*	m_logMutex;

	std::map<unsigned int, LogGroup*> m_logGroups;

};
#endif