#include "SystemManager.h"

using namespace ECSL;

SystemManager::SystemManager(std::vector<SystemWorkGroup*>* _systemWorkGroups)
{
	m_systemWorkGroups = _systemWorkGroups;
}

SystemManager::~SystemManager()
{
	for (int n = m_systemWorkGroups->size() - 1; n >= 0; --n)
		delete m_systemWorkGroups->at(n);

	delete m_systemWorkGroups;
}

void SystemManager::InitializeSystems()
{

	/*	Initialize all system groups	*/
	for (unsigned int groupId = 0; groupId < m_systemWorkGroups->size(); ++groupId)
	{
		std::vector<System*>* systems = m_systemWorkGroups->at(groupId)->GetSystems();
		/*	Go through all systems in the group and initialize them	*/
		for (unsigned int systemId = 0; systemId < systems->size(); ++systemId)
		{
			systems->at(systemId)->Initialize();

			//TODO Bitmask
		}
	}


}