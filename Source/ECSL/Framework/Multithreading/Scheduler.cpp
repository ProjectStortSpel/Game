#include "Scheduler.h"

#include <assert.h>
#include <sstream>

using namespace ECSL;

Scheduler::Scheduler(DataManager* _dataManager, SystemManager* _systemManager, MessageManager* _messageManager)
:	m_dataManager(_dataManager),
	m_systemManager(_systemManager),
	m_messageManager(_messageManager),
	m_workItems(new std::vector<ECSLWorkItem*>()),
	m_updateWorkItems(new std::vector<std::vector<ECSLWorkItem*>*>()),
	m_entitiesAddedWorkItems(new std::vector<std::vector<ECSLWorkItem*>*>()),
	m_entitiesRemovedWorkItems(new std::vector<std::vector<ECSLWorkItem*>*>()),
	m_sortMessagesWorkItems(new std::vector<ECSLWorkItem*>()),
	m_messagesReceivedWorkItems(new std::vector<std::vector<ECSLWorkItem*>*>()),
	m_deleteMessagesWorkItems(new std::vector<ECSLWorkItem*>()),
	m_updateSystemEntityListsWorkItems(new std::vector<ECSLWorkItem*>()),
	m_copyCurrentListsWorkItems(new std::vector<ECSLWorkItem*>()),
	m_updateEntityTableWorkItems(new std::vector<ECSLWorkItem*>()),
	m_deleteComponentDataWorkItems(new std::vector<ECSLWorkItem*>()),
	m_recycleEntityIdsWorkItems(new std::vector<ECSLWorkItem*>()),
	m_clearListsWorkItems(new std::vector<ECSLWorkItem*>()),
	m_deactivatedWorkItems(new std::map<unsigned int, std::vector<DeactivatedWorkItem*>>())
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
	delete(m_messagesReceivedWorkItems);
	delete(m_deleteMessagesWorkItems);
	delete(m_updateSystemEntityListsWorkItems);
	delete(m_updateEntityTableWorkItems);
	delete(m_recycleEntityIdsWorkItems);
	delete(m_clearListsWorkItems);
	delete(m_deactivatedWorkItems);

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

void Scheduler::UpdateWorkItemLists()
{
	SystemActivationManager* systemActivationManager = m_systemManager->GetSystemActivationManager();
	std::vector<unsigned int>* systemsToActivate = systemActivationManager->PullSystemsToActivate();
	std::vector<unsigned int>* systemsToDeactivate = systemActivationManager->PullSystemsToDeactivate();

	/* Deactivate systems */
	for (const auto systemId : *systemsToDeactivate)
	{
		bool updateErased = EraseSystemWorkItems(systemId, DeactivatedWorkItem::WorkItemType::Update, m_updateWorkItems);
		bool messagesReceivedErased = EraseSystemWorkItems(systemId, DeactivatedWorkItem::WorkItemType::MessagesReceived, m_messagesReceivedWorkItems);
		bool entitiesAddedErased = EraseSystemWorkItems(systemId, DeactivatedWorkItem::WorkItemType::EntitiesAdded, m_entitiesAddedWorkItems);
		bool entitiesRemovedErased = EraseSystemWorkItems(systemId, DeactivatedWorkItem::WorkItemType::EntitiesRemoved, m_entitiesRemovedWorkItems);
		/* System is already removed or doesn't have any of the system stages except Initialize() */
		assert(updateErased || messagesReceivedErased || entitiesAddedErased || entitiesRemovedErased);
	}

	/* Activate systems */
	for (const auto systemId : *systemsToActivate)
	{
		/* System is already active */
		assert(m_deactivatedWorkItems->find(systemId) != m_deactivatedWorkItems->end());
		auto it = m_deactivatedWorkItems->find(systemId);
		for (auto deactivatedWorkItem : it->second)
		{
			ECSLWorkItem* workItem = deactivatedWorkItem->workItem;
			switch (deactivatedWorkItem->type)
			{
			case DeactivatedWorkItem::WorkItemType::Update:
				(*m_updateWorkItems)[workItem->GroupId]->push_back(workItem);
				break;

			case DeactivatedWorkItem::WorkItemType::MessagesReceived:
				(*m_messagesReceivedWorkItems)[workItem->GroupId]->push_back(workItem);
				break;

			case DeactivatedWorkItem::WorkItemType::EntitiesAdded:
				(*m_entitiesAddedWorkItems)[workItem->GroupId]->push_back(workItem);
				break;

			case DeactivatedWorkItem::WorkItemType::EntitiesRemoved:
				(*m_entitiesRemovedWorkItems)[workItem->GroupId]->push_back(workItem);
				break;
			}
		}
		it->second.clear();
	}

	delete(systemsToActivate);
	delete(systemsToDeactivate);
}

bool Scheduler::EraseSystemWorkItems(unsigned int _systemId, DeactivatedWorkItem::WorkItemType _workItemType, std::vector<std::vector<ECSLWorkItem*>*>* _workItemGroups)
{
	bool elementsErased = false;
	for (const auto workItemGroup : *_workItemGroups)
	{
		std::vector<ECSLWorkItem*>::iterator start = workItemGroup->end();
		std::vector<ECSLWorkItem*>::iterator end = workItemGroup->end();
		for (auto it = workItemGroup->begin(); it != workItemGroup->end(); ++it)
		{
			ECSLWorkItem* workItem = *it;
			if (workItem->SystemId == _systemId)
			{
				if (start == workItemGroup->end())
				{
					start = it;
					end = it + 1;
				}
				else
					end = it + 1;
				DeactivatedWorkItem* deactivatedWorkItem = new DeactivatedWorkItem();
				deactivatedWorkItem->type = _workItemType;
				deactivatedWorkItem->workItem = workItem;
				(*m_deactivatedWorkItems)[workItem->SystemId].push_back(deactivatedWorkItem);
			}
		}
		if (start != workItemGroup->end())
		{
			workItemGroup->erase(start, end);
			elementsErased = true;
		}
	}
	return elementsErased;
}

MPL::TaskId Scheduler::ScheduleUpdateSystems(MPL::TaskId _dependency)
{
	MPL::TaskId currentTaskId = _dependency;
	MPL::TaskId lastTaskId;
	/* Create tasks of each groups' work items. Each new task will have a dependency to the previous task */
	for (auto workItems : *m_updateWorkItems)
	{
		if (workItems->size() == 0)
			continue;
		/* Send the work items to the TaskManager and create a task out of it */
		lastTaskId = currentTaskId;
		currentTaskId = m_taskManager->Add(lastTaskId, *(std::vector<MPL::WorkItem*>*)workItems);
	}
	return currentTaskId;
}

MPL::TaskId Scheduler::ScheduleUpdateSystemEntityLists(MPL::TaskId _dependency)
{
	return m_taskManager->Add(_dependency, *(std::vector<MPL::WorkItem*>*)m_updateSystemEntityListsWorkItems);
}

MPL::TaskId Scheduler::ScheduleEntitiesAdded(MPL::TaskId _dependency)
{
	MPL::TaskId currentTaskId = _dependency;
	MPL::TaskId lastTaskId;
	/* Create tasks of each groups' work items. Each new task will have a dependency to the previous task */
	for (auto workItems : *m_entitiesAddedWorkItems)
	{
		if (workItems->size() == 0)
			continue;
		/* Send the work items to the TaskManager and create a task out of it */
		lastTaskId = currentTaskId;
		currentTaskId = m_taskManager->Add(lastTaskId, *(std::vector<MPL::WorkItem*>*)workItems);
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
		if (workItems->size() == 0)
			continue;
		/* Send the work items to the TaskManager and create a task out of it */
		lastTaskId = currentTaskId;
		currentTaskId = m_taskManager->Add(lastTaskId, *(std::vector<MPL::WorkItem*>*)workItems);
	}
	return currentTaskId;
}

MPL::TaskId Scheduler::ScheduleSortMessages(MPL::TaskId _dependency)
{
	return m_taskManager->Add(_dependency, *(std::vector<MPL::WorkItem*>*)m_sortMessagesWorkItems);
}

MPL::TaskId Scheduler::ScheduleMessagesReceived(MPL::TaskId _dependency)
{
	MPL::TaskId currentTaskId = _dependency;
	MPL::TaskId lastTaskId;
	/* Create tasks of each groups' work items. Each new task will have a dependency to the previous task */
	for (auto workItems : *m_messagesReceivedWorkItems)
	{
		if (workItems->size() == 0)
			continue;
		/* Send the work items to the TaskManager and create a task out of it */
		lastTaskId = currentTaskId;
		currentTaskId = m_taskManager->Add(lastTaskId, *(std::vector<MPL::WorkItem*>*)workItems);
	}
	return currentTaskId;
}

MPL::TaskId Scheduler::ScheduleDeleteMessages(MPL::TaskId _dependency)
{
	return m_taskManager->Add(_dependency, *(std::vector<MPL::WorkItem*>*)m_deleteMessagesWorkItems);
}

MPL::TaskId Scheduler::ScheduleCopyCurrentLists(MPL::TaskId _dependency)
{
	return m_taskManager->Add(_dependency, *(std::vector<MPL::WorkItem*>*)m_copyCurrentListsWorkItems);
}

MPL::TaskId Scheduler::ScheduleUpdateEntityTable(MPL::TaskId _dependency)
{
	return m_taskManager->Add(_dependency, *(std::vector<MPL::WorkItem*>*)m_updateEntityTableWorkItems);
}

MPL::TaskId Scheduler::ScheduleDeleteComponentData(MPL::TaskId _dependency)
{
	return m_taskManager->Add(_dependency, *(std::vector<MPL::WorkItem*>*)m_deleteComponentDataWorkItems);
}

MPL::TaskId Scheduler::ScheduleRecycleEntities(MPL::TaskId _dependency)
{
	return m_taskManager->Add(_dependency, *(std::vector<MPL::WorkItem*>*)m_recycleEntityIdsWorkItems);
}

MPL::TaskId Scheduler::ScheduleClearSystemEntityChangeLists(MPL::TaskId _dependency)
{
	return m_taskManager->Add(_dependency, *(std::vector<MPL::WorkItem*>*)m_clearListsWorkItems);
}

void Scheduler::PerformUpdateSystems()
{
	for (auto workItems : *m_updateWorkItems)
	{
		for (auto workItem : *workItems)
			workItem->Work(workItem->Data);
	}
}

void Scheduler::PerformUpdateSystemEntityLists()
{
	for (auto workItem : *m_updateSystemEntityListsWorkItems)
		workItem->Work(workItem->Data);
}

void Scheduler::PerformEntitiesAdded()
{
	for (auto workItems : *m_entitiesAddedWorkItems)
	{
		for (auto workItem : *workItems)
			workItem->Work(workItem->Data);
	}
}

void Scheduler::PerformEntitiesRemoved()
{
	for (auto workItems : *m_entitiesRemovedWorkItems)
	{
		for (auto workItem : *workItems)
			workItem->Work(workItem->Data);
	}
}

void Scheduler::PerformSortMessages()
{
	for (auto workItem : *m_sortMessagesWorkItems)
		workItem->Work(workItem->Data);
}

void Scheduler::PerformMessagesReceived()
{
	for (auto workItems : *m_messagesReceivedWorkItems)
	{
		for (auto workItem : *workItems)
			workItem->Work(workItem->Data);
	}
}

void Scheduler::PerformDeleteMessages()
{
	for (auto workItem : *m_deleteMessagesWorkItems)
		workItem->Work(workItem->Data);
}

void Scheduler::PerformCopyCurrentLists()
{
	for (auto workItem : *m_copyCurrentListsWorkItems)
		workItem->Work(workItem->Data);
}

void Scheduler::PerformUpdateEntityTable()
{
	for (auto workItem : *m_updateEntityTableWorkItems)
		workItem->Work(workItem->Data);
}

void Scheduler::PerformDeleteComponentData()
{
	for (auto workItem : *m_deleteComponentDataWorkItems)
		workItem->Work(workItem->Data);
}

void Scheduler::PerformRecycleEntities()
{
	for (auto workItem : *m_recycleEntityIdsWorkItems)
		workItem->Work(workItem->Data);
}

void Scheduler::PerformClearSystemEntityChangeLists()
{
	for (auto workItem : *m_clearListsWorkItems)
		workItem->Work(workItem->Data);
}

void Scheduler::AddUpdateSystemsTasks()
{
	for (auto systemGroup : *m_systemManager->GetSystemWorkGroups())
	{
		int localGroupId = -1;
		std::vector<ECSLWorkItem*>* workItems = new std::vector<ECSLWorkItem*>();
		for (auto system : *systemGroup->GetSystems())
		{
			/* Create one work item for each system update task */
			for (unsigned int taskIndex = 0; taskIndex < system->GetUpdateTaskCount(); ++taskIndex)
			{
				UpdateData* data = new UpdateData();
				data->system = system;
				data->runtimeInfo.TaskIndex = taskIndex;
				data->runtimeInfo.TaskCount = system->GetUpdateTaskCount();
				ECSLWorkItem* workItem = new ECSLWorkItem();
				workItem->SystemId = system->GetId();
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
		ECSLWorkItem* workItem = new ECSLWorkItem();
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
		std::vector<ECSLWorkItem*>* workItems = new std::vector<ECSLWorkItem*>();
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
				ECSLWorkItem* workItem = new ECSLWorkItem();
				workItem->SystemId = system->GetId();
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
		std::vector<ECSLWorkItem*>* workItems = new std::vector<ECSLWorkItem*>();
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
				ECSLWorkItem* workItem = new ECSLWorkItem();
				workItem->SystemId = system->GetId();
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
		ECSLWorkItem* workItem = new ECSLWorkItem();
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

void Scheduler::AddMessagesReceivedTasks()
{
	for (auto systemGroup : *m_systemManager->GetSystemWorkGroups())
	{
		int localGroupId = -1;
		std::vector<ECSLWorkItem*>* workItems = new std::vector<ECSLWorkItem*>();
		for (auto system : *systemGroup->GetSystems())
		{
			/* Create one work item for each system update task */
			for (unsigned int taskIndex = 0; taskIndex < system->GetMessagesReceivedTaskCount(); ++taskIndex)
			{
				MessagesReceivedData* data = new MessagesReceivedData();
				data->messageManager = m_messageManager;
				data->system = system;
				data->runtimeInfo.TaskIndex = taskIndex;
				data->runtimeInfo.TaskCount = system->GetMessagesReceivedTaskCount();
				ECSLWorkItem* workItem = new ECSLWorkItem();
				workItem->SystemId = system->GetId();
				workItem->Work = &SystemMessagesReceived;
				workItem->Data = data;
				std::stringstream s;
				s << system->GetSystemName() << "->MessagesReceived() Task: " << taskIndex;
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
			m_messagesReceivedWorkItems->push_back(workItems);
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
		ECSLWorkItem* workItem = new ECSLWorkItem();
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
		ECSLWorkItem* workItem = new ECSLWorkItem();
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
	const unsigned int updateEntityTableWorkCount = m_taskManager->GetThreadCount();

	for (unsigned int i = 0; i < updateEntityTableWorkCount; ++i)
	{
		UpdateEntityTableData* data = new UpdateEntityTableData();
		data->dataManager = m_dataManager;
		data->runtimeInfo.TaskIndex = i;
		data->runtimeInfo.TaskCount = updateEntityTableWorkCount;
		ECSLWorkItem* workItem = new ECSLWorkItem();
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
	const unsigned int deleteComponentDataWorkCount = m_taskManager->GetThreadCount();

	for (unsigned int i = 0; i < deleteComponentDataWorkCount; ++i)
	{
		DeleteComponentDataData* data = new DeleteComponentDataData();
		data->dataManager = m_dataManager;
		data->runtimeInfo.TaskIndex = i;
		data->runtimeInfo.TaskCount = deleteComponentDataWorkCount;
		ECSLWorkItem* workItem = new ECSLWorkItem();
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
		ECSLWorkItem* workItem = new ECSLWorkItem();
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

void Scheduler::AddClearSystemEntityChangeListsTask()
{
	const unsigned int clearSystemEntityChangeListsCount = 1;

	for (unsigned int i = 0; i < clearSystemEntityChangeListsCount; ++i)
	{
		ClearListsData* data = new ClearListsData();
		data->scheduler = this;
		data->runtimeInfo.TaskIndex = i;
		data->runtimeInfo.TaskCount = clearSystemEntityChangeListsCount;
		ECSLWorkItem* workItem = new ECSLWorkItem();
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

void ECSL::SystemMessagesReceived(void* _data)
{
	MessagesReceivedData* data = (MessagesReceivedData*)_data;
	data->system->MessagesReceived(*data->messageManager->GetMessagesToSystem(data->system->GetId()));
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