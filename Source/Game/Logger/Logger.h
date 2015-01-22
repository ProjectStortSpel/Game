#ifndef LOGGER_H
#define LOGGER_H

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
	unsigned int m_filterMask;
	unsigned int m_groupIdCounter;
	unsigned int m_messageIndex;

	std::map<unsigned int, LogGroup*> m_logGroups;

};
#endif