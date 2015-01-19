#include "SystemIdManager.h"

#include <assert.h>

using namespace ECSL;

SystemIdManager::SystemIdManager()
: m_nextSystemId(-1), m_systemStringIds(new std::unordered_map<std::string, unsigned int>)
{

}

SystemIdManager::~SystemIdManager()
{
	delete(m_systemStringIds);
}

unsigned int SystemIdManager::CreateSystemId(const std::string& _systemName)
{
	/* System with that name is already created */
	assert(m_systemStringIds->find(_systemName) == m_systemStringIds->end());

	/* System doesn't have a name */
	assert(_systemName != "");

	unsigned int id = ++m_nextSystemId;
	m_systemStringIds->insert(std::pair<std::string, unsigned int>(_systemName, id));
	return id;
}

unsigned int SystemIdManager::GetSystemId(const std::string& _systemName)
{
	return m_systemStringIds->find(_systemName)->second;
}