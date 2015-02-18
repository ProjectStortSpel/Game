#ifndef SYSTEMACTIVATIONMANAGER_H
#define SYSTEMACTIVATIONMANAGER_H

#include <SDL/SDL.h>
#include <vector>
#include "ECSL/Framework/Common/ContainerHelper.h"

namespace ECSL
{
	class SystemActivationManager
	{
	public:
		SystemActivationManager();
		~SystemActivationManager();

		void ActivateSystem(unsigned int _systemId);
		void DeactivateSystem(unsigned int _systemId);

		std::vector<unsigned int>* PullSystemsToActivate();
		std::vector<unsigned int>* PullSystemsToDeactivate();

	private:
		SDL_mutex* m_systemsToActivateMutex;
		SDL_mutex* m_systemsToDeactivateMutex;
		std::vector<unsigned int>* m_systemsToActivate;
		std::vector<unsigned int>* m_systemsToDeactivate;
	};
}

#endif