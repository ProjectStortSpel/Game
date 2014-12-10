#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <SDL/SDL.h>
#include <vector>
#include "ECSL/Framework/Components/DataManager.h"
#include "SystemWorkGroup.h"

namespace ECSL
{
	class DECLSPEC SystemManager
	{
	public:
		SystemManager(DataManager* _dataManager, std::vector<SystemWorkGroup*>* _systemWorkGroups);
		~SystemManager();

		void InitializeSystems();

		void Update(float _dt);

		void SystemEntitiesUpdate();

	private:
		int m_nextSystemId;
		DataManager* m_dataManager;
		std::vector<SystemWorkGroup*>* m_systemWorkGroups;

		void AddEntityToSystem(unsigned int _entityId, System* _system);
		void RemoveEntityFromSystem(unsigned int _entityId, System* _system);

		void GenerateComponentFilter(System* _system, FilterType _filterType);
		unsigned int GetSystemId(System* _system);
	};
}

#endif