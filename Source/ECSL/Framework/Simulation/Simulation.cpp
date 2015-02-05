#include "Simulation.h"

#include <MPL/Managers/TaskManager.h>

using namespace ECSL;

Simulation::Simulation()
{

}

Simulation::~Simulation()
{
	delete(m_scheduler);
}

void Simulation::Initialize(DataManager* _dataManager, SystemManager* _systemManager, MessageManager* _messageManager)
{
	m_dataManager = _dataManager;
	m_systemManager = _systemManager;
	m_messageManager = _messageManager;
	m_scheduler = new Scheduler(_dataManager, _systemManager, _messageManager);
	m_scheduler->AddUpdateSystemsTasks();
	m_scheduler->AddSortMessagesTask();
	m_scheduler->AddMessagesReceivedTasks();
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