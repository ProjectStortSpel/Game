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



class DECLSPEC Logger
{
public:
	static Logger& GetInstance();
	~Logger();

	unsigned int AddGroup(const std::string& _groupName, bool _addToOutput = true);
	void Log(unsigned int _groupIndex, LogSeverity _severity, const std::string& _message);
	void Log(const std::string& _groupName, LogSeverity _severity, const std::string& _message);
	void ChangeFilterFor(unsigned int _groupIndex, bool _printInfo);
	void SetPath(std::string _path);

private:
	Logger();

	std::string FixDateLength(unsigned int _dateValue);
	std::string SeverityToString(LogSeverity _severity);
	std::string GetTabs(std::string& _groupName);
	void CreateFile();
	void AppendFile(LogEntry& _logEntry);

	void Trim(std::string& _str);
	void TrimStart(std::string& _str);
	void TrimEnd(std::string& _str);

	unsigned int m_filterMask;
	unsigned int m_groupIdCounter;
	unsigned int m_messageIndex;
	unsigned int m_loggerGroupId;

	unsigned short m_longestGroupName;
	std::string	m_logFileName;

	SDL_mutex*	m_logMutex;

	std::map<unsigned int, LogGroup*> m_logGroups;
	std::map<std::string, unsigned int> m_groupNameIndex;
	std::vector<LogEntry>* m_dumpLogs;

	std::string* m_path;

};
#endif