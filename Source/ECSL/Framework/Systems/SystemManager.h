#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <SDL/SDL.h>
#include <vector>
#include "SystemWorkGroup.h"

namespace ECSL
{
	class DECLSPEC SystemManager
	{
	private:
		std::vector<SystemWorkGroup*>* m_systemWorkGroups;

	public:
		explicit SystemManager(std::vector<SystemWorkGroup*>* _systemWorkGroups);
		~SystemManager();

		void InitializeSystems();
		
		template<typename SystemType>
		void AddSystem();
	};

	template<typename SystemType>
	void SystemManager::AddSystem()
	{
		SystemType* system = new SystemType();
		m_systems->push_back(system);
	}
}

#endif