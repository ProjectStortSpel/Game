#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "RuntimeInfo.h"
#include "ECSL/Framework/Components/DataManager.h"
#include "ECSL/Framework/Systems/SystemManager.h"
#include "ECSL/Framework/Systems/SystemWorkGroup.h"
#include "MPL/Framework/Tasks/Task.h"

namespace ECSL
{
	struct UpdateData
	{
		System* System;
		RuntimeInfo RuntimeInfo;
	};

	struct OnEntityData
	{
		System* System;
		RuntimeInfo RuntimeInfo;
		unsigned int EntityId;
	};

	struct DataManagerData
	{
		DataManager* DataManager;
		RuntimeInfo RuntimeInfo;
	};

	struct UpdateSystemEntityListsData
	{
		SystemManager* SystemManager;
		RuntimeInfo RuntimeInfo;
		std::vector<std::vector<System*>*>* EntityAddedRequests;
		std::vector<std::vector<System*>*>* EntityRemovedRequests;
	};

	class DECLSPEC Scheduler
	{
	public:
		Scheduler(DataManager* _dataManager, SystemManager* _systemManager);
		~Scheduler();

		void UpdateDt(float _dt);
		MPL::TaskId ScheduleUpdate();
		MPL::TaskId ScheduleUpdateEntityTable(MPL::TaskId _dependency);
		MPL::TaskId ScheduleClearDeadEntities(MPL::TaskId _dependency);

		void AddUpdateTasks(const std::vector<SystemWorkGroup*>& _systemGroups);
		void AddOnEntityAddedTask(const std::vector<System*>& _systems);
		void AddOnEntityRemovedTask(const std::vector<System*>& _systems);
		void AddUpdateEntityTableTask();
		void AddClearDeadEntitiesTask();
		void AddUpdateSystemEntityListsTasks();

	private:
		DataManager* m_dataManager;
		SystemManager* m_systemManager;
		std::vector<std::vector<MPL::WorkItem*>*>* m_updateWorkItems;
		std::vector<MPL::WorkItem*>* m_updateEntityComponentsWorkItems;
		std::vector<MPL::WorkItem*>* m_clearDeadEntitiesWorkItems;
		std::vector<std::vector<MPL::WorkItem*>*>* m_updateGroupEntityListsWorkItems;
		std::vector<std::vector<System*>*>* m_entityAddedRequests;
		std::vector<std::vector<System*>*>* m_entityRemovedRequests;
	};

	static void SystemUpdate(void* _data);

	static void SystemOnEntityAdded(void* _data);

	static void SystemOnEntityRemoved(void* _data);

	static void DataManagerUpdateEntityTable(void* _data);

	static void DataManagerClearDeadEntities(void* _data);

	static void SystemManagerUpdateSystemEntityLists(void* _data);
};

#endif