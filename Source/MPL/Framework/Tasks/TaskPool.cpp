#include "TaskPool.h"

#include <SDL/SDL_mutex.h>

using namespace MPL;

TaskPool::TaskPool()
: m_openList(new std::map<TaskId, Task*>()), m_queue(new std::list<WorkItem*>()), m_workTaskConnection(new std::map<WorkItem*, Task*>())
{
	m_idMutex = SDL_CreateMutex();
	m_taskMutex = SDL_CreateMutex();
}

TaskPool::~TaskPool()
{

}

TaskId TaskPool::BeginCreateTask(TaskId _dependency, WorkItem* _workItem)
{
	Task* task = new Task();
	task->Id = GenerateTaskId();
	task->Dependency = _dependency;
	task->Work = _workItem;
	task->OpenWorkItemCount = 1;
	AddToOpenList(task);
	if (task->Work != 0)
		AddToQueue(_workItem, task);

	return task->Id;
}

void TaskPool::FinishCreateTask(TaskId _id)
{
	SDL_LockMutex(m_taskMutex);
	DecreaseWorkCount((*m_openList)[_id]);
	SDL_UnlockMutex(m_taskMutex);
}

void TaskPool::CreateChild(TaskId _parentId, WorkItem* _workItem)
{
	SDL_LockMutex(m_taskMutex);
	AddToQueue(_workItem, (*m_openList)[_parentId]);
	SDL_UnlockMutex(m_taskMutex);
}

TaskId TaskPool::GenerateTaskId()
{
	SDL_LockMutex(m_idMutex);
	TaskId id = ++m_nextId;
	SDL_UnlockMutex(m_idMutex);
	return id;
}

void TaskPool::AddToOpenList(Task* _task)
{
	SDL_LockMutex(m_taskMutex);
	(*m_openList)[_task->Id] = _task;
	SDL_UnlockMutex(m_taskMutex);
}

void TaskPool::AddToQueue(WorkItem* _workItem, Task* _task)
{
	SDL_LockMutex(m_taskMutex);

	/* Add work item to queue */
	m_queue->push_back(_workItem);

	/* Create connection between work item and task id */
	(*m_workTaskConnection)[_workItem] = _task;

	/* Increase open work item count */
	IncreaseWorkCount(_task);

	SDL_UnlockMutex(m_taskMutex);
}

void TaskPool::IncreaseWorkCount(Task* _task)
{
	++_task->OpenWorkItemCount;
}

void TaskPool::DecreaseWorkCount(Task* _task)
{
	--_task->OpenWorkItemCount;
	if (_task->OpenWorkItemCount == 0)
	{
		m_openList->erase(_task->Id);
		delete(_task);
	}
}

WorkItem* TaskPool::FetchWork()
{
	SDL_LockMutex(m_taskMutex);

	WorkItem* workItem = 0;

	/* Fetch work to do from the queue */
	for (auto it = m_queue->begin(); it != m_queue->end(); ++it)
	{
		TaskId dependency = (*m_workTaskConnection)[*it]->Dependency;
		/* Accept the task if the dependency task is already done or if task doesn't have dependency */
		if ((dependency != -1 && m_openList->find(dependency) == m_openList->end()) || dependency == -1)
		{
			workItem = *it;
			m_queue->erase(it);
			break;
		}
	}

	SDL_UnlockMutex(m_taskMutex);

	return workItem;
}

void TaskPool::WorkDone(WorkItem* _workItem)
{
	SDL_LockMutex(m_taskMutex);

	/* Find and remove the work item from the queue */
	for (auto it = m_queue->begin(); it != m_queue->end(); ++it)
		if (*it == _workItem)
		{
			m_queue->erase(it);
			m_workTaskConnection->erase(*it);
			break;
		}

	/* Get task through the work task connection */
	Task* task = (*m_workTaskConnection)[_workItem];

	/* Decrease open work item count */
	DecreaseWorkCount(task);

	SDL_UnlockMutex(m_taskMutex);
}

bool TaskPool::IsTaskDone(TaskId _id)
{
	SDL_LockMutex(m_taskMutex);
	bool isDone = (m_openList->find(_id) == m_openList->end());
	SDL_UnlockMutex(m_taskMutex);
	return isDone;
}

