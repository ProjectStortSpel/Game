#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <SDL/SDL.h>
#include <vector>
#include "SystemIdManager.h"
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

		void CopyChangedEntities(const RuntimeInfo& _runtime);

		void UpdateGroupEntityLists(const RuntimeInfo& _runtime, unsigned int _groupIndex);

		const std::vector<SystemWorkGroup*>* GetSystemWorkGroups() { return m_systemWorkGroups; }

	private:
		SystemIdManager* m_systemIdManager;
		DataManager* m_dataManager;
		std::vector<SystemWorkGroup*>* m_systemWorkGroups;
		std::vector<unsigned int>* m_changedEntitiesCopy;

		void AddEntityToSystem(unsigned int _entityId, System* _system);
		void RemoveEntityFromSystem(unsigned int _entityId, System* _system);

		void GenerateComponentFilter(System* _system, FilterType _filterType);
		unsigned int GetSystemId(System* _system);
	};
}

#endif