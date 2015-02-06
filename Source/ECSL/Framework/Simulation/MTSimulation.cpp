#include "MTSimulation.h"

using namespace ECSL;

MTSimulation::MTSimulation()
{

}

MTSimulation::~MTSimulation()
{

}

void MTSimulation::Update(float _dt)
{
	/* Update dt for every work item */
	m_scheduler->UpdateDt(_dt);

	/* Update work item lists (Temporary add / remove work items) */
	m_scheduler->UpdateWorkItemLists();

	/* Call Update() in every system */
	MPL::TaskId updateSystems = m_scheduler->ScheduleUpdateSystems(MPL::NoDependency);

	/* Sort all messages sent by systems */
	MPL::TaskId sortMessages = m_scheduler->ScheduleSortMessages(updateSystems);

	/* Call MessagesReceived() in each system that has atleast one message received */
	MPL::TaskId messagesReceived = m_scheduler->ScheduleMessagesReceived(sortMessages);

	/* Delete all messages */
	MPL::TaskId deleteMessages = m_scheduler->ScheduleDeleteMessages(messagesReceived);

	/* Copy all entity and component changes to new lists */
	MPL::TaskId copyCurrentLists = m_scheduler->ScheduleCopyCurrentLists(deleteMessages);

	/* Update entity component table data */
	MPL::TaskId updateEntityTable = m_scheduler->ScheduleUpdateEntityTable(copyCurrentLists);

	/* Update every systems' entity list */
	MPL::TaskId updateSystemEntityLists = m_scheduler->ScheduleUpdateSystemEntityLists(updateEntityTable);

	/* Call EntitiesAdded() in each system that has atleast one newly added entity */
	MPL::TaskId entitiesAdded = m_scheduler->ScheduleEntitiesAdded(updateSystemEntityLists);

	/* Call EntitiesRemoved() in each system that has atleast one newly removed entity */
	MPL::TaskId entitiesRemoved = m_scheduler->ScheduleEntitiesRemoved(entitiesAdded);

	/* Clear all the used lists in Scheduler */
	MPL::TaskId clearSystemEntityChangeLists = m_scheduler->ScheduleClearSystemEntityChangeLists(entitiesRemoved);

	/* Delete component data */
	MPL::TaskId deleteComponentData = m_scheduler->ScheduleDeleteComponentData(clearSystemEntityChangeLists);

	/* Recycle all dead ids back to the list of available ids */
	MPL::TaskId recycleEntityIds = m_scheduler->ScheduleRecycleEntities(deleteComponentData);

	/* Main thread helps with tasks while waiting */
	MPL::TaskManager::GetInstance().WaitFor(recycleEntityIds);
}