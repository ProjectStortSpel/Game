#include "SystemWorkGroup.h"

using namespace ECSL;

SystemWorkGroup::SystemWorkGroup()
{
	m_systems = new std::vector<System*>();
}

SystemWorkGroup::SystemWorkGroup(System* _system)
{
	m_systems = new std::vector<System*>();
	m_systems->push_back(_system);
}

SystemWorkGroup::~SystemWorkGroup()
{
	if (m_systems)
	{
		for (int i = m_systems->size() - 1; i >= 0; --i)
		{
			delete m_systems->back();
			m_systems->pop_back();
		}

		delete(m_systems);
	}
}