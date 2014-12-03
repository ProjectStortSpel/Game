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
		for (int i = (unsigned int)m_systems->size() - 1; i >= 0; --i)
		{
			delete m_systems->back();
			m_systems->pop_back();
		}

		delete(m_systems);
	}
}

void SystemWorkGroup::Update(float _dt)
{
	for (int n = 0; n < m_systems->size(); ++n)
		m_systems->at(n)->Update(_dt);
}