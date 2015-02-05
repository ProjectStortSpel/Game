#include "SQSimulation.h"

using namespace ECSL;

SQSimulation::SQSimulation()
{

}

SQSimulation::~SQSimulation()
{

}

void SQSimulation::Update(float _dt)
{
	/* Update dt for every work item */
	m_scheduler->UpdateDt(_dt);

	/* Update work item lists (Temporary add / remove work items) */
	m_scheduler->UpdateWorkItemLists();

	/* Call Update() in every system */
	m_scheduler->PerformUpdateSystems();

	/* Sort all messages sent by systems */
	m_scheduler->PerformSortMessages();

	/* Call MessagesReceived() in each system that has atleast one message received */
	m_scheduler->PerformMessagesReceived();

	/* Delete all messages */
	m_scheduler->PerformDeleteMessages();

	/* Copy all entity and component changes to new lists */
	m_scheduler->PerformCopyCurrentLists();

	/* Update entity component table data */
	m_scheduler->PerformUpdateEntityTable();

	/* Update every systems' entity list */
	m_scheduler->PerformUpdateSystemEntityLists();

	/* Call EntitiesAdded() in each system that has atleast one newly added entity */
	m_scheduler->PerformEntitiesAdded();

	/* Call EntitiesRemoved() in each system that has atleast one newly removed entity */
	m_scheduler->PerformEntitiesRemoved();

	/* Clear all the used lists in Scheduler */
	m_scheduler->PerformClearSystemEntityChangeLists();

	/* Delete component data */
	m_scheduler->PerformDeleteComponentData();

	/* Recycle all dead ids back to the list of available ids */
	m_scheduler->PerformRecycleEntities();

	/* Clear all the used lists in DataManager */
	m_scheduler->PerformClearCopiedLists();
}