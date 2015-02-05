#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "RuntimeInfo.h"
#include "ECSL/Framework/Components/DataManager.h"
#include "ECSL/Framework/Systems/Messaging/MessageManager.h"
#include "ECSL/Framework/Systems/SystemManager.h"
#include "ECSL/Framework/Systems/SystemWorkGroup.h"
#include "ECSL/Framework/Multithreading/ECSLWorkItem.h"
#include "MPL/Managers/TaskManager.h"

namespace ECSL
{
	class DECLSPEC Scheduler
	{
	public:
		Scheduler(DataManager* _dataManager, SystemManager* _systemManager, MessageManager* _messageManager);
		~Scheduler();

		void UpdateDt(float _dt);
		void UpdateWorkItemLists();

		MPL::TaskId ScheduleUpdateSystems(MPL::TaskId _dependency);
		MPL::TaskId ScheduleUpdateSystemEntityLists(MPL::TaskId _dependency);
		MPL::TaskId ScheduleEntitiesAdded(MPL::TaskId _dependency);
		MPL::TaskId ScheduleEntitiesRemoved(MPL::TaskId _dependency);
		MPL::TaskId ScheduleSortMessages(MPL::TaskId _dependency);
		MPL::TaskId ScheduleMessagesReceived(MPL::TaskId _dependency);
		MPL::TaskId ScheduleDeleteMessages(MPL::TaskId _dependency);
		MPL::TaskId ScheduleCopyCurrentLists(MPL::TaskId _dependency);
		MPL::TaskId ScheduleUpdateEntityTable(MPL::TaskId _dependency);
		MPL::TaskId ScheduleDeleteComponentData(MPL::TaskId _dependency);
		MPL::TaskId ScheduleRecycleEntities(MPL::TaskId _dependency);
		MPL::TaskId ScheduleClearCopiedLists(MPL::TaskId _dependency);
		MPL::TaskId ScheduleClearSystemEntityChangeLists(MPL::TaskId _dependency);

		void PerformUpdateSystems();
		void PerformUpdateSystemEntityLists();
		void PerformEntitiesAdded();
		void PerformEntitiesRemoved();
		void PerformSortMessages();
		void PerformMessagesReceived();
		void PerformDeleteMessages();
		void PerformCopyCurrentLists();
		void PerformUpdateEntityTable();
		void PerformDeleteComponentData();
		void PerformRecycleEntities();
		void PerformClearCopiedLists();
		void PerformClearSystemEntityChangeLists();

		/// System Tasks
		void AddUpdateSystemsTasks();
		void AddEntitiesAddedTasks();
		void AddEntitiesRemovedTasks();
		void AddMessagesReceivedTasks();

		/// DataManager Tasks
		void AddCopyCurrentListsTask();
		void AddUpdateEntityTableTask();
		void AddDeleteComponentDataTask();
		void AddRecycleEntityIdsTask();
		void AddClearCopiedListsTask();

		/// SystemManager Tasks
		void AddUpdateSystemEntityListsTasks();

		/// MessageManager Tasks
		void AddSortMessagesTask();
		void AddDeleteMessagesTask();

		/// Scheduler Tasks
		void AddClearSystemEntityChangeListsTask();


		void ClearSystemEntityChangeLists(const RuntimeInfo& _runtimeInfo);

	private:
		struct DeactivatedWorkItem
		{
			enum WorkItemType { Update, EntitiesAdded, EntitiesRemoved, MessagesReceived };
			ECSLWorkItem* workItem;
			WorkItemType type;
		};

		unsigned int m_currentGroupId;
		DataManager* m_dataManager;
		SystemManager* m_systemManager;
		MessageManager* m_messageManager;
		MPL::TaskManager* m_taskManager;
		std::vector<ECSLWorkItem*>* m_workItems;
		std::vector<std::vector<ECSLWorkItem*>*>* m_updateWorkItems;
		std::vector<ECSLWorkItem*>* m_updateSystemEntityListsWorkItems;
		std::vector<std::vector<ECSLWorkItem*>*>* m_entitiesAddedWorkItems;
		std::vector<std::vector<ECSLWorkItem*>*>* m_entitiesRemovedWorkItems;
		std::vector<ECSLWorkItem*>* m_sortMessagesWorkItems;
		std::vector<std::vector<ECSLWorkItem*>*>* m_messagesReceivedWorkItems;
		std::vector<ECSLWorkItem*>* m_deleteMessagesWorkItems;
		std::vector<ECSLWorkItem*>* m_copyCurrentListsWorkItems;
		std::vector<ECSLWorkItem*>* m_updateEntityTableWorkItems;
		std::vector<ECSLWorkItem*>* m_deleteComponentDataWorkItems;
		std::vector<ECSLWorkItem*>* m_recycleEntityIdsWorkItems;
		std::vector<ECSLWorkItem*>* m_clearCopiedListsWorkItems;
		std::vector<ECSLWorkItem*>* m_clearListsWorkItems;
		std::map<unsigned int, std::vector<DeactivatedWorkItem*>>* m_deactivatedWorkItems;
		std::vector<std::vector<unsigned int>*>* m_entitiesToAddToSystems;
		std::vector<std::vector<unsigned int>*>* m_entitiesToRemoveFromSystems;

		bool EraseSystemWorkItems(unsigned int _systemId, DeactivatedWorkItem::WorkItemType _workItemType, std::vector<std::vector<ECSLWorkItem*>*>* _workItemGroups);
	};

	static void SystemUpdate(void* _data);

	static void SystemEntitiesAdded(void* _data);

	static void SystemEntitiesRemoved(void* _data);

	static void SystemMessagesReceived(void* _data);

	static void DataManagerCopyCurrentLists(void* _data);

	static void DataManagerUpdateEntityTable(void* _data);

	static void DataManagerDeleteComponentData(void* _data);

	static void DataManagerRecycleEntityIds(void* _data);

	static void DataManagerClearCopiedLists(void* _data);

	static void SystemManagerUpdateSystemEntityLists(void* _data);

	static void MessageManagerSortMessages(void* _data);

	static void MessageManagerDeleteMessages(void* _data);

	static void SchedulerClearSystemEntityChangeLists(void* _data);

	struct RuntimeData
	{
		RuntimeInfo runtimeInfo;
	};

	struct UpdateData
	{
		RuntimeInfo runtimeInfo;
		System* system;
	};

	struct UpdateSystemEntityListsData
	{
		RuntimeInfo runtimeInfo;
		SystemManager* systemManager;
		std::vector<std::vector<unsigned int>*>* entitiesToAddToSystems;
		std::vector<std::vector<unsigned int>*>* entitiesToRemoveFromSystems;
	};

	struct EntitiesAddedData
	{
		RuntimeInfo runtimeInfo;
		System* system;
		std::vector<std::vector<unsigned int>*>* entitiesToAddToSystems;
	};

	struct EntitiesRemovedData
	{
		RuntimeInfo runtimeInfo;
		System* system;
		std::vector<std::vector<unsigned int>*>* entitiesToRemoveFromSystems;
	};

	struct SortMessagesData
	{
		RuntimeInfo runtimeInfo;
		MessageManager* messageManager;
	};

	struct MessagesReceivedData
	{
		RuntimeInfo runtimeInfo;
		MessageManager* messageManager;
		System* system;
	};

	struct DeleteMessagesData
	{
		RuntimeInfo runtimeInfo;
		MessageManager* messageManager;
	};

	struct CopyCurrentListsData
	{
		RuntimeInfo runtimeInfo;
		DataManager* dataManager;
	};

	struct UpdateEntityTableData
	{
		RuntimeInfo runtimeInfo;
		DataManager* dataManager;
	};

	struct DeleteComponentDataData
	{
		RuntimeInfo runtimeInfo;
		DataManager* dataManager;
	};

	struct RecycleEntityIdsData
	{
		RuntimeInfo runtimeInfo;
		DataManager* dataManager;
	};

	struct ClearCopiedListsData
	{
		RuntimeInfo runtimeInfo;
		DataManager* dataManager;
	};

	struct ClearListsData
	{
		RuntimeInfo runtimeInfo;
		Scheduler* scheduler;
	};
};

#endif