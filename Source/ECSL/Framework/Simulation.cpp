#include "Simulation.h"

#include <MPL/Managers/TaskManager.h>

using namespace ECSL;

Simulation::Simulation(DataManager* _dataManager, SystemManager* _systemManager, MessageManager* _messageManager)
: m_dataManager(_dataManager), m_systemManager(_systemManager), m_messageManager(_messageManager)
{
	m_scheduler = new Scheduler(_dataManager, _systemManager, _messageManager);
	m_scheduler->AddUpdateSystemsTasks();
	m_scheduler->AddUpdateEntityTableTask();
	m_scheduler->AddUpdateSystemEntityListsTasks();
	m_scheduler->AddEntitiesAddedTasks();
	m_scheduler->AddEntitiesRemovedTasks();
	m_scheduler->AddSortMessagesTask();
	m_scheduler->AddMessagesRecievedTasks();
	m_scheduler->AddDeleteMessagesTask();
	m_scheduler->AddClearDeadEntitiesTask();
	m_scheduler->AddRecycleEntityIdsTask();
	m_scheduler->AddClearChangeListsTask();
	m_scheduler->AddClearListsTask();
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

	/* Update entity component table data */
	MPL::TaskId updateEntityTable = m_scheduler->ScheduleUpdateEntityTable(deleteMessages);

	/* Update every systems' entity list */
	MPL::TaskId updateSystemEntityLists = m_scheduler->ScheduleUpdateSystemEntityLists(updateEntityTable);

	/* Call EntitiesAdded for each system that has atleast one newly added entity */
	MPL::TaskId entitiesAdded = m_scheduler->ScheduleEntitiesAdded(updateSystemEntityLists);

	/* Call EntitiesRemoved for each system that has atleast one newly removed entity */
	MPL::TaskId entitiesRemoved = m_scheduler->ScheduleEntitiesRemoved(entitiesAdded);

	/* Clear dead entities from entity table */
	/* TODO: Kill Entity under EntitiesAdded/EntitiesRemoved orsakar problem */
	MPL::TaskId clearDeadEntities = m_scheduler->ScheduleClearDeadEntities(entitiesRemoved);

	/* Recycle all dead ids back to the list of available ids */
	MPL::TaskId recycleEntityIds = m_scheduler->ScheduleRecycleEntities(clearDeadEntities);

	/* Clear all the used lists in DataManager */
	MPL::TaskId clearChangeLists = m_scheduler->ScheduleClearChangeLists(recycleEntityIds);

	/* Clear all the used lists in Scheduler */
	MPL::TaskId clearLists = m_scheduler->ScheduleClearLists(clearChangeLists);

	MPL::TaskManager::GetInstance().WaitFor(clearChangeLists);
}