#include "SystemIdManager.h"

using namespace ECSL;

SystemIdManager::SystemIdManager()
: m_nextSystemId(-1), m_stringSystemId(new std::unordered_map<std::string, unsigned int>)
{

}

SystemIdManager::~SystemIdManager()
{
	delete(m_stringSystemId);
}

unsigned int SystemIdManager::GetSystemId(const std::string& _systemName)
{
	auto it = m_stringSystemId->find(_systemName);
	if (it == m_stringSystemId->end())
	{
		unsigned int newId = ++m_nextSystemId;
		m_stringSystemId->insert(std::pair<std::string, unsigned int>(_systemName, newId));
		return newId;
	}
	else
		return it->second;
}