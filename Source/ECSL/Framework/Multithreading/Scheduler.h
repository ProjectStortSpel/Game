#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "RuntimeInfo.h"
#include "ECSL/Framework/Components/DataManager.h"
#include "ECSL/Framework/Systems/Messaging/MessageManager.h"
#include "ECSL/Framework/Systems/SystemManager.h"
#include "ECSL/Framework/Systems/SystemWorkGroup.h"
#include "MPL/Framework/Tasks/Task.h"
#include "MPL/Managers/TaskManager.h"

namespace ECSL
{
	class DECLSPEC Scheduler
	{
	public:
		Scheduler(DataManager* _dataManager, SystemManager* _systemManager, MessageManager* _messageManager);
		~Scheduler();

		void UpdateDt(float _dt);
		MPL::TaskId ScheduleUpdateSystems(MPL::TaskId _dependency);
		MPL::TaskId ScheduleUpdateEntityTable(MPL::TaskId _dependency);
		MPL::TaskId ScheduleUpdateSystemEntityLists(MPL::TaskId _dependency);
		MPL::TaskId ScheduleEntitiesAdded(MPL::TaskId _dependency);
		MPL::TaskId ScheduleEntitiesRemoved(MPL::TaskId _dependency);
		MPL::TaskId ScheduleSortMessages(MPL::TaskId _dependency);
		MPL::TaskId ScheduleMessagesRecieved(MPL::TaskId _dependency);
		MPL::TaskId ScheduleDeleteMessages(MPL::TaskId _dependency);
		MPL::TaskId ScheduleClearDeadEntities(MPL::TaskId _dependency);
		MPL::TaskId ScheduleRecycleEntities(MPL::TaskId _dependency);
		MPL::TaskId ScheduleClearChangeLists(MPL::TaskId _dependency);
		MPL::TaskId ScheduleClearLists(MPL::TaskId _dependency);

		void AddUpdateSystemsTasks();
		void AddUpdateEntityTableTask();
		void AddUpdateSystemEntityListsTasks();
		void AddEntitiesAddedTasks();
		void AddEntitiesRemovedTasks();
		void AddSortMessagesTask();
		void AddMessagesRecievedTasks();
		void AddDeleteMessagesTask();
		void AddClearDeadEntitiesTask();
		void AddRecycleEntityIdsTask();
		void AddClearChangeListsTask();
		void AddClearListsTask();

		void ClearLists(const RuntimeInfo& _runtimeInfo);

	private:
		DataManager* m_dataManager;
		SystemManager* m_systemManager;
		MessageManager* m_messageManager;
		MPL::TaskManager* m_taskManager;
		MPL::TaskId m_onEntityTaskId;
		std::vector<MPL::WorkItem*>* m_workItems;
		std::vector<std::vector<MPL::WorkItem*>*>* m_updateWorkItems;
		std::vector<MPL::WorkItem*>* m_updateEntityTableWorkItems;
		std::vector<MPL::WorkItem*>* m_updateSystemEntityListsWorkItems;
		std::vector<std::vector<MPL::WorkItem*>*>* m_entitiesAddedWorkItems;
		std::vector<std::vector<MPL::WorkItem*>*>* m_entitiesRemovedWorkItems;
		std::vector<MPL::WorkItem*>* m_sortMessagesWorkItems;
		std::vector<std::vector<MPL::WorkItem*>*>* m_messagesRecievedWorkItems;
		std::vector<MPL::WorkItem*>* m_deleteMessagesWorkItems;
		std::vector<MPL::WorkItem*>* m_clearDeadEntitiesWorkItems;
		std::vector<MPL::WorkItem*>* m_recycleEntityIdsWorkItems;
		std::vector<MPL::WorkItem*>* m_clearChangeListsWorkItems;
		std::vector<MPL::WorkItem*>* m_clearListsWorkItems;
		std::vector<std::vector<unsigned int>*>* m_entitiesToAddToSystems;
		std::vector<std::vector<unsigned int>*>* m_entitiesToRemoveFromSystems;
	};

	static void SystemUpdate(void* _data);

	static void SystemEntitiesAdded(void* _data);

	static void SystemEntitiesRemoved(void* _data);

	static void SystemMessagesRecieved(void* _data);

	static void DataManagerUpdateEntityTable(void* _data);

	static void DataManagerClearDeadEntities(void* _data);

	static void DataManagerRecycleEntityIds(void* _data);

	static void DataManagerClearChangeLists(void* _data);

	static void SystemManagerUpdateSystemEntityLists(void* _data);

	static void MessageManagerSortMessages(void* _data);

	static void MessageManagerDeleteMessages(void* _data);

	static void SchedulerClearLists(void* _data);

	struct RuntimeData
	{
		RuntimeInfo RuntimeInfo;
	};

	struct UpdateData
	{
		RuntimeInfo RuntimeInfo;
		System* System;
	};

	struct UpdateEntityTableData
	{
		RuntimeInfo RuntimeInfo;
		DataManager* DataManager;
	};

	struct UpdateSystemEntityListsData
	{
		RuntimeInfo RuntimeInfo;
		SystemManager* SystemManager;
		std::vector<std::vector<unsigned int>*>* EntitiesToAddToSystems;
		std::vector<std::vector<unsigned int>*>* EntitiesToRemoveFromSystems;
	};

	struct EntitiesAddedData
	{
		RuntimeInfo RuntimeInfo;
		System* System;
		std::vector<std::vector<unsigned int>*>* EntitiesToAddToSystems;
	};

	struct EntitiesRemovedData
	{
		RuntimeInfo RuntimeInfo;
		System* System;
		std::vector<std::vector<unsigned int>*>* EntitiesToRemoveFromSystems;
	};

	struct SortMessagesData
	{
		RuntimeInfo RuntimeInfo;
		MessageManager* MessageManager;
	};

	struct MessagesRecievedData
	{
		RuntimeInfo RuntimeInfo;
		MessageManager* MessageManager;
		System* System;
	};

	struct DeleteMessagesData
	{
		RuntimeInfo RuntimeInfo;
		MessageManager* MessageManager;
	};

	struct ClearDeadEntitiesData
	{
		RuntimeInfo RuntimeInfo;
		DataManager* DataManager;
	};

	struct RecycleEntityIdsData
	{
		RuntimeInfo RuntimeInfo;
		DataManager* DataManager;
	};

	struct ClearChangeListsData
	{
		RuntimeInfo RuntimeInfo;
		DataManager* DataManager;
	};

	struct ClearListsData
	{
		RuntimeInfo RuntimeInfo;
		Scheduler* Scheduler;
	};
};

#endif