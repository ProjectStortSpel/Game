#include "Scheduler.h"

using namespace ECSL;

Scheduler::Scheduler(DataManager* _dataManager, SystemManager* _systemManager, MessageManager* _messageManager)
:	m_dataManager(_dataManager),
	m_systemManager(_systemManager),
	m_messageManager(_messageManager),
	m_workItems(new std::vector<MPL::WorkItem*>()),
	m_updateWorkItems(new std::vector<std::vector<MPL::WorkItem*>*>()),
	m_entitiesAddedWorkItems(new std::vector<std::vector<MPL::WorkItem*>*>()),
	m_entitiesRemovedWorkItems(new std::vector<std::vector<MPL::WorkItem*>*>()),
	m_sortMessagesWorkItems(new std::vector<MPL::WorkItem*>()),
	m_messagesRecievedWorkItems(new std::vector<std::vector<MPL::WorkItem*>*>()),
	m_deleteMessagesWorkItems(new std::vector<MPL::WorkItem*>()),
	m_updateSystemEntityListsWorkItems(new std::vector<MPL::WorkItem*>()),
	m_copyCurrentListsWorkItems(new std::vector<MPL::WorkItem*>()),
	m_updateEntityTableWorkItems(new std::vector<MPL::WorkItem*>()),
	m_deleteComponentDataWorkItems(new std::vector<MPL::WorkItem*>()),
	m_recycleEntityIdsWorkItems(new std::vector<MPL::WorkItem*>()),
	m_clearCopiedListsWorkItems(new std::vector<MPL::WorkItem*>()),
	m_clearListsWorkItems(new std::vector<MPL::WorkItem*>())
{
	unsigned int workCount = MPL::TaskManager::GetInstance().GetThreadCount();
	m_entitiesToAddToSystems = new std::vector<std::vector<unsigned int>*>();
	m_entitiesToRemoveFromSystems = new std::vector<std::vector<unsigned int>*>();
	for (unsigned int i = 0; i < m_systemManager->GetSystems()->size(); ++i)
	{
		m_entitiesToAddToSystems->push_back(new std::vector<unsigned int>());
		m_entitiesToRemoveFromSystems->push_back(new std::vector<unsigned int>());
	}

	m_taskManager = &MPL::TaskManager::GetInstance();
}

Scheduler::~Scheduler()
{
	for (auto workItem : *m_workItems)
		delete(workItem);
	delete(m_updateWorkItems);
	delete(m_entitiesAddedWorkItems);
	delete(m_entitiesRemovedWorkItems);
	delete(m_sortMessagesWorkItems);
	delete(m_messagesRecievedWorkItems);
	delete(m_deleteMessagesWorkItems);
	delete(m_updateSystemEntityListsWorkItems);
	delete(m_updateEntityTableWorkItems);
	delete(m_recycleEntityIdsWorkItems);
	delete(m_clearListsWorkItems);

	for (auto systemGroup : *m_entitiesToAddToSystems)
		delete(systemGroup);
	for (auto systemGroup : *m_entitiesToRemoveFromSystems)
		delete(systemGroup);
	delete(m_entitiesToAddToSystems);
	delete(m_entitiesToRemoveFromSystems);
}

void Scheduler::UpdateDt(float _dt)
{
	for (auto workItem : *m_workItems)
	{
		((RuntimeData*)workItem->Data)->RuntimeInfo.Dt = _dt;
	}
}

MPL::TaskId Scheduler::ScheduleUpdateSystems(MPL::TaskId _dependency)
{
	MPL::TaskId currentTaskId = _dependency;
	MPL::TaskId lastTaskId;
	/* Create tasks of each groups' work items. Each new task will have a dependency to the previous task */
	for (auto workItems : *m_updateWorkItems)
	{
		/* Send the work items to the TaskManager and create a task out of it */
		lastTaskId = currentTaskId;
		currentTaskId = m_taskManager->Add(lastTaskId, *workItems);
	}
	return currentTaskId;
}

MPL::TaskId Scheduler::ScheduleUpdateSystemEntityLists(MPL::TaskId _dependency)
{
	return m_taskManager->Add(_dependency, *m_updateSystemEntityListsWorkItems);
}

MPL::TaskId Scheduler::ScheduleEntitiesAdded(MPL::TaskId _dependency)
{
	MPL::TaskId currentTaskId = _dependency;
	MPL::TaskId lastTaskId;
	/* Create tasks of each groups' work items. Each new task will have a dependency to the previous task */
	for (auto workItems : *m_entitiesAddedWorkItems)
	{
		/* Send the work items to the TaskManager and create a task out of it */
		lastTaskId = currentTaskId;
		currentTaskId = m_taskManager->Add(lastTaskId, *workItems);
	}
	return currentTaskId;
}

MPL::TaskId Scheduler::ScheduleEntitiesRemoved(MPL::TaskId _dependency)
{
	MPL::TaskId currentTaskId = _dependency;
	MPL::TaskId lastTaskId;
	/* Create tasks of each groups' work items. Each new task will have a dependency to the previous task */
	for (auto workItems : *m_entitiesRemovedWorkItems)
	{
		/* Send the work items to the TaskManager and create a task out of it */
		lastTaskId = currentTaskId;
		currentTaskId = m_taskManager->Add(lastTaskId, *workItems);
	}
	return currentTaskId;
}

MPL::TaskId Scheduler::ScheduleSortMessages(MPL::TaskId _dependency)
{
	return m_taskManager->Add(_dependency, *m_sortMessagesWorkItems);
}

MPL::TaskId Scheduler::ScheduleMessagesRecieved(MPL::TaskId _dependency)
{
	MPL::TaskId currentTaskId = _dependency;
	MPL::TaskId lastTaskId;
	/* Create tasks of each groups' work items. Each new task will have a dependency to the previous task */
	for (auto workItems : *m_messagesRecievedWorkItems)
	{
		/* Send the work items to the TaskManager and create a task out of it */
		lastTaskId = currentTaskId;
		currentTaskId = m_taskManager->Add(lastTaskId, *workItems);
	}
	return currentTaskId;
}

MPL::TaskId Scheduler::ScheduleDeleteMessages(MPL::TaskId _dependency)
{
	return m_taskManager->Add(_dependency, *m_deleteMessagesWorkItems);
}

MPL::TaskId Scheduler::ScheduleCopyCurrentLists(MPL::TaskId _dependency)
{
	return m_taskManager->Add(_dependency, *m_copyCurrentListsWorkItems);
}

MPL::TaskId Scheduler::ScheduleUpdateEntityTable(MPL::TaskId _dependency)
{
	return m_taskManager->Add(_dependency, *m_updateEntityTableWorkItems);
}

MPL::TaskId Scheduler::ScheduleDeleteComponentData(MPL::TaskId _dependency)
{
	return m_taskManager->Add(_dependency, *m_deleteComponentDataWorkItems);
}

MPL::TaskId Scheduler::ScheduleRecycleEntities(MPL::TaskId _dependency)
{
	return m_taskManager->Add(_dependency, *m_recycleEntityIdsWorkItems);
}

MPL::TaskId Scheduler::ScheduleClearCopiedLists(MPL::TaskId _dependency)
{
	return m_taskManager->Add(_dependency, *m_clearCopiedListsWorkItems);
}

MPL::TaskId Scheduler::ScheduleClearSystemEntityChangeLists(MPL::TaskId _dependency)
{
	return m_taskManager->Add(_dependency, *m_clearListsWorkItems);
}

void Scheduler::AddUpdateSystemsTasks()
{
	for (auto systemGroup : *m_systemManager->GetSystemWorkGroups())
	{
		std::vector<MPL::WorkItem*>* workItems = new std::vector<MPL::WorkItem*>();
		for (auto system : *systemGroup->GetSystems())
		{
			/* Create one work item for each system update task */
			for (unsigned int taskIndex = 0; taskIndex < system->GetUpdateTaskCount(); ++taskIndex)
			{
				UpdateData* data = new UpdateData();
				data->System = system;
				data->RuntimeInfo.TaskIndex = taskIndex;
				data->RuntimeInfo.TaskCount = system->GetUpdateTaskCount();
				MPL::WorkItem* workItem = new MPL::WorkItem();
				workItem->Work = &SystemUpdate;
				workItem->Data = data;
				workItems->push_back(workItem);
				m_workItems->push_back(workItem);
			}
		}
		/* Add the work item list if it isn't empty. Else just delete it */
		if (workItems->size() > 0)
			m_updateWorkItems->push_back(workItems);
		else
			delete(workItems);
	}
}

void Scheduler::AddUpdateSystemEntityListsTasks()
{
	const unsigned int updateSystemEntityListsWorkCount = m_taskManager->GetThreadCount();

	for (unsigned int i = 0; i < updateSystemEntityListsWorkCount; ++i)
	{
		UpdateSystemEntityListsData* data = new UpdateSystemEntityListsData();
		data->SystemManager = m_systemManager;
		data->EntitiesToAddToSystems = m_entitiesToAddToSystems;
		data->EntitiesToRemoveFromSystems = m_entitiesToRemoveFromSystems;
		data->RuntimeInfo.TaskIndex = i;
		data->RuntimeInfo.TaskCount = updateSystemEntityListsWorkCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &SystemManagerUpdateSystemEntityLists;
		workItem->Data = data;
		m_updateSystemEntityListsWorkItems->push_back(workItem);
		m_workItems->push_back(workItem);
	}
}

void Scheduler::AddEntitiesAddedTasks()
{
	for (auto systemGroup : *m_systemManager->GetSystemWorkGroups())
	{
		std::vector<MPL::WorkItem*>* workItems = new std::vector<MPL::WorkItem*>();
		for (auto system : *systemGroup->GetSystems())
		{
			/* Create one work item for each systems' OnEntityAdded task */
			for (unsigned int taskIndex = 0; taskIndex < system->GetEntitiesAddedTaskCount(); ++taskIndex)
			{
				EntitiesAddedData* data = new EntitiesAddedData();
				data->System = system;
				data->RuntimeInfo.TaskIndex = taskIndex;
				data->RuntimeInfo.TaskCount = system->GetEntitiesAddedTaskCount();
				data->EntitiesToAddToSystems = m_entitiesToAddToSystems;
				MPL::WorkItem* workItem = new MPL::WorkItem();
				workItem->Work = &SystemEntitiesAdded;
				workItem->Data = data;
				workItems->push_back(workItem);
				m_workItems->push_back(workItem);
			}
		}
		/* Add the work item list if it isn't empty. Else just delete it */
		if (workItems->size() > 0)
			m_entitiesAddedWorkItems->push_back(workItems);
		else
			delete(workItems);
	}
}

void Scheduler::AddEntitiesRemovedTasks()
{
	for (auto systemGroup : *m_systemManager->GetSystemWorkGroups())
	{
		std::vector<MPL::WorkItem*>* workItems = new std::vector<MPL::WorkItem*>();
		for (auto system : *systemGroup->GetSystems())
		{
			/* Create one work item for each systems' OnEntityAdded task */
			for (unsigned int taskIndex = 0; taskIndex < system->GetEntitiesRemovedTaskCount(); ++taskIndex)
			{
				EntitiesRemovedData* data = new EntitiesRemovedData();
				data->System = system;
				data->RuntimeInfo.TaskIndex = taskIndex;
				data->RuntimeInfo.TaskCount = system->GetEntitiesRemovedTaskCount();
				data->EntitiesToRemoveFromSystems = m_entitiesToRemoveFromSystems;
				MPL::WorkItem* workItem = new MPL::WorkItem();
				workItem->Work = &SystemEntitiesRemoved;
				workItem->Data = data;
				workItems->push_back(workItem);
				m_workItems->push_back(workItem);
			}
		}
		/* Add the work item list if it isn't empty. Else just delete it */
		if (workItems->size() > 0)
			m_entitiesRemovedWorkItems->push_back(workItems);
		else
			delete(workItems);
	}
}

void Scheduler::AddSortMessagesTask()
{
	const unsigned int sortMessagesWorkCount = m_taskManager->GetThreadCount();

	for (unsigned int i = 0; i < sortMessagesWorkCount; ++i)
	{
		SortMessagesData* data = new SortMessagesData();
		data->MessageManager = m_messageManager;
		data->RuntimeInfo.TaskIndex = i;
		data->RuntimeInfo.TaskCount = sortMessagesWorkCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &MessageManagerSortMessages;
		workItem->Data = data;
		m_sortMessagesWorkItems->push_back(workItem);
		m_workItems->push_back(workItem);
	}
}

void Scheduler::AddMessagesRecievedTasks()
{
	for (auto systemGroup : *m_systemManager->GetSystemWorkGroups())
	{
		std::vector<MPL::WorkItem*>* workItems = new std::vector<MPL::WorkItem*>();
		for (auto system : *systemGroup->GetSystems())
		{
			/* Create one work item for each system update task */
			for (unsigned int taskIndex = 0; taskIndex < system->GetMessagesRecievedTaskCount(); ++taskIndex)
			{
				MessagesRecievedData* data = new MessagesRecievedData();
				data->MessageManager = m_messageManager;
				data->System = system;
				data->RuntimeInfo.TaskIndex = taskIndex;
				data->RuntimeInfo.TaskCount = system->GetMessagesRecievedTaskCount();
				MPL::WorkItem* workItem = new MPL::WorkItem();
				workItem->Work = &SystemMessagesRecieved;
				workItem->Data = data;
				workItems->push_back(workItem);
				m_workItems->push_back(workItem);
			}
		}
		/* Add the work item list if it isn't empty. Else just delete it */
		if (workItems->size() > 0)
			m_messagesRecievedWorkItems->push_back(workItems);
		else
			delete(workItems);
	}
}

void Scheduler::AddDeleteMessagesTask()
{
	const unsigned int deleteMessagesWorkCount = m_taskManager->GetThreadCount();

	for (unsigned int i = 0; i < deleteMessagesWorkCount; ++i)
	{
		DeleteMessagesData* data = new DeleteMessagesData();
		data->MessageManager = m_messageManager;
		data->RuntimeInfo.TaskIndex = i;
		data->RuntimeInfo.TaskCount = deleteMessagesWorkCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &MessageManagerDeleteMessages;
		workItem->Data = data;
		m_deleteMessagesWorkItems->push_back(workItem);
		m_workItems->push_back(workItem);
	}
}

void Scheduler::AddCopyCurrentListsTask()
{
	const unsigned int copyCurrentListsWorkCount = 1;

	for (unsigned int i = 0; i < copyCurrentListsWorkCount; ++i)
	{
		CopyCurrentListsData* data = new CopyCurrentListsData();
		data->DataManager = m_dataManager;
		data->RuntimeInfo.TaskIndex = i;
		data->RuntimeInfo.TaskCount = copyCurrentListsWorkCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &DataManagerCopyCurrentLists;
		workItem->Data = data;
		m_copyCurrentListsWorkItems->push_back(workItem);
		m_workItems->push_back(workItem);
	}
}

void Scheduler::AddUpdateEntityTableTask()
{
	const unsigned int updateEntityTableWorkCount = 1;

	for (unsigned int i = 0; i < updateEntityTableWorkCount; ++i)
	{
		UpdateEntityTableData* data = new UpdateEntityTableData();
		data->DataManager = m_dataManager;
		data->RuntimeInfo.TaskIndex = i;
		data->RuntimeInfo.TaskCount = updateEntityTableWorkCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &DataManagerUpdateEntityTable;
		workItem->Data = data;
		m_updateEntityTableWorkItems->push_back(workItem);
		m_workItems->push_back(workItem);
	}
}

void Scheduler::AddDeleteComponentDataTask()
{
	const unsigned int deleteComponentDataWorkCount = 1;

	for (unsigned int i = 0; i < deleteComponentDataWorkCount; ++i)
	{
		DeleteComponentDataData* data = new DeleteComponentDataData();
		data->DataManager = m_dataManager;
		data->RuntimeInfo.TaskIndex = i;
		data->RuntimeInfo.TaskCount = deleteComponentDataWorkCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &DataManagerDeleteComponentData;
		workItem->Data = data;
		m_deleteComponentDataWorkItems->push_back(workItem);
		m_workItems->push_back(workItem);
	}
}

void Scheduler::AddRecycleEntityIdsTask()
{
	const unsigned int recycleEntityIdsWorkCount = 1;

	for (unsigned int i = 0; i < recycleEntityIdsWorkCount; ++i)
	{
		RecycleEntityIdsData* data = new RecycleEntityIdsData();
		data->DataManager = m_dataManager;
		data->RuntimeInfo.TaskIndex = i;
		data->RuntimeInfo.TaskCount = recycleEntityIdsWorkCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &DataManagerRecycleEntityIds;
		workItem->Data = data;
		m_recycleEntityIdsWorkItems->push_back(workItem);
		m_workItems->push_back(workItem);
	}
}

void Scheduler::AddClearCopiedListsTask()
{
	const unsigned int clearCopiedListsWorkCount = 1;

	for (unsigned int i = 0; i < clearCopiedListsWorkCount; ++i)
	{
		ClearCopiedListsData* data = new ClearCopiedListsData();
		data->DataManager = m_dataManager;
		data->RuntimeInfo.TaskIndex = i;
		data->RuntimeInfo.TaskCount = clearCopiedListsWorkCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &DataManagerClearCopiedLists;
		workItem->Data = data;
		m_clearCopiedListsWorkItems->push_back(workItem);
		m_workItems->push_back(workItem);
	}
}

void Scheduler::AddClearSystemEntityChangeListsTask()
{
	const unsigned int clearSystemEntityChangeListsCount = 1;

	for (unsigned int i = 0; i < clearSystemEntityChangeListsCount; ++i)
	{
		ClearListsData* data = new ClearListsData();
		data->Scheduler = this;
		data->RuntimeInfo.TaskIndex = i;
		data->RuntimeInfo.TaskCount = clearSystemEntityChangeListsCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &SchedulerClearSystemEntityChangeLists;
		workItem->Data = data;
		m_clearListsWorkItems->push_back(workItem);
		m_workItems->push_back(workItem);
	}
}

void Scheduler::ClearSystemEntityChangeLists(const RuntimeInfo& _runtimeInfo)
{
	for (auto systemGroup : *m_entitiesToAddToSystems)
		systemGroup->clear();
	for (auto systemGroup : *m_entitiesToRemoveFromSystems)
		systemGroup->clear();
}

void ECSL::SystemUpdate(void* _data)
{
	UpdateData* data = (UpdateData*)_data;
	data->System->Update(data->RuntimeInfo);
}

void ECSL::SystemEntitiesAdded(void* _data)
{
	EntitiesAddedData* data = (EntitiesAddedData*)_data;
	auto entitiesToAdd = data->EntitiesToAddToSystems->at(data->System->GetId());
	if (entitiesToAdd->size() > 0)
		data->System->EntitiesAdded(data->RuntimeInfo, *entitiesToAdd);
}

void ECSL::SystemEntitiesRemoved(void* _data)
{
	EntitiesRemovedData* data = (EntitiesRemovedData*)_data;
	auto entitiesToRemove = data->EntitiesToRemoveFromSystems->at(data->System->GetId());
	if (entitiesToRemove->size() > 0)
		data->System->EntitiesRemoved(data->RuntimeInfo, *entitiesToRemove);
}

void ECSL::SystemMessagesRecieved(void* _data)
{
	MessagesRecievedData* data = (MessagesRecievedData*)_data;
	data->System->MessagesRecieved(*data->MessageManager->GetMessagesToSystem(data->System->GetId()));
}

void ECSL::DataManagerCopyCurrentLists(void* _data)
{
	CopyCurrentListsData* data = (CopyCurrentListsData*)_data;
	data->DataManager->CopyCurrentLists(data->RuntimeInfo);
}

void ECSL::DataManagerUpdateEntityTable(void* _data)
{
	UpdateEntityTableData* data = (UpdateEntityTableData*)_data;
	data->DataManager->UpdateEntityTable(data->RuntimeInfo);
}

void ECSL::DataManagerDeleteComponentData(void* _data)
{
	DeleteComponentDataData* data = (DeleteComponentDataData*)_data;
	data->DataManager->DeleteComponentData(data->RuntimeInfo);
}

void ECSL::DataManagerRecycleEntityIds(void* _data)
{
	RecycleEntityIdsData* data = (RecycleEntityIdsData*)_data;
	data->DataManager->RecycleEntityIds(data->RuntimeInfo);
}

void ECSL::DataManagerClearCopiedLists(void* _data)
{
	ClearCopiedListsData* data = (ClearCopiedListsData*)_data;
	data->DataManager->ClearCopiedLists(data->RuntimeInfo);
}

void ECSL::SystemManagerUpdateSystemEntityLists(void* _data)
{
	UpdateSystemEntityListsData* data = (UpdateSystemEntityListsData*)_data;
	data->SystemManager->UpdateSystemEntityLists(data->RuntimeInfo, *data->EntitiesToAddToSystems, *data->EntitiesToRemoveFromSystems);
}

void ECSL::MessageManagerSortMessages(void* _data)
{
	SortMessagesData* data = (SortMessagesData*)_data;
	data->MessageManager->SortMessages(data->RuntimeInfo);
}

void ECSL::MessageManagerDeleteMessages(void* _data)
{
	DeleteMessagesData* data = (DeleteMessagesData*)_data;
	data->MessageManager->DeleteMessages(data->RuntimeInfo);
}

void ECSL::SchedulerClearSystemEntityChangeLists(void* _data)
{
	ClearListsData* data = (ClearListsData*)_data;
	data->Scheduler->ClearSystemEntityChangeLists(data->RuntimeInfo);
}