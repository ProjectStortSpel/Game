#include "Simulation.h"

#include <MPL/Managers/TaskManager.h>

using namespace ECSL;

Simulation::Simulation(DataManager* _dataManager, SystemManager* _systemManager, MessageManager* _messageManager)
: m_dataManager(_dataManager), m_systemManager(_systemManager), m_messageManager(_messageManager)
{
	m_scheduler = new Scheduler(_dataManager, _systemManager, _messageManager);
	m_scheduler->AddUpdateSystemsTasks();
	m_scheduler->AddSortMessagesTask();
	m_scheduler->AddMessagesRecievedTasks();
	m_scheduler->AddDeleteMessagesTask();
	m_scheduler->AddCopyCurrentListsTask();
	m_scheduler->AddUpdateEntityTableTask();
	m_scheduler->AddUpdateSystemEntityListsTasks();
	m_scheduler->AddEntitiesAddedTasks();
	m_scheduler->AddEntitiesRemovedTasks();
	m_scheduler->AddClearSystemEntityChangeListsTask();
	m_scheduler->AddDeleteComponentDataTask();
	m_scheduler->AddRecycleEntityIdsTask();
	m_scheduler->AddClearCopiedListsTask();
}

Simulation::~Simulation()
{
	delete(m_scheduler);
}

void Simulation::Update(float _dt)
{
	/* Update dt for every work item */
	m_scheduler->UpdateDt(_dt);

	/* Update systems */
	MPL::TaskId updateSystems = m_scheduler->ScheduleUpdateSystems(MPL::NoDependency);

	/* Sort all messages sent by systems */
	MPL::TaskId sortMessages = m_scheduler->ScheduleSortMessages(updateSystems);

	/* Call MessagesRecieved() for each system that has atleast one message recieved */
	MPL::TaskId messagesRecieved = m_scheduler->ScheduleMessagesRecieved(sortMessages);

	/* Delete all messages */
	MPL::TaskId deleteMessages = m_scheduler->ScheduleDeleteMessages(messagesRecieved);

	/* Copy all entity and component changes to new lists */
	MPL::TaskId copyCurrentLists = m_scheduler->ScheduleCopyCurrentLists(deleteMessages);

	/* Update entity component table data */
	MPL::TaskId updateEntityTable = m_scheduler->ScheduleUpdateEntityTable(copyCurrentLists);

	/* Update every systems' entity list */
	MPL::TaskId updateSystemEntityLists = m_scheduler->ScheduleUpdateSystemEntityLists(updateEntityTable);

	/* Call EntitiesAdded for each system that has atleast one newly added entity */
	MPL::TaskId entitiesAdded = m_scheduler->ScheduleEntitiesAdded(updateSystemEntityLists);

	/* Call EntitiesRemoved for each system that has atleast one newly removed entity */
	MPL::TaskId entitiesRemoved = m_scheduler->ScheduleEntitiesRemoved(entitiesAdded);

	/* Clear all the used lists in Scheduler (used for entitiesAdded and entitiesRemoved) */
	MPL::TaskId clearSystemEntityChangeLists = m_scheduler->ScheduleClearSystemEntityChangeLists(entitiesRemoved);

	/* Delete component data */
	MPL::TaskId deleteComponentData = m_scheduler->ScheduleDeleteComponentData(clearSystemEntityChangeLists);

	/* Recycle all dead ids back to the list of available ids */
	MPL::TaskId recycleEntityIds = m_scheduler->ScheduleRecycleEntities(deleteComponentData);

	/* Clear all the used lists in DataManager */
	MPL::TaskId clearCopiedLists = m_scheduler->ScheduleClearCopiedLists(recycleEntityIds);

	/* Main thread helps with tasks while waiting */
	MPL::TaskManager::GetInstance().WaitFor(clearCopiedLists);
}