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

void SystemWorkGroup::AddSystem(System* _system)
{
	/* TODO: Add check so same system doesn't get added */
	m_systems->push_back(_system);
}

void SystemWorkGroup::Update(float _dt)
{
	unsigned int beforeExecution, afterExecution;
	for (int n = 0; n < m_systems->size(); ++n)
	{
		beforeExecution = SDL_GetTicks();
		afterExecution = 0;
		m_systems->at(n)->Update(_dt);
		afterExecution = SDL_GetTicks() - beforeExecution;

		if (afterExecution != 0)
			int a = 2;
	}
}