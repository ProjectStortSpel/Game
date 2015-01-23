#include "Logger.h"

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

	m_logMutex = SDL_CreateMutex();

	m_loggerGroupId = AddGroup("Logger", true);
}

Logger::~Logger()
{
	for (auto logKey : m_logGroups)
	{
		delete(logKey.second->Messages);
		delete(logKey.second);
	}
	m_logGroups.size();
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

	if (_groupName.size() > m_longestGroupName)
	{

		m_longestGroupName = _groupName.size();
	}

	/*	Remove the group from output filter	*/
	if (!_addToOutput)
		m_filterMask ^= ((unsigned int)1 << newGroupId);

	Log(newGroupId, LogSeverity::Info, "Added to log groups!");
	SDL_UnlockMutex(m_logMutex);
	return newGroupId;
}

void Logger::Log(unsigned int _groupIndex, LogSeverity _severity, const std::string& _message)
{
	SDL_LockMutex(m_logMutex);
	/*	Get log group	*/
	LogGroup* tGroup = m_logGroups[_groupIndex];

	/*	Add group info to message	*/
	std::stringstream newMessage;
	newMessage << "[" << tGroup->GroupName << "]\t";
	newMessage << SeverityToString(_severity) << ": ";
	newMessage << _message << "\n";

	/*	Create new log entry	*/
	LogEntry newEntry = LogEntry();
	newEntry.Severity = _severity;
	newEntry.Message = newMessage.str();

	/*	Add entry	*/
	tGroup->Messages->push_back(newEntry);

	/*	If the group is active in filter print the message	*/
	if (m_filterMask & ((unsigned int)1 << _groupIndex))
		printf(newMessage.str().c_str());

	SDL_UnlockMutex(m_logMutex);
}

void Logger::DumpLog()
{
	/*		*/
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



void Logger::Trim(std::string& _str)
{

}

void Logger::TrimStart(std::string& _str)
{

}

void Logger::TrimEnd(std::string& _str)
{

}