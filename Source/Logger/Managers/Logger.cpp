#include <ctime>
#include <iostream>
#include "Logger.h"
#include "FileSystem/Directory.h"
#include "FileSystem/File.h"

Logger& Logger::GetInstance()
{
	static Logger* instance = new Logger();
	return *instance;
}

#pragma region Constructor/Destructor
Logger::Logger()
{
	m_logGroups = std::map<unsigned int, LogGroup*>();
	m_groupIdCounter = 0;
	m_messageIndex = 0;
	m_filterMask = 0;
	m_longestGroupName = 0;
	m_filterMask = ~m_filterMask;
	m_dumpLogs = new std::vector<LogEntry>();
	m_logMutex = SDL_CreateMutex();
	m_path = new std::string("");

	//CreateFile();


}

Logger::~Logger()
{
	for (auto logKey : m_logGroups)
	{
		delete(logKey.second->Messages);
		delete(logKey.second);
	}
	m_logGroups.clear();

	delete(m_path);
	delete(m_dumpLogs);
}
#pragma endregion



unsigned int Logger::AddGroup(const std::string& _groupName, bool _addToOutput)
{
	SDL_LockMutex(m_logMutex);
	/*	Create new group id	*/
	unsigned int newGroupId = m_groupIdCounter++;
	LogGroup* newGroup = new LogGroup();
	newGroup->GroupId = newGroupId;
	newGroup->GroupName = std::string(_groupName);
	newGroup->Messages = new std::vector<LogEntry>();

	/*	Add group to map	*/
	m_logGroups[newGroupId] = newGroup;
	m_groupNameIndex[_groupName] = newGroupId;

	if (_groupName.size() > m_longestGroupName)
		m_longestGroupName = _groupName.size();

	/*	Remove the group from output filter	*/
	if (!_addToOutput)
		m_filterMask ^= ((unsigned int)1 << newGroupId);

	SDL_UnlockMutex(m_logMutex);
	Log(newGroupId, LogSeverity::Info, "Added to log groups!");
	return newGroupId;
}

void Logger::Log(unsigned int _groupIndex, LogSeverity _severity, const std::string& _message)
{
	SDL_LockMutex(m_logMutex);
	/*	Get log group	*/
	if (m_logGroups.find(_groupIndex) == m_logGroups.end())
	{
		SDL_UnlockMutex(m_logMutex);
		std::stringstream newMessage;
		newMessage << "Undefined group used! (" << (int)_groupIndex << ")";
		Log(0, LogSeverity::Error, newMessage.str());
		return;
	}

	LogGroup* tGroup = m_logGroups[_groupIndex];

	/*	Add group info to message	*/
	std::stringstream newMessage;
	newMessage << "[" << tGroup->GroupName << "]" << GetTabs(tGroup->GroupName);
	newMessage << SeverityToString(_severity) << ": ";
	newMessage << _message << "\r\n";

	/*	Create new log entry	*/
	LogEntry newEntry = LogEntry();
	newEntry.Severity = _severity;
	newEntry.Message = newMessage.str();

	/*	Add entry	*/
	tGroup->Messages->push_back(newEntry);
	AppendFile(newEntry);

	/*	If the group is active in filter print the message	*/
	if (m_filterMask & ((unsigned int)1 << _groupIndex))
		SDL_Log(newMessage.str().c_str());

	SDL_UnlockMutex(m_logMutex);
}

void Logger::Log(const std::string& _groupName, LogSeverity _severity, const std::string& _message)
{
	SDL_LockMutex(m_logMutex);
	/*	Get log group	*/
	if (m_groupNameIndex.find(_groupName) == m_groupNameIndex.end())
	{
		SDL_UnlockMutex(m_logMutex);
		std::stringstream newMessage;
		newMessage << "Undefined group used! (" << _groupName << ")";
		Log(0, LogSeverity::Error, newMessage.str());
		return;
	}
	LogGroup* tGroup = m_logGroups[m_groupNameIndex[_groupName]];

	/*	Add group info to message	*/
	std::stringstream newMessage;
	newMessage << "[" << tGroup->GroupName << "]" << GetTabs(tGroup->GroupName);
	newMessage << SeverityToString(_severity) << ": ";
	newMessage << _message << "\r\n";

	/*	Create new log entry	*/
	LogEntry newEntry = LogEntry();
	newEntry.Severity = _severity;
	newEntry.Message = newMessage.str();

	/*	Add entry	*/
	tGroup->Messages->push_back(newEntry);
	AppendFile(newEntry);

	/*	If the group is active in filter print the message	*/
	if (m_filterMask & ((unsigned int)1 << m_groupNameIndex[_groupName]))
		SDL_Log(newMessage.str().c_str());

	SDL_UnlockMutex(m_logMutex);
}

void Logger::CreateFile()
{
#if !defined(__ANDROID__)

	/*	Create the file	*/
	time_t		tTime = time(0);
	struct tm	timeInfo;

#ifdef WIN32
	time(&tTime);
	localtime_s(&timeInfo, &tTime);
	_localtime64_s(&timeInfo, &tTime);
#else
	timeInfo = *(localtime(&tTime));
#endif

	std::ostringstream ss;
	ss << *m_path;
	ss << "debuglog_";
	ss << (1900+timeInfo.tm_year) << "-";
	ss << FixDateLength(timeInfo.tm_mon + 1) << "-";
	ss << FixDateLength(timeInfo.tm_mday) << "_";
	ss << FixDateLength(timeInfo.tm_hour); //<< ".";
	//ss << FixDateLength(timeInfo.tm_min) << ".";
	//ss << FixDateLength(timeInfo.tm_sec);
	ss << ".txt";
    
	m_logFileName = ss.str();


    std::stringstream ss2;
	ss2 << *m_path;
   // ss2 << "data";
    


	FileSystem::Directory::Create(ss2.str());

	FileSystem::File::Create(m_logFileName);


    
#endif


	m_loggerGroupId = AddGroup("Logger", true);
}

void Logger::AppendFile(LogEntry& _logEntry)
{
#if !defined(__ANDROID__)

	SDL_RWops* tFile;
	FileSystem::File::Append(m_logFileName, &tFile);
	FileSystem::File::Write(tFile, _logEntry.Message);
	FileSystem::File::Close(tFile);
	
	///*	Open the file	*/
	//char* end;
	//SDL_RWops* tFile = SDL_RWFromFile(m_logFileName.c_str(), "a");
	//SDL_RWwrite(tFile, _logEntry.Message.c_str(), 1, _logEntry.Message.size());
	//SDL_RWclose(tFile);

#endif
}

void Logger::ChangeFilterFor(unsigned int _groupIndex, bool _printInfo)
{
	SDL_LockMutex(m_logMutex);
	if (_printInfo)
		m_filterMask |= ((unsigned int)1 << _groupIndex);
	else
		m_filterMask &= ~((unsigned int)1 << _groupIndex); 
	SDL_UnlockMutex(m_logMutex);
}

void Logger::SetPath(std::string _path)
{
	*m_path = _path;
	CreateFile();
}

std::string Logger::SeverityToString(LogSeverity _severity)
{
	switch (_severity)
	{
	case LogSeverity::Error:
		return "Error";
		break;

	case LogSeverity::Info:
		return "Info";
		break;

	case LogSeverity::Warning:
		return "Warning";
		break;

	default:
		std::stringstream newMessage;
		newMessage << "Undefined severity used! (" << (int)_severity << ")";
		Log(m_loggerGroupId, LogSeverity::Error, newMessage.str());

		return "???";
	}
}

std::string Logger::GetTabs(std::string& _groupName)
{
	std::string strTabs = "\t";

	if (_groupName.size() != m_longestGroupName)
	{
		int nTabsA = (_groupName.size()+2) / 4;
		int nTabsB = (m_longestGroupName+2) / 4;

		for (int n = 1; n <= nTabsB - nTabsA; ++n)
			strTabs.append("\t");
	}

	return strTabs;
}


void Logger::Trim(std::string& _str)
{
	TrimStart(_str);
	TrimEnd(_str);
}

void Logger::TrimStart(std::string& _str)
{
	unsigned int charIndex = _str.size() - 1;
	for (int i = 0; i < _str.size() - 1; ++i)
		if (_str[i] != '\t' && _str[i] != ' ')
		{
			charIndex = i;
			break;
		}

	_str = _str.substr(charIndex, _str.size());
}

void Logger::TrimEnd(std::string& _str)
{
	unsigned int charIndex = 0;
	for (int i = _str.size() - 1; i >= 0; --i)
		if (_str[i] != '\t' && _str[i] != ' ')
		{
			charIndex = i;
			break;
		}

	_str = _str.substr(0, charIndex + 1);
}

std::string Logger::FixDateLength(unsigned int _dateValue)
{
	std::ostringstream ss;

	if (_dateValue < 10)
		ss << "0" << _dateValue;
	else
		ss << _dateValue;

	return ss.str();
}