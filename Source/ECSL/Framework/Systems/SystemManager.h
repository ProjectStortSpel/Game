#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <SDL/SDL.h>
#include <vector>
#include "SystemWorkGroup.h"
#include "ECSL/Framework/Components/DataManager.h"
#include "ECSL/Framework/Multithreading/RuntimeInfo.h"

namespace ECSL
{
	class DECLSPEC SystemManager
	{
	public:
		SystemManager(DataManager* _dataManager, std::vector<SystemWorkGroup*>* _systemWorkGroups);
		~SystemManager();

		void InitializeSystems();
		void PostInitializeSystems();

		void UpdateSystemEntityLists(const RuntimeInfo& _runtime,
			std::vector<std::vector<unsigned int>*>& _entitiesToAddToSystems,
			std::vector<std::vector<unsigned int>*>& _entitiesToRemoveFromSystems);

		const std::vector<System*>* GetSystems() { return m_systems; }
		const std::vector<SystemWorkGroup*>* GetSystemWorkGroups() { return m_systemWorkGroups; }
		SystemIdManager* GetSystemIdManager() { return m_systemIdManager; }
		SystemActivationManager* GetSystemActivationManager() { return m_systemActivationManager; }

	private:
		SystemActivationManager* m_systemActivationManager;
		SystemIdManager* m_systemIdManager;
		DataManager* m_dataManager;
		std::vector<System*>* m_systems;
		std::vector<SystemWorkGroup*>* m_systemWorkGroups;

		void GenerateComponentFilter(System* _system, FilterType _filterType);
	};
}

#endif