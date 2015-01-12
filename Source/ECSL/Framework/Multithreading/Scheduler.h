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

	struct OnEntityAddedData
	{
		System* System;
		RuntimeInfo RuntimeInfo;
		std::vector<std::vector<unsigned int>*>* EntitiesToAddToSystems;
	};

	struct OnEntityRemovedData
	{
		System* System;
		RuntimeInfo RuntimeInfo;
		std::vector<std::vector<unsigned int>*>* EntitiesToRemoveFromSystems;
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
		std::vector<std::vector<unsigned int>*>* EntitiesToAddToSystems;
		std::vector<std::vector<unsigned int>*>* EntitiesToRemoveFromSystems;
	};

	class DECLSPEC Scheduler
	{
	public:
		Scheduler(DataManager* _dataManager, SystemManager* _systemManager);
		~Scheduler();

		void UpdateDt(float _dt);
		MPL::TaskId ScheduleUpdateSystems();
		MPL::TaskId ScheduleUpdateEntityTable(MPL::TaskId _dependency);
		MPL::TaskId ScheduleUpdateSystemEntityLists(MPL::TaskId _dependency);
		MPL::TaskId ScheduleOnEntityAddedTasks(MPL::TaskId _dependency);
		MPL::TaskId ScheduleOnEntityRemovedTasks(MPL::TaskId _dependency);
		MPL::TaskId ScheduleClearDeadEntities(MPL::TaskId _dependency);

		void AddUpdateSystemsTasks();
		void AddUpdateSystemEntityListsTasks();
		void AddOnEntityAddedTasks();
		void AddOnEntityRemovedTasks();
		void AddUpdateEntityTableTask();
		void AddClearDeadEntitiesTask();

	private:
		DataManager* m_dataManager;
		SystemManager* m_systemManager;
		MPL::TaskId m_onEntityTaskId;
		std::vector<std::vector<MPL::WorkItem*>*>* m_updateWorkItems;
		std::vector<MPL::WorkItem*>* m_updateEntityTableWorkItems;
		std::vector<MPL::WorkItem*>* m_updateSystemEntityListsWorkItems;
		std::vector<std::vector<MPL::WorkItem*>*>* m_onEntityAddedWorkItems;
		std::vector<std::vector<MPL::WorkItem*>*>* m_onEntityRemovedWorkItems;
		std::vector<MPL::WorkItem*>* m_clearDeadEntitiesWorkItems;
		std::vector<std::vector<unsigned int>*>* m_entitiesToAddToSystems;
		std::vector<std::vector<unsigned int>*>* m_entitiesToRemoveFromSystems;
	};

	static void SystemUpdate(void* _data);

	static void SystemOnEntityAdded(void* _data);

	static void SystemOnEntityRemoved(void* _data);

	static void DataManagerUpdateEntityTable(void* _data);

	static void DataManagerClearDeadEntities(void* _data);

	static void SystemManagerUpdateSystemEntityLists(void* _data);
};

#endif