#include "Scheduler.h"

#include <sstream>

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
	m_currentGroupId = 0;
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
		((RuntimeData*)workItem->Data)->runtimeInfo.Dt = _dt;
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
		int localGroupId = -1;
		std::vector<MPL::WorkItem*>* workItems = new std::vector<MPL::WorkItem*>();
		for (auto system : *systemGroup->GetSystems())
		{
			/* Create one work item for each system update task */
			for (unsigned int taskIndex = 0; taskIndex < system->GetUpdateTaskCount(); ++taskIndex)
			{
				UpdateData* data = new UpdateData();
				data->system = system;
				data->runtimeInfo.TaskIndex = taskIndex;
				data->runtimeInfo.TaskCount = system->GetUpdateTaskCount();
				MPL::WorkItem* workItem = new MPL::WorkItem();
				workItem->Work = &SystemUpdate;
				workItem->Data = data;
				std::stringstream s;
				s << system->GetSystemName() << "->Update() Task: " << taskIndex;
				workItem->Name = new std::string(s.str());
				workItem->LocalGroupId = ++localGroupId;
				workItem->GroupId = m_currentGroupId;
				workItems->push_back(workItem);
				m_workItems->push_back(workItem);
			}
		}
		/* Add the work item list if it isn't empty. Else just delete it */
		if (workItems->size() > 0)
		{
			m_updateWorkItems->push_back(workItems);
			++m_currentGroupId;
		}
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
		data->systemManager = m_systemManager;
		data->entitiesToAddToSystems = m_entitiesToAddToSystems;
		data->entitiesToRemoveFromSystems = m_entitiesToRemoveFromSystems;
		data->runtimeInfo.TaskIndex = i;
		data->runtimeInfo.TaskCount = updateSystemEntityListsWorkCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &SystemManagerUpdateSystemEntityLists;
		workItem->Data = data;
		std::stringstream s;
		s << "SystemManager->UpdateSystemEntityList() Task: " << i;
		workItem->Name = new std::string(s.str());
		workItem->LocalGroupId = i;
		workItem->GroupId = m_currentGroupId;
		m_updateSystemEntityListsWorkItems->push_back(workItem);
		m_workItems->push_back(workItem);
	}

	++m_currentGroupId;
}

void Scheduler::AddEntitiesAddedTasks()
{
	for (auto systemGroup : *m_systemManager->GetSystemWorkGroups())
	{
		std::vector<MPL::WorkItem*>* workItems = new std::vector<MPL::WorkItem*>();
		int localGroupId = -1;
		for (auto system : *systemGroup->GetSystems())
		{
			/* Create one work item for each systems' OnEntityAdded task */
			for (unsigned int taskIndex = 0; taskIndex < system->GetEntitiesAddedTaskCount(); ++taskIndex)
			{
				EntitiesAddedData* data = new EntitiesAddedData();
				data->system = system;
				data->runtimeInfo.TaskIndex = taskIndex;
				data->runtimeInfo.TaskCount = system->GetEntitiesAddedTaskCount();
				data->entitiesToAddToSystems = m_entitiesToAddToSystems;
				MPL::WorkItem* workItem = new MPL::WorkItem();
				workItem->Work = &SystemEntitiesAdded;
				workItem->Data = data;
				std::stringstream s;
				s << system->GetSystemName() << "->EntitiesAdded() Task: " << taskIndex;
				workItem->Name = new std::string(s.str());
				workItem->LocalGroupId = ++localGroupId;
				workItem->GroupId = m_currentGroupId;
				workItems->push_back(workItem);
				m_workItems->push_back(workItem);
			}
		}
		/* Add the work item list if it isn't empty. Else just delete it */
		if (workItems->size() > 0)
		{
			m_entitiesAddedWorkItems->push_back(workItems);
			++m_currentGroupId;
		}
		else
			delete(workItems);
	}
}

void Scheduler::AddEntitiesRemovedTasks()
{
	for (auto systemGroup : *m_systemManager->GetSystemWorkGroups())
	{
		int localGroupId = -1;
		std::vector<MPL::WorkItem*>* workItems = new std::vector<MPL::WorkItem*>();
		for (auto system : *systemGroup->GetSystems())
		{
			/* Create one work item for each systems' OnEntityAdded task */
			for (unsigned int taskIndex = 0; taskIndex < system->GetEntitiesRemovedTaskCount(); ++taskIndex)
			{
				EntitiesRemovedData* data = new EntitiesRemovedData();
				data->system = system;
				data->runtimeInfo.TaskIndex = taskIndex;
				data->runtimeInfo.TaskCount = system->GetEntitiesRemovedTaskCount();
				data->entitiesToRemoveFromSystems = m_entitiesToRemoveFromSystems;
				MPL::WorkItem* workItem = new MPL::WorkItem();
				workItem->Work = &SystemEntitiesRemoved;
				workItem->Data = data;
				std::stringstream s;
				s << system->GetSystemName() << "->EntitiesRemoved() Task: " << taskIndex;
				workItem->Name = new std::string(s.str());
				workItem->LocalGroupId = ++localGroupId;
				workItem->GroupId = m_currentGroupId;
				workItems->push_back(workItem);
				m_workItems->push_back(workItem);
			}
		}
		/* Add the work item list if it isn't empty. Else just delete it */
		if (workItems->size() > 0)
		{
			m_entitiesRemovedWorkItems->push_back(workItems);
			++m_currentGroupId;
		}
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
		data->messageManager = m_messageManager;
		data->runtimeInfo.TaskIndex = i;
		data->runtimeInfo.TaskCount = sortMessagesWorkCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &MessageManagerSortMessages;
		workItem->Data = data;
		std::stringstream s;
		s << "MessageManager->SortMessages() Task: " << i;
		workItem->Name = new std::string(s.str());
		workItem->LocalGroupId = i;
		workItem->GroupId = m_currentGroupId;
		m_sortMessagesWorkItems->push_back(workItem);
		m_workItems->push_back(workItem);
	}

	++m_currentGroupId;
}

void Scheduler::AddMessagesRecievedTasks()
{
	for (auto systemGroup : *m_systemManager->GetSystemWorkGroups())
	{
		int localGroupId = -1;
		std::vector<MPL::WorkItem*>* workItems = new std::vector<MPL::WorkItem*>();
		for (auto system : *systemGroup->GetSystems())
		{
			/* Create one work item for each system update task */
			for (unsigned int taskIndex = 0; taskIndex < system->GetMessagesRecievedTaskCount(); ++taskIndex)
			{
				MessagesRecievedData* data = new MessagesRecievedData();
				data->messageManager = m_messageManager;
				data->system = system;
				data->runtimeInfo.TaskIndex = taskIndex;
				data->runtimeInfo.TaskCount = system->GetMessagesRecievedTaskCount();
				MPL::WorkItem* workItem = new MPL::WorkItem();
				workItem->Work = &SystemMessagesRecieved;
				workItem->Data = data;
				std::stringstream s;
				s << system->GetSystemName() << "->MessagesRecieved() Task: " << taskIndex;
				workItem->Name = new std::string(s.str());
				workItem->LocalGroupId = ++localGroupId;
				workItem->GroupId = m_currentGroupId;
				workItems->push_back(workItem);
				m_workItems->push_back(workItem);
			}
		}
		/* Add the work item list if it isn't empty. Else just delete it */
		if (workItems->size() > 0)
		{
			m_messagesRecievedWorkItems->push_back(workItems);
			++m_currentGroupId;
		}
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
		data->messageManager = m_messageManager;
		data->runtimeInfo.TaskIndex = i;
		data->runtimeInfo.TaskCount = deleteMessagesWorkCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &MessageManagerDeleteMessages;
		workItem->Data = data;
		std::stringstream s;
		s << "MessageManager->DeleteMessages() Task: " << i;
		workItem->Name = new std::string(s.str());
		workItem->LocalGroupId = i;
		workItem->GroupId = m_currentGroupId;
		m_deleteMessagesWorkItems->push_back(workItem);
		m_workItems->push_back(workItem);
	}

	++m_currentGroupId;
}

void Scheduler::AddCopyCurrentListsTask()
{
	const unsigned int copyCurrentListsWorkCount = 1;

	for (unsigned int i = 0; i < copyCurrentListsWorkCount; ++i)
	{
		CopyCurrentListsData* data = new CopyCurrentListsData();
		data->dataManager = m_dataManager;
		data->runtimeInfo.TaskIndex = i;
		data->runtimeInfo.TaskCount = copyCurrentListsWorkCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &DataManagerCopyCurrentLists;
		workItem->Data = data;
		std::stringstream s;
		s << "DataManager->CopyCurrentLists() Task: " << i;
		workItem->Name = new std::string(s.str());
		workItem->LocalGroupId = i;
		workItem->GroupId = m_currentGroupId;
		m_copyCurrentListsWorkItems->push_back(workItem);
		m_workItems->push_back(workItem);
	}

	++m_currentGroupId;
}

void Scheduler::AddUpdateEntityTableTask()
{
	const unsigned int updateEntityTableWorkCount = 1;

	for (unsigned int i = 0; i < updateEntityTableWorkCount; ++i)
	{
		UpdateEntityTableData* data = new UpdateEntityTableData();
		data->dataManager = m_dataManager;
		data->runtimeInfo.TaskIndex = i;
		data->runtimeInfo.TaskCount = updateEntityTableWorkCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &DataManagerUpdateEntityTable;
		workItem->Data = data;
		std::stringstream s;
		s << "DataManager->UpdateEntityTable() Task: " << i;
		workItem->Name = new std::string(s.str());
		workItem->LocalGroupId = i;
		workItem->GroupId = m_currentGroupId;
		m_updateEntityTableWorkItems->push_back(workItem);
		m_workItems->push_back(workItem);
	}

	++m_currentGroupId;
}

void Scheduler::AddDeleteComponentDataTask()
{
	const unsigned int deleteComponentDataWorkCount = 1;

	for (unsigned int i = 0; i < deleteComponentDataWorkCount; ++i)
	{
		DeleteComponentDataData* data = new DeleteComponentDataData();
		data->dataManager = m_dataManager;
		data->runtimeInfo.TaskIndex = i;
		data->runtimeInfo.TaskCount = deleteComponentDataWorkCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &DataManagerDeleteComponentData;
		workItem->Data = data;
		std::stringstream s;
		s << "DataManager->DeleteComponentData() Task: " << i;
		workItem->Name = new std::string(s.str());
		workItem->LocalGroupId = i;
		workItem->GroupId = m_currentGroupId;
		m_deleteComponentDataWorkItems->push_back(workItem);
		m_workItems->push_back(workItem);
	}

	++m_currentGroupId;
}

void Scheduler::AddRecycleEntityIdsTask()
{
	const unsigned int recycleEntityIdsWorkCount = 1;

	for (unsigned int i = 0; i < recycleEntityIdsWorkCount; ++i)
	{
		RecycleEntityIdsData* data = new RecycleEntityIdsData();
		data->dataManager = m_dataManager;
		data->runtimeInfo.TaskIndex = i;
		data->runtimeInfo.TaskCount = recycleEntityIdsWorkCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &DataManagerRecycleEntityIds;
		workItem->Data = data;
		std::stringstream s;
		s << "DataManager->RecycleEntityIds() Task: " << i;
		workItem->Name = new std::string(s.str());
		workItem->LocalGroupId = i;
		workItem->GroupId = m_currentGroupId;
		m_recycleEntityIdsWorkItems->push_back(workItem);
		m_workItems->push_back(workItem);
	}

	++m_currentGroupId;
}

void Scheduler::AddClearCopiedListsTask()
{
	const unsigned int clearCopiedListsWorkCount = 1;

	for (unsigned int i = 0; i < clearCopiedListsWorkCount; ++i)
	{
		ClearCopiedListsData* data = new ClearCopiedListsData();
		data->dataManager = m_dataManager;
		data->runtimeInfo.TaskIndex = i;
		data->runtimeInfo.TaskCount = clearCopiedListsWorkCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &DataManagerClearCopiedLists;
		workItem->Data = data;
		std::stringstream s;
		s << "DataManager->ClearCopiedLists() Task: " << i;
		workItem->Name = new std::string(s.str());
		workItem->LocalGroupId = i;
		workItem->GroupId = m_currentGroupId;
		m_clearCopiedListsWorkItems->push_back(workItem);
		m_workItems->push_back(workItem);
	}

	++m_currentGroupId;
}

void Scheduler::AddClearSystemEntityChangeListsTask()
{
	const unsigned int clearSystemEntityChangeListsCount = 1;

	for (unsigned int i = 0; i < clearSystemEntityChangeListsCount; ++i)
	{
		ClearListsData* data = new ClearListsData();
		data->scheduler = this;
		data->runtimeInfo.TaskIndex = i;
		data->runtimeInfo.TaskCount = clearSystemEntityChangeListsCount;
		MPL::WorkItem* workItem = new MPL::WorkItem();
		workItem->Work = &SchedulerClearSystemEntityChangeLists;
		workItem->Data = data;
		std::stringstream s;
		s << "Scheduler->ClearSystemEntityChangeLists() Task: " << i;
		workItem->Name = new std::string(s.str());
		workItem->LocalGroupId = i;
		workItem->GroupId = m_currentGroupId;
		m_clearListsWorkItems->push_back(workItem);
		m_workItems->push_back(workItem);
	}

	++m_currentGroupId;
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
	data->system->Update(data->runtimeInfo);
}

void ECSL::SystemEntitiesAdded(void* _data)
{
	EntitiesAddedData* data = (EntitiesAddedData*)_data;
	auto entitiesToAdd = data->entitiesToAddToSystems->at(data->system->GetId());
	if (entitiesToAdd->size() > 0)
		data->system->EntitiesAdded(data->runtimeInfo, *entitiesToAdd);
}

void ECSL::SystemEntitiesRemoved(void* _data)
{
	EntitiesRemovedData* data = (EntitiesRemovedData*)_data;
	auto entitiesToRemove = data->entitiesToRemoveFromSystems->at(data->system->GetId());
	if (entitiesToRemove->size() > 0)
		data->system->EntitiesRemoved(data->runtimeInfo, *entitiesToRemove);
}

void ECSL::SystemMessagesRecieved(void* _data)
{
	MessagesRecievedData* data = (MessagesRecievedData*)_data;
	data->system->MessagesRecieved(*data->messageManager->GetMessagesToSystem(data->system->GetId()));
}

void ECSL::DataManagerCopyCurrentLists(void* _data)
{
	CopyCurrentListsData* data = (CopyCurrentListsData*)_data;
	data->dataManager->CopyCurrentLists(data->runtimeInfo);
}

void ECSL::DataManagerUpdateEntityTable(void* _data)
{
	UpdateEntityTableData* data = (UpdateEntityTableData*)_data;
	data->dataManager->UpdateEntityTable(data->runtimeInfo);
}

void ECSL::DataManagerDeleteComponentData(void* _data)
{
	DeleteComponentDataData* data = (DeleteComponentDataData*)_data;
	data->dataManager->DeleteComponentData(data->runtimeInfo);
}

void ECSL::DataManagerRecycleEntityIds(void* _data)
{
	RecycleEntityIdsData* data = (RecycleEntityIdsData*)_data;
	data->dataManager->RecycleEntityIds(data->runtimeInfo);
}

void ECSL::DataManagerClearCopiedLists(void* _data)
{
	ClearCopiedListsData* data = (ClearCopiedListsData*)_data;
	data->dataManager->ClearCopiedLists(data->runtimeInfo);
}

void ECSL::SystemManagerUpdateSystemEntityLists(void* _data)
{
	UpdateSystemEntityListsData* data = (UpdateSystemEntityListsData*)_data;
	data->systemManager->UpdateSystemEntityLists(data->runtimeInfo, *data->entitiesToAddToSystems, *data->entitiesToRemoveFromSystems);
}

void ECSL::MessageManagerSortMessages(void* _data)
{
	SortMessagesData* data = (SortMessagesData*)_data;
	data->messageManager->SortMessages(data->runtimeInfo);
}

void ECSL::MessageManagerDeleteMessages(void* _data)
{
	DeleteMessagesData* data = (DeleteMessagesData*)_data;
	data->messageManager->DeleteMessages(data->runtimeInfo);
}

void ECSL::SchedulerClearSystemEntityChangeLists(void* _data)
{
	ClearListsData* data = (ClearListsData*)_data;
	data->scheduler->ClearSystemEntityChangeLists(data->runtimeInfo);
}