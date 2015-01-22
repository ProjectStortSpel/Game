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
	m_filterMask = ~m_filterMask;
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
	/*	Create new group id	*/
	unsigned int newGroupId = m_groupIdCounter++;
	LogGroup* newGroup = new LogGroup();
	newGroup->GroupId = newGroupId;
	newGroup->GroupName = std::string(_groupName);
	newGroup->Messages = new std::vector<LogEntry>();

	/*	Add group to map	*/
	m_logGroups[newGroupId] = newGroup;

	/*	Remove the group from output filter	*/
	if (!_addToOutput)
		m_filterMask ^= ((unsigned int)1 << newGroupId);

	Log(newGroupId, LogSeverity::Info, "Added to log groups!");

	return newGroupId;
}

void Logger::Log(unsigned int _groupIndex, LogSeverity _severity, const std::string& _message)
{
	/*	Get log group	*/
	LogGroup* tGroup = m_logGroups[_groupIndex];

	/*	Add group info to message	*/
	std::stringstream newMessage;
	newMessage << "[#" << m_messageIndex++ << " " << tGroup->GroupName << "] ";
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
}

void Logger::DumpLog()
{
	/*		*/
}

void Logger::ChangeFilterFor(unsigned int _groupIndex, bool _printInfo)
{
	if (_printInfo)
		m_filterMask |= ((unsigned int)1 << _groupIndex);
	else
		m_filterMask &= ~((unsigned int)1 << _groupIndex);
}