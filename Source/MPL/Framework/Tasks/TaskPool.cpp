#include "TaskPool.h"

#include <SDL/SDL_mutex.h>

using namespace MPL;

TaskPool::TaskPool()
: m_openList(new std::map<TaskId, Task*>()), m_queue(new std::deque<WorkItem*>())
{
	m_idMutex = SDL_CreateMutex();
	m_workItemCountMutex = SDL_CreateMutex();
	m_openListMutex = SDL_CreateMutex();
	m_queueMutex = SDL_CreateMutex();
}

TaskPool::~TaskPool()
{

}

TaskId TaskPool::CreateTask(TaskId _dependency, WorkItem* _workItem)
{
	Task* task = new Task();
	task->Id = GenerateTaskId();
	task->Dependency = _dependency;
	task->ParentId = -1;
	task->Work = _workItem;
	task->OpenWorkItemCount = 1;
	AddToOpenList(task);
	if (task->Work != 0)
		AddToQueue(_workItem, task);

	return task->Id;
}

void TaskPool::CreateChild(TaskId _parentId, WorkItem* _workItem)
{
	AddToQueue(_workItem, (*m_openList)[_parentId]);
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
	SDL_LockMutex(m_openListMutex);
	(*m_openList)[_task->Id] = _task;
	SDL_UnlockMutex(m_openListMutex);
}

void TaskPool::RemoveFromOpenList(Task* _task)
{

}

void TaskPool::AddToQueue(WorkItem* _workItem, Task* _task)
{
	SDL_LockMutex(m_queueMutex);

	/* Add work item to queue */
	m_queue->push_back(_workItem);

	/* Create connection between work item and task id */
	(*m_workTaskConnection)[_workItem] = _task->Id;

	/* Increase open work item count */
	_task->OpenWorkItemCount;

	SDL_UnlockMutex(m_queueMutex);
}