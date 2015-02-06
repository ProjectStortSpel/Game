#include "SystemActivationManager.h"

using namespace ECSL;

SystemActivationManager::SystemActivationManager()
:	m_systemsToActivate(new std::vector<unsigned int>()),
	m_systemsToDeactivate(new std::vector<unsigned int>())
{
	m_systemsToActivateMutex = SDL_CreateMutex();
	m_systemsToDeactivateMutex = SDL_CreateMutex();
}

SystemActivationManager::~SystemActivationManager()
{
	delete(m_systemsToActivate);
	delete(m_systemsToDeactivate);
	SDL_DestroyMutex(m_systemsToActivateMutex);
	SDL_DestroyMutex(m_systemsToDeactivateMutex);
}

void SystemActivationManager::ActivateSystem(unsigned int _systemId)
{
	SDL_LockMutex(m_systemsToActivateMutex);
	ContainerHelper::AddUniqueElement<unsigned int>(_systemId, *m_systemsToActivate);
	SDL_UnlockMutex(m_systemsToActivateMutex);
}

void SystemActivationManager::DeactivateSystem(unsigned int _systemId)
{
	SDL_LockMutex(m_systemsToDeactivateMutex);
	ContainerHelper::AddUniqueElement<unsigned int>(_systemId, *m_systemsToDeactivate);
	SDL_UnlockMutex(m_systemsToDeactivateMutex);
}

std::vector<unsigned int>& SystemActivationManager::PullSystemsToActivate()
{
	std::vector<unsigned int>* systemsToActivate = m_systemsToActivate;
	m_systemsToActivate = new std::vector<unsigned int>();
	return *systemsToActivate;
}

std::vector<unsigned int>& SystemActivationManager::PullSystemsToDeactivate()
{
	std::vector<unsigned int>* systemsToDeactivate = m_systemsToDeactivate;
	m_systemsToDeactivate = new std::vector<unsigned int>();
	return *systemsToDeactivate;
}